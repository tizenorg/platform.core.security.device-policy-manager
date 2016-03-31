/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */
#include <aul.h>
#include <bundle.h>
#include <tzplatform_config.h>

#include <gio/gio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include "zone.hxx"

#include "smack.h"
#include "process.h"
#include "packman.h"
#include "filesystem.h"
#include "auth/user.h"
#include "auth/group.h"
#include "xml/parser.h"
#include "xml/document.h"
#include "audit/logger.h"

#define ZONE_UID_MIN       60001
#define ZONE_UID_MAX       65000

#define ZONE_GROUP         "users"

#define ZONE_MANIFEST_DIR   CONF_PATH "/zone/"
#define ZONE_PROVISION_DIR  "/tmp/zone/provision/"

#define FREEDESKTOP_LOGIN_INTERFACE \
    "org.freedesktop.login1",   \
    "/org/freedesktop/login1",  \
    "org.freedesktop.login1.Manager"

#define HOME_SMACKLABEL     "User::Home"
#define SHARED_SMACKLABEL   "User::App::Shared"
#define APP_SMACKLABEL   "User::Pkg::"

namespace DevicePolicyManager {

static const char *defaultGroups[] = {"audio", "video", "display", "log", NULL};
static const char *defaultAppDir[] = {"cache", "data", "shared", NULL};

static int setZoneState(uid_t id, int state)
{
    GDBusConnection* connection;
    GError* error = NULL;
    GVariant* param = g_variant_new("(ubb)", id, state, 1);

    connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (connection == NULL) {
        ERROR(std::string("Failed to get system DBUS : ") +  error->message);
        g_error_free(error);
        return -1;
    }

    GVariant* var;
    var = g_dbus_connection_call_sync(connection, FREEDESKTOP_LOGIN_INTERFACE,
                                      "SetUserLinger", param,
                                      NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
                                      &error);
    if (var == NULL) {
        ERROR(std::string("Failed to call Setlinger: ") + error->message);
        g_error_free(error);
        return -1;
    } else {
        g_variant_unref(var);
    }

    /*
        ret = g_dbus_connection_close_sync(connection, NULL, &error);
        if (ret == FALSE) {
            g_printerr("Failed to close system DBUS : %s\n", error->message);
            g_error_free(error);
            return -1;
        }
    */
    return 0;
}

template <typename... Args>
void execute(const std::string& path, Args&&... args)
{
    std::vector<std::string> argsVector = { args... };
    runtime::Process proc(path, argsVector);
    proc.execute();
}



Zone::Zone(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (int)(Zone::createZone)(std::string, std::string));
    context.registerParametricMethod(this, (int)(Zone::removeZone)(std::string));
    context.registerParametricMethod(this, (int)(Zone::lockZone)(std::string));
    context.registerParametricMethod(this, (int)(Zone::unlockZone)(std::string));
    context.registerNonparametricMethod(this, (std::vector<std::string>)(Zone::getZoneList)());
    context.registerParametricMethod(this, (int)(Zone::getZoneState)(std::string));

    context.createNotification("Zone::created");
    context.createNotification("Zone::removed");
}

Zone::~Zone()
{
}

int Zone::createZone(const std::string& name, const std::string& setupWizAppid)
{
    std::string provisionDirPath(ZONE_PROVISION_DIR + name);
    runtime::File provisionDir(provisionDirPath);
    int ret;

    try {
        provisionDir.remove(true);
    } catch (runtime::Exception& e) {}

    try {
        //create a directory for zone setup
        provisionDir.makeDirectory(true);
        runtime::Smack::setAccess(provisionDir, APP_SMACKLABEL + setupWizAppid);

        //launch setup-wizard app
        bundle *b = ::bundle_create();
        ::bundle_add_str(b, "Name", name.c_str());
        ::bundle_add_str(b, "ProvisionDir", provisionDirPath.c_str());

        ret = ::aul_launch_app_for_uid(setupWizAppid.c_str(), b, context.getPeerUid());
        ::bundle_free(b);

        if (ret < 0) {
            throw runtime::Exception("Failed to launch application: " + std::to_string(ret));
        }
    } catch (runtime::Exception& e) {}

    auto create = [name, setupWizAppid, provisionDirPath, this] {
        std::unique_ptr<xml::Document> bundleXml;
        xml::Node::NodeList nodes;
        mode_t old_mask;
        int ret;

        old_mask = ::umask(0);
        try {
            //attach a directory for inotify
            int fd = inotify_init();
            if (fd < 0) {
                throw runtime::Exception("Failed to initialize inotify");
            }

            std::string createfile;
            char inotifyBuf[sizeof (struct inotify_event) + PATH_MAX + 1];
            struct inotify_event *event = reinterpret_cast<struct inotify_event *>((void*)inotifyBuf);
            int wd = inotify_add_watch(fd, provisionDirPath.c_str(), IN_CREATE);

            while (createfile != ".completed") {
                ret = read(fd, inotifyBuf, sizeof(inotifyBuf));
                if (ret < 0) {
                    throw runtime::Exception("Failed to get inotify");
                }
                createfile = event->name;
            }

            inotify_rm_watch(fd, wd);
            close(fd);

            //create zone user
            runtime::User user = runtime::User::create
                                (name, ZONE_GROUP, ZONE_UID_MIN, ZONE_UID_MAX);

            //create zone home directories
            const struct {
                enum tzplatform_variable dir;
                const char* smack;
            } dirs[] = {
                {TZ_USER_HOME, HOME_SMACKLABEL},
                {TZ_USER_CACHE, SHARED_SMACKLABEL},
                {TZ_USER_APPROOT, HOME_SMACKLABEL},
                {TZ_USER_DB, HOME_SMACKLABEL},
                {TZ_USER_PACKAGES, HOME_SMACKLABEL},
                {TZ_USER_ICONS, HOME_SMACKLABEL},
                {TZ_USER_CONFIG, SHARED_SMACKLABEL},
                {TZ_USER_DATA, HOME_SMACKLABEL},
                {TZ_USER_SHARE, SHARED_SMACKLABEL},
                {TZ_USER_ETC, HOME_SMACKLABEL},
                {TZ_USER_LIVE, HOME_SMACKLABEL},
                {TZ_USER_UG, HOME_SMACKLABEL},
                {TZ_USER_APP, HOME_SMACKLABEL},
                {TZ_USER_CONTENT, SHARED_SMACKLABEL},
                {TZ_USER_CAMERA, SHARED_SMACKLABEL},
                {TZ_USER_VIDEOS, SHARED_SMACKLABEL},
                {TZ_USER_IMAGES, SHARED_SMACKLABEL},
                {TZ_USER_SOUNDS, SHARED_SMACKLABEL},
                {TZ_USER_MUSIC, SHARED_SMACKLABEL},
                {TZ_USER_GAMES, SHARED_SMACKLABEL},
                {TZ_USER_DOCUMENTS, SHARED_SMACKLABEL},
                {TZ_USER_OTHERS, SHARED_SMACKLABEL},
                {TZ_USER_DOWNLOADS, SHARED_SMACKLABEL},
                {TZ_SYS_HOME, NULL},
            };

            ::umask(0022);

            ::tzplatform_set_user(user.getUid());
            for (int i = 0; dirs[i].dir != TZ_SYS_HOME; i++) {
                runtime::File dir(std::string(::tzplatform_getenv(dirs[i].dir)));
                try {
                    dir.makeDirectory();
                } catch (runtime::Exception& e) {}
                dir.chown(user.getUid(), user.getGid());
                runtime::Smack::setAccess(dir, dirs[i].smack);
                runtime::Smack::setTransmute(dir, true);
            }
            std::string homeDir = ::tzplatform_getenv(TZ_USER_HOME);
            std::string appDir = ::tzplatform_getenv(TZ_USER_APP);
            ::tzplatform_reset_user();

            //create systemd user unit directory
            runtime::File systemdUserUnit(homeDir + "/.config/systemd/user");
            systemdUserUnit.makeDirectory(true);

            //mask systemd user unit
            ret = ::symlink("/dev/null",
                      (systemdUserUnit.getPath() + "/starter.service").c_str());

            if (ret != 0)
                throw runtime::Exception("Failed to mask starter.service");

            for (int i = 0; defaultGroups[i]; i++) {
                runtime::Group grp(defaultGroups[i]);
                grp.addMember(name);
            }

            //initialize package db
            execute("/usr/bin/pkg_initdb",
                    "pkg_initdb", std::to_string(user.getUid()));

            //initialize security-manager
            execute("/usr/bin/security-manager-cmd",
                    "security-manager-cmd", "--manage-users=add",
                    "--uid=" + std::to_string(user.getUid()),
                    "--usertype=normal");

            //initialize package rw directory in home
            std::vector<std::string> pkgList = PackageManager::instance().
                                        getInstalledPackageList(user.getUid());
            for (std::vector<std::string>::iterator it = pkgList.begin();
                 it != pkgList.end(); it++) {
                runtime::File dir(appDir + "/" + *it);
                try {
                    dir.makeDirectory();
                    dir.chown(user.getUid(), user.getGid());
                    runtime::Smack::setAccess(dir, APP_SMACKLABEL + *it);
                    runtime::Smack::setTransmute(dir, true);
                    for (int i = 0; defaultAppDir[i]; i++) {
                        runtime::File insideDir(dir.getPath() + "/" + defaultAppDir[i]);
                        insideDir.makeDirectory();
                        insideDir.chown(user.getUid(), user.getGid());
                    }
                } catch (runtime::Exception& e) {}
            }

            //read manifest xml file
            bundleXml = std::unique_ptr<xml::Document>(xml::Parser::parseFile(provisionDirPath + "/manifest.xml"));

            //execute hooks of creation
            std::vector<std::string> args;
            args.push_back("");
            args.push_back(name);
            args.push_back(std::to_string(user.getUid()));
            args.push_back(std::to_string(user.getGid()));

            nodes = bundleXml->evaluate("//bundle-manifest/hooks/create");
            for (xml::Node::NodeList::iterator it = nodes.begin();
                    it != nodes.end(); it++) {
                std::string path = it->getChildren().begin()->getContent();
                args[0] = path;
                runtime::Process exec(path, args);
                exec.execute();
            }

            //TODO: write container owner info

            //write manifest file
            ::umask(0077);
            bundleXml->write(ZONE_MANIFEST_DIR + name + ".xml", "UTF-8", true);

            //unlock the user
            setZoneState(user.getUid(), 1);

            context.notify("Zone::created", name, std::string());
        } catch (runtime::Exception& e) {
            ERROR(e.what());
        }
        ::umask(old_mask);
    };

    std::thread asyncWork(create);
    asyncWork.detach();

    return 0;
}

int Zone::removeZone(const std::string& name)
{
    int ret;

    //lock the user
    ret = lockZone(name);
    if (ret != 0) {
        return -1;
    }

    auto remove = [name, this] {
        runtime::File bundle(ZONE_MANIFEST_DIR + name + ".xml");
        std::unique_ptr<xml::Document> bundleXml;
        xml::Node::NodeList nodes;

        try {
            runtime::User user(name);

            //remove notification for ckm-tool
            execute("/usr/bin/ckm_tool",
                    "ckm_tool", "-d", std::to_string(user.getUid()));
            //initialize security-manager
            execute("/usr/bin/security-manager-cmd",
                    "security-manager-cmd",
                    "--manage-users=remove",
                    "--uid=" + std::to_string(user.getUid()));

            //execute hooks of destroy
            std::vector<std::string> args;
            args.push_back("");
            args.push_back(name);
            args.push_back(std::to_string(user.getUid()));
            args.push_back(std::to_string(user.getGid()));

            bundleXml = std::unique_ptr<xml::Document>(xml::Parser::parseFile(bundle.getPath()));
            nodes = bundleXml->evaluate("//bundle-manifest/hooks/destroy");
            for (xml::Node::NodeList::iterator it = nodes.begin();
                    it != nodes.end(); it++) {
                std::string path = it->getChildren().begin()->getContent();
                args[0] = path;
                runtime::Process exec(path, args);
                exec.execute();
            }

            //remove zone user
            user.remove();

            bundle.remove();

            context.notify("Zone::removed", name, std::string());
        } catch (runtime::Exception& e) {
            ERROR(e.what());
            return;
        }
    };

    std::thread asyncWork(remove);
    asyncWork.detach();

    return 0;
}

int Zone::lockZone(const std::string& name)
{
    int result;

    try {
        runtime::User user(name);

        result = setZoneState(user.getUid(), 0);
        if (result != 0) {
            return -1;
        }
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int Zone::unlockZone(const std::string& name)
{
    int result;

    try {
        runtime::User user(name);

        result = setZoneState(user.getUid(), 1);
        if (result != 0) {
            return -1;
        }
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

std::vector<std::string> Zone::getZoneList()
{
    std::vector<std::string> list;
    try {
        runtime::DirectoryIterator iter(runtime::Path(ZONE_MANIFEST_DIR)), end;

        while (iter != end) {
            const std::string& path = (*iter).getPath();
            size_t name = path.rfind('/') + 1;
            size_t ext = path.rfind(".xml");
            list.push_back(path.substr(name, ext - name));
            ++iter;
        }
    } catch (runtime::Exception& e) {}

    return list;
}

int Zone::getZoneState(const std::string& name)
{
    return 0;
}

Zone zonePolicy(Server::instance());

} // namespace DevicePolicyManager

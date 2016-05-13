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
#include "dbus/connection.h"

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

static void setZoneState(uid_t id, int state)
{
    dbus::Connection& systemDBus = dbus::Connection::getSystem();
    systemDBus.methodcall(FREEDESKTOP_LOGIN_INTERFACE, "SetUserLinger",
                          -1, "", "(ubb)", id, state, 1);
}

template <typename... Args>
void execute(const std::string& path, Args&&... args)
{
    std::vector<std::string> argsVector = { args... };
    runtime::Process proc(path, argsVector);
    proc.execute();
}



ZonePolicy::ZonePolicy(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (int)(ZonePolicy::createZone)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::removeZone)(std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::lockZone)(std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::unlockZone)(std::string));
    context.registerNonparametricMethod(this, (std::vector<std::string>)(ZonePolicy::getZoneList)());
    context.registerParametricMethod(this, (int)(ZonePolicy::getZoneState)(std::string));

    context.createNotification("ZonePolicy::created");
    context.createNotification("ZonePolicy::removed");
}

ZonePolicy::~ZonePolicy()
{
}

int ZonePolicy::createZone(const std::string& name, const std::string& setupWizAppid)
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
                std::string path = it->getContent();
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

            context.notify("ZonePolicy::created", name, std::string());
        } catch (runtime::Exception& e) {
            ERROR(e.what());
        }
        ::umask(old_mask);
    };

    std::thread asyncWork(create);
    asyncWork.detach();

    return 0;
}

int ZonePolicy::removeZone(const std::string& name)
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
                std::string path = it->getContent();
                args[0] = path;
                runtime::Process exec(path, args);
                exec.execute();
            }

            //remove zone user
            user.remove();

            bundle.remove();

            context.notify("ZonePolicy::removed", name, std::string());
        } catch (runtime::Exception& e) {
            ERROR(e.what());
            return;
        }
    };

    std::thread asyncWork(remove);
    asyncWork.detach();

    return 0;
}

int ZonePolicy::lockZone(const std::string& name)
{
    try {
        runtime::User user(name);
        setZoneState(user.getUid(), 0);
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int ZonePolicy::unlockZone(const std::string& name)
{
    try {
        runtime::User user(name);
        setZoneState(user.getUid(), 1);
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

std::vector<std::string> ZonePolicy::getZoneList()
{
    return std::vector<std::string>();
}

int ZonePolicy::getZoneState(const std::string& name)
{
    return 0;
}

ZonePolicy zonePolicy(Server::instance());

} // namespace DevicePolicyManager

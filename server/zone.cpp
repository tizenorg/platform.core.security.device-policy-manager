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
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include <tzplatform_config.h>

#include "zone.hxx"

#include "error.h"
#include "smack.h"
#include "process.h"
#include "packman.h"
#include "launchpad.h"
#include "filesystem.h"
#include "auth/user.h"
#include "auth/group.h"
#include "xml/parser.h"
#include "xml/document.h"
#include "audit/logger.h"
#include "dbus/connection.h"

#define ZONE_UID_MIN       60001
#define ZONE_UID_MAX       65000

namespace DevicePolicyManager {

namespace {

const std::vector<std::string> defaultGroups = {
    "audio",
    "video",
    "display",
    "log"
};

const std::vector<std::string> defaultAppDirs = {
    "cache",
    "data",
    "shared"
};

const std::vector<std::string> unitsToMask = {
    "starter.service",
    "scim.service"
};

const std::string ZONE_MANIFEST_DIR = CONF_PATH "/zone/";
const std::string ZONE_PROVISION_DIR = "/tmp/zone/provision/";

const std::string HOME_SMACKLABEL = "User::Home";
const std::string SHARED_SMACKLABEL = "User::App::Shared";
const std::string APP_SMACKLABEL = "User::Pkg::";

const std::string ZONE_GROUP = "users";

template <typename... Args>
inline void execute(const std::string& path, Args&&... args)
{
    std::vector<std::string> argsVector = { args... };
    runtime::Process proc(path, argsVector);
    proc.execute();
}

void waitForSetupWizard(const std::string& watch)
{
    int fd = ::inotify_init();
    if (fd < 0) {
        throw runtime::Exception("Failed to initialize inotify");
    }

    std::string createfile;
    char inotifyBuf[sizeof (struct inotify_event) + PATH_MAX + 1];
    struct inotify_event *event = reinterpret_cast<struct inotify_event *>((void*)inotifyBuf);
    int wd = ::inotify_add_watch(fd, watch.c_str(), IN_CREATE);

    while (createfile != ".completed") {
        int ret = ::read(fd, inotifyBuf, sizeof(inotifyBuf));
        if (ret < 0) {
            throw runtime::Exception("Failed to get inotify");
        }
        createfile = event->name;
    }

    ::inotify_rm_watch(fd, wd);
    ::close(fd);
}

std::string prepareDirectories(const runtime::User& user)
{
    //create zone home directories
    const struct {
        enum tzplatform_variable dir;
        const std::string& smack;
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
        {TZ_SYS_HOME, ""},
    };

    ::umask(0022);

    ::tzplatform_set_user(user.getUid());

    std::string pivot(::tzplatform_getenv(TZ_USER_HOME));

    try {
        for (int i = 0; dirs[i].dir != TZ_SYS_HOME; i++) {
            runtime::File dir(::tzplatform_getenv(dirs[i].dir));
            dir.makeDirectory(false, user.getUid(), user.getGid());
            runtime::Smack::setAccess(dir, dirs[i].smack);
            runtime::Smack::setTransmute(dir, true);
        }
    } catch (runtime::Exception& e) {
        ::tzplatform_reset_user();
        throw runtime::Exception(e.what());
    }

    ::tzplatform_reset_user();

    return pivot;
}

void deployPackages(const runtime::User& user)
{
    try {
        //initialize package db
        execute("/usr/bin/pkg_initdb", "pkg_initdb",
                "--uid", std::to_string(user.getUid()));

        PackageManager& packageManager = PackageManager::instance();
        std::vector<std::string> pkgList = packageManager.getPackageList(user.getUid());

        ::umask(0022);

        ::tzplatform_set_user(user.getUid());
        for (const std::string& pkgid : pkgList) {
            std::string appbase = std::string(::tzplatform_getenv(TZ_USER_APP)) + "/" + pkgid;
            runtime::File dir(appbase);
            dir.makeDirectory(false, user.getUid(), user.getGid());
            runtime::Smack::setAccess(dir, APP_SMACKLABEL + pkgid);
            runtime::Smack::setTransmute(dir, true);

            for (const std::string& subdir : defaultAppDirs) {
                runtime::File insideDir(appbase + "/" + subdir);
                insideDir.makeDirectory(false, user.getUid(), user.getGid());
            }
        }
        ::tzplatform_reset_user();
    } catch (runtime::Exception& e) {
        ::tzplatform_reset_user();
        throw runtime::Exception(e.what());
    }
}

void maskUserServices(const std::string& pivot, const runtime::User& user)
{
    runtime::File unitbase(pivot + "/.config/systemd/user");
    unitbase.makeDirectory(true);
    unitbase.chmod(S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

    for (const std::string& unit : unitsToMask) {
        std::string target = unitbase.getPath() + "/" + unit;
        if (::symlink("/dev/null", target.c_str()) == -1) {
            throw runtime::Exception(runtime::GetSystemErrorMessage());
        }
    }
}

void setZoneState(uid_t id, int state)
{
    dbus::Connection& systemDBus = dbus::Connection::getSystem();
    systemDBus.methodcall("org.freedesktop.login1",
                          "/org/freedesktop/login1",
                          "org.freedesktop.login1.Manager",
                          "SetUserLinger",
                          -1, "", "(ubb)", id, state, 1);
}

void startZoneProvisioningThread(PolicyControlContext& context, const std::string& name, const std::string& watch)
{
    auto provisioningWorker = [name, watch, &context]() {
        std::unique_ptr<xml::Document> manifest;

        mode_t omask = ::umask(0);
        try {
            waitForSetupWizard(watch);

            //create zone user
            runtime::User user = runtime::User::create(name, ZONE_GROUP, ZONE_UID_MIN, ZONE_UID_MAX);
            for (const std::string& grp : defaultGroups) {
                runtime::Group group(grp);
                group.addMember(name);
            }

            std::string pivot = prepareDirectories(user);
            maskUserServices(pivot, user);
            deployPackages(user);

            //initialize security-manager
            execute("/usr/bin/security-manager-cmd",
                    "security-manager-cmd", "--manage-users=add",
                    "--uid=" + std::to_string(user.getUid()),
                    "--usertype=normal");

            //change group to system_share
            runtime::Group systemShareGroup("system_share");
            ::tzplatform_set_user(user.getUid());
            runtime::File appRootDir(::tzplatform_getenv(TZ_USER_APPROOT));
            runtime::File dbDir(::tzplatform_getenv(TZ_USER_DB));
            ::tzplatform_reset_user();
            appRootDir.chown(user.getUid(), systemShareGroup.getGid());
            appRootDir.chmod(0750);

            dbDir.chown(user.getUid(), systemShareGroup.getGid());
            dbDir.chmod(0770);

            manifest.reset(xml::Parser::parseFile(watch + "/manifest.xml"));
            ::umask(0077);
            manifest->write(ZONE_MANIFEST_DIR + name + ".xml", "UTF-8", true);

            //TODO: write container owner info

            //unlock the user
            setZoneState(user.getUid(), 1);

            context.notify("ZonePolicy::created", name, std::string());
        } catch (runtime::Exception& e) {
            ERROR(e.what());
            context.notify("ZonePolicy::removed", name, std::string());
        }

        ::umask(omask);
    };

    std::thread asyncWork(provisioningWorker);
    asyncWork.detach();
}

const std::string convertPathForOwner(const std::string& path, const runtime::User& user, const runtime::User& owner)
{
    ::tzplatform_set_user(owner.getUid());
    std::string ownerHome(::tzplatform_getenv(TZ_USER_HOME));
    ::tzplatform_reset_user();
    ownerHome += "/.zone";

    ::tzplatform_set_user(user.getUid());
    std::string userHome(::tzplatform_getenv(TZ_USER_HOME));
    ::tzplatform_reset_user();

    std::string userHomeForOwner(ownerHome + "/" + user.getName());

    std::string convertedPath(path);

    if (convertedPath.compare(0, userHome.size(), userHome) == 0) {
        convertedPath.replace(0, userHome.size(), userHomeForOwner);
    }

    return convertedPath;
}

void prepareFileForOwner(const std::string path, const runtime::User& pkgUser, const runtime::User& owner)
{
    std::string pathLink = convertPathForOwner(path, pkgUser, owner);

    if (path != pathLink) {
        runtime::File linkFile(pathLink);
        linkFile.makeBaseDirectory(pkgUser.getUid(), pkgUser.getGid());
        if (linkFile.exists()) {
            linkFile.remove();
        }

        int ret = ::link(path.c_str(), pathLink.c_str());
        if (ret != 0) {
            //TODO: copy the icon instead of linking
            throw runtime::Exception("Failed to link from " + path +
                                     " to " + pathLink);
        }
    }
}

int packageEventHandler(uid_t target_uid, int req_id,
                        const char *pkg_type, const char *pkgid,
                        const char *key, const char *val,
                        const void *pmsg, void *data)
{
    static std::string type;
    std::string keystr = key;

    if (target_uid == tzplatform_getuid(TZ_SYS_GLOBALAPP_USER)) {
        return 0;
    }

    std::transform(keystr.begin(), keystr.end(), keystr.begin(), ::tolower);
    if (keystr == "start") {
        type = val;
        std::transform(type.begin(), type.end(), type.begin(), ::tolower);
        return 0;
    } else if (keystr != "end" && keystr != "ok") {
        return 0;
    }

    try {
        runtime::User owner("owner"), pkgUser(target_uid);

        if (type == "install" || type == "update") {
            PackageInfo info(pkgid, pkgUser.getUid());
            std::string icon = info.getIcon();
            prepareFileForOwner(icon, pkgUser, owner);

            for (const std::string &appid : info.getAppList()) {
                ApplicationInfo info(appid, pkgUser.getUid());
                std::string icon = info.getIcon();
                prepareFileForOwner(icon, pkgUser, owner);
            }
        } else {
            ::tzplatform_set_user(pkgUser.getUid());
            std::string pkgPath(::tzplatform_getenv(TZ_USER_APP));
            pkgPath = pkgPath + "/" + pkgid;
            ::tzplatform_reset_user();

            runtime::File pkgDirForOwner(convertPathForOwner(pkgPath, pkgUser, owner));
            pkgDirForOwner.remove(true);
        }
    } catch (runtime::Exception &e) {
        ERROR(e.what());
    }

    return 0;
}

} // namespace

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

    PackageManager& packageManager = PackageManager::instance();
    packageManager.setEventCallback(packageEventHandler, this);
}

ZonePolicy::~ZonePolicy()
{
    PackageManager& packageManager = PackageManager::instance();
    packageManager.unsetEventCallback();
}

int ZonePolicy::createZone(const std::string& name, const std::string& setupWizAppid)
{
    std::string provisionDirPath(ZONE_PROVISION_DIR + name);
    runtime::File provisionDir(provisionDirPath);

    try {
        if (provisionDir.exists()) {
            provisionDir.remove(true);
        }

        //create a directory for zone setup
        provisionDir.makeDirectory(true);
        runtime::Smack::setAccess(provisionDir, APP_SMACKLABEL + setupWizAppid);

        Bundle bundle;
        bundle.add("zone", name);
        bundle.add("provisionDir", provisionDirPath);

        Launchpad launchpad(context.getPeerUid());
        launchpad.launch(setupWizAppid, bundle);

        startZoneProvisioningThread(context, name, provisionDirPath);
    } catch (runtime::Exception& e) {
        ERROR(e.what());

        if (provisionDir.exists()) {
            provisionDir.remove(true);
        }
        return -1;
    }

    return 0;
}

int ZonePolicy::removeZone(const std::string& name)
{
    if (lockZone(name) != 0) {
        return -1;
    }

    auto remove = [name, this] {
        runtime::File manifest(ZONE_MANIFEST_DIR + name + ".xml");

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

            //remove zone user
            user.remove();
            manifest.remove();

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

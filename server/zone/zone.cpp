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

#include <gum/gum-user.h>
#include <gum/common/gum-user-types.h>
#include <notification.h>
#include <notification_internal.h>
#include <tzplatform_config.h>
#include <auth-passwd-admin.h>

#include "zone/zone.hxx"

#include "error.h"
#include "process.h"
#include "packman.h"
#include "launchpad.h"
#include "filesystem.h"
#include "auth/user.h"
#include "xml/parser.h"
#include "xml/document.h"
#include "audit/logger.h"
#include "dbus/connection.h"

#define ZONE_DELEGATOR_APP  "org.tizen.keyguard"
#define NOTIFICATION_SUB_ICON_PATH  DATA_PATH "/zone_noti_list_sub_icon.png"

namespace DevicePolicyManager {

namespace {

const std::vector<std::string> defaultGroups = {
    "audio",
    "video",
    "display",
    "log"
};

const std::vector<std::string> unitsToMask = {
    "starter.service",
    "scim.service"
};

const std::string ZONE_MANIFEST_DIR = CONF_PATH "/zone/";
const std::string ZONE_SKEL_PATH = "/etc/skel";
const std::string ZONE_CREATE_HOOK_PATH = "/etc/gumd/useradd.d";
const std::string ZONE_REMOVE_HOOK_PATH = "/etc/gumd/userdel.d";

const std::string ZONE_DEFAULT_OWNER = "owner";

const std::string ZONE_GROUP = "users";

std::vector<std::string> createdZoneList;
static std::atomic<bool> isZoneForeground(false);

std::unordered_map<std::string, int> notiProxyCallbackMap;
std::unordered_map<int, notification_h> notiHandleMap;

inline void maskUserServices(const runtime::User& user)
{
    ::tzplatform_set_user(user.getUid());
    std::string pivot(::tzplatform_getenv(TZ_USER_HOME));
    ::tzplatform_reset_user();

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

inline void setZoneState(uid_t id, int state)
{
    dbus::Connection& systemDBus = dbus::Connection::getSystem();
    systemDBus.methodcall("org.freedesktop.login1",
                          "/org/freedesktop/login1",
                          "org.freedesktop.login1.Manager",
                          "SetUserLinger",
                          -1, "", "(ubb)", id, state, 1);
}

inline const std::string convertPathForOwner(const std::string& path, const runtime::User& user, const runtime::User& owner)
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

inline void prepareFileForOwner(const std::string path, const runtime::User& pkgUser, const runtime::User& owner)
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
        runtime::User owner(ZONE_DEFAULT_OWNER), pkgUser(target_uid);

        if (type == "install" || type == "update") {
            PackageInfo pkg(pkgid, pkgUser.getUid());
            std::string icon = pkg.getIcon();
            prepareFileForOwner(icon, pkgUser, owner);

            for (const ApplicationInfo& app : pkg.getAppList()) {
                std::string icon = app.getIcon();
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

void initializeCreatedZoneList() {
    try {
        runtime::DirectoryIterator iter(ZONE_MANIFEST_DIR), end;

        while (iter != end) {
            const std::string& file = iter->getName();
            const std::string& name(file.substr(0, file.rfind(".xml")));
            createdZoneList.push_back(name);
            ++iter;
        }
    } catch (runtime::Exception& e) {}
}

#define NT_TITLE     NOTIFICATION_TEXT_TYPE_TITLE
#define NT_CONTENT   NOTIFICATION_TEXT_TYPE_CONTENT
#define NT_ICON      NOTIFICATION_IMAGE_TYPE_ICON
#define NT_INDICATOR NOTIFICATION_IMAGE_TYPE_ICON_FOR_INDICATOR
#define NT_NONE      NOTIFICATION_VARIABLE_TYPE_NONE
#define NT_EVENT     NOTIFICATION_LY_ONGOING_PROGRESS
#define NT_APP       NOTIFICATION_DISPLAY_APP_INDICATOR

#define NT_ICON_PATH DATA_PATH "/zone_indicator_icon.png"
#define NT_TEXT      "Container Mode"
#define NT_APPINFO   "Zone Application"

#define NT_ERROR_NONE   NOTIFICATION_ERROR_NONE

void zoneProcessCallback(GDBusConnection *connection,
	                     const gchar *sender, const gchar *objectPath,
	                     const gchar *interface, const gchar *signalName,
	                     GVariant *params, gpointer userData)
{
    static runtime::User owner(ZONE_DEFAULT_OWNER);
    int pid, status;

    notification_h noti = reinterpret_cast<notification_h>(userData);

    g_variant_get(params, "(ii)", &status, &pid);

    if (status != 5) {
        return;
    }

    struct stat st;
    std::string proc("/proc/" + std::to_string(pid));
    if (::stat(proc.c_str(), &st) != 0) {
        return;
    }

    if (owner.getUid() != st.st_uid) {
        if (!isZoneForeground) {
            notification_set_text(noti, NT_CONTENT, NT_APPINFO, NULL, NT_NONE);
            notification_post_for_uid(noti, owner.getUid());
            isZoneForeground = true;
        }
    } else {
        if (isZoneForeground) {
            notification_delete_for_uid(noti, owner.getUid());
            isZoneForeground = false;
        }
    }
}

notification_h createNotification()
{
    notification_h noti = notification_create(NOTIFICATION_TYPE_ONGOING);
    if (noti == NULL) {
        return NULL;
    }

    if (notification_set_text(noti, NT_TITLE, NT_TEXT, NULL, NT_NONE) != NT_ERROR_NONE) {
        notification_free(noti);
        return NULL;
    }
    if (notification_set_image(noti, NT_ICON, NT_ICON_PATH) != NT_ERROR_NONE) {
        notification_free(noti);
        return NULL;
    }

    if (notification_set_image(noti, NT_INDICATOR, NT_ICON_PATH) != NT_ERROR_NONE) {
        notification_free(noti);
        return NULL;
    }

    if (notification_set_layout(noti, NT_EVENT) != NT_ERROR_NONE) {
        notification_free(noti);
        return NULL;
    }

    if (notification_set_display_applist(noti, NT_APP) != NT_ERROR_NONE) {
        notification_free(noti);
        return NULL;
    }

    return noti;
}

void zoneProcessMonitor()
{
    GError *error = NULL;
    GDBusConnection* connection;
    connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (connection == NULL) {
        ERROR("GBus Connection failed");
        g_error_free(error);
        return;
    }

    notification_h noti = createNotification();
    if (noti == NULL) {
        ERROR("Failed to created notification");
        return;
    }

    g_dbus_connection_signal_subscribe(connection,
                                        NULL,
                                        "org.tizen.resourced.process",
                                        "ProcStatus",
                                        "/Org/Tizen/ResourceD/Process",
                                        NULL,
                                        G_DBUS_SIGNAL_FLAGS_NONE,
                                        zoneProcessCallback,
                                        reinterpret_cast<gpointer>(noti),
                                        NULL);
}

void notiProxyInsert(const runtime::User& owner, const runtime::User& user, int privId, notification_h noti)
{
    std::string zoneLauncherUri;
    notification_h newNoti;
    app_control_h appControl;
    char* pkgId;

    notification_clone(noti, &newNoti);

    notification_get_pkgname(noti, &pkgId);
    PackageInfo pkg(pkgId, user.getUid());
    notification_set_image(newNoti, NOTIFICATION_IMAGE_TYPE_ICON, pkg.getIcon().c_str());
    notification_set_image(newNoti, NOTIFICATION_IMAGE_TYPE_ICON_SUB, NOTIFICATION_SUB_ICON_PATH);

    notification_get_launch_option(newNoti, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, (void *)&appControl);
    if (appControl != NULL) {
        char* appId = NULL, *uri = NULL;

        app_control_get_app_id(appControl, &appId);
        if (appId == NULL) {
            appId = strdup("");
        }

        zoneLauncherUri = "zone://enter/" + user.getName() + "/" + appId;

        app_control_get_uri(appControl, &uri);
        if (uri != NULL) {
            zoneLauncherUri += "?uri=";
            zoneLauncherUri +=  uri;
            free(uri);
        }

        free(appId);
        app_control_set_app_id(appControl, ZONE_DELEGATOR_APP);
        app_control_set_uri(appControl, zoneLauncherUri.c_str());
        notification_set_launch_option(newNoti, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, appControl);
    }

    notification_post_for_uid(newNoti, owner.getUid());
    notiHandleMap.insert(std::make_pair(privId, newNoti));
}

void notiProxyDelete(const runtime::User& owner, int privId)
{
    std::unordered_map<int, notification_h>::iterator it;

    it = notiHandleMap.find(privId);
    if (it == notiHandleMap.end()) {
        return;
    }
    notification_delete_for_uid(it->second, owner.getUid());
    notification_free(it->second);
    notiHandleMap.erase(it);
}

void notiProxyUpdate(const runtime::User& owner, const runtime::User& user, int privId, notification_h noti) {
    std::unordered_map<int, notification_h>::iterator it;
    double progress;
    char *str;

    it = notiHandleMap.find(privId);
    if (it == notiHandleMap.end()) {
        return;
    }

    notification_image_type_e imageTypes[] = {
        NOTIFICATION_IMAGE_TYPE_ICON,
        NOTIFICATION_IMAGE_TYPE_ICON_FOR_INDICATOR,
        NOTIFICATION_IMAGE_TYPE_ICON_FOR_LOCK,
        NOTIFICATION_IMAGE_TYPE_THUMBNAIL,
        NOTIFICATION_IMAGE_TYPE_THUMBNAIL_FOR_LOCK,
        NOTIFICATION_IMAGE_TYPE_ICON_SUB,
        NOTIFICATION_IMAGE_TYPE_BACKGROUND,
        NOTIFICATION_IMAGE_TYPE_LIST_1,
        NOTIFICATION_IMAGE_TYPE_LIST_2,
        NOTIFICATION_IMAGE_TYPE_LIST_3,
        NOTIFICATION_IMAGE_TYPE_LIST_4,
        NOTIFICATION_IMAGE_TYPE_LIST_5,
        NOTIFICATION_IMAGE_TYPE_BUTTON_1,
        NOTIFICATION_IMAGE_TYPE_BUTTON_2,
        NOTIFICATION_IMAGE_TYPE_BUTTON_3,
        NOTIFICATION_IMAGE_TYPE_BUTTON_4,
        NOTIFICATION_IMAGE_TYPE_BUTTON_5,
        NOTIFICATION_IMAGE_TYPE_BUTTON_6
    };

    for (notification_image_type_e type : imageTypes) {
        notification_get_image(noti, type, &str);
        notification_set_image(it->second, type, str);
    }

    notification_text_type_e textTypes[] = {
        NOTIFICATION_TEXT_TYPE_TITLE,
        NOTIFICATION_TEXT_TYPE_CONTENT,
        NOTIFICATION_TEXT_TYPE_CONTENT_FOR_DISPLAY_OPTION_IS_OFF,
        NOTIFICATION_TEXT_TYPE_EVENT_COUNT,
        NOTIFICATION_TEXT_TYPE_INFO_1,
        NOTIFICATION_TEXT_TYPE_INFO_SUB_1,
        NOTIFICATION_TEXT_TYPE_INFO_2,
        NOTIFICATION_TEXT_TYPE_INFO_SUB_2,
        NOTIFICATION_TEXT_TYPE_INFO_3,
        NOTIFICATION_TEXT_TYPE_INFO_SUB_3,
        NOTIFICATION_TEXT_TYPE_GROUP_TITLE,
        NOTIFICATION_TEXT_TYPE_GROUP_CONTENT,
        NOTIFICATION_TEXT_TYPE_GROUP_CONTENT_FOR_DISPLAY_OPTION_IS_OFF,
        NOTIFICATION_TEXT_TYPE_BUTTON_1,
        NOTIFICATION_TEXT_TYPE_BUTTON_2,
        NOTIFICATION_TEXT_TYPE_BUTTON_3,
        NOTIFICATION_TEXT_TYPE_BUTTON_4,
        NOTIFICATION_TEXT_TYPE_BUTTON_5,
        NOTIFICATION_TEXT_TYPE_BUTTON_6,
    };

    for (notification_text_type_e type : textTypes) {
        notification_get_text(noti, type, &str);
        notification_set_text(it->second, type, str, NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
    }

    notification_get_size(noti, &progress);
    notification_set_size(it->second, progress);

    notification_get_progress(noti, &progress);
    notification_set_progress(it->second, progress);

    notification_update_for_uid(it->second, owner.getUid());
}

void notiProxyCallback(void *data, notification_type_e type, notification_op *op_list, int num_op)
{
    static runtime::User owner(ZONE_DEFAULT_OWNER);
    runtime::User user(*reinterpret_cast<std::string*>(data));

    // TODO : should remove noti in the zone when related-zone is removed
    //        This will be imlemented when notification bug is fixed

    for (int i = 0; i < num_op; i++) {
        notification_h noti = NULL;
        int opType, privId;

        notification_op_get_data(op_list + i, NOTIFICATION_OP_DATA_TYPE, &opType);
        notification_op_get_data(op_list + i, NOTIFICATION_OP_DATA_PRIV_ID, &privId);

        switch (opType) {
        case NOTIFICATION_OP_INSERT:
            notification_op_get_data(op_list + i, NOTIFICATION_OP_DATA_NOTI, &noti);
            notiProxyInsert(owner, user, privId, noti);
            break;
        case NOTIFICATION_OP_DELETE:
            notiProxyDelete(owner, privId);
            break;
        case NOTIFICATION_OP_UPDATE:
            notification_op_get_data(op_list + i, NOTIFICATION_OP_DATA_NOTI, &noti);
            notiProxyUpdate(owner, user, privId, noti);
            break;
        }
    }
}

} // namespace

ZoneManager::ZoneManager(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, "", (int)(ZoneManager::createZone)(std::string, std::string));
    context.registerParametricMethod(this, "", (int)(ZoneManager::removeZone)(std::string));
    context.registerParametricMethod(this, "", (int)(ZoneManager::lockZone)(std::string));
    context.registerParametricMethod(this, "", (int)(ZoneManager::unlockZone)(std::string));
    context.registerParametricMethod(this, "", (int)(ZoneManager::getZoneState)(std::string));
    context.registerParametricMethod(this, "", (std::vector<std::string>)(ZoneManager::getZoneList)(int));
    context.registerParametricMethod(this, "", (int)(ZoneManager::resetZonePassword)(std::string, std::string));

    context.createNotification("ZoneManager::created");
    context.createNotification("ZoneManager::removed");

    PackageManager& packageManager = PackageManager::instance();
    packageManager.setEventCallback(packageEventHandler, this);

    zoneProcessMonitor();

    initializeCreatedZoneList();
    for (std::string& name : createdZoneList) {
        if (name == ZONE_DEFAULT_OWNER) {
            continue;
        }

        runtime::User zone(name);
        int noti = notification_register_detailed_changed_cb_for_uid(notiProxyCallback, &name, zone.getUid());
        notiProxyCallbackMap.insert(std::make_pair(name, noti));
    }
}

ZoneManager::~ZoneManager()
{
    PackageManager& packageManager = PackageManager::instance();
    packageManager.unsetEventCallback();
}

int ZoneManager::createZone(const std::string& name, const std::string& manifest)
{
    auto provisioningWorker = [name, manifest, this]() {
        std::unique_ptr<xml::Document> manifestFile;

        try {
            //create zone user by gumd
            GumUser *guser = gum_user_create_sync(FALSE);
            g_object_set(G_OBJECT(guser), "username", name.c_str(),
                            "usertype", GUM_USERTYPE_SECURITY, NULL);
            gboolean ret = gum_user_add_sync(guser);
            g_object_unref(guser);

            if (!ret) {
                throw runtime::Exception("Failed to remove user (" + name + ") by gumd");
            }

            runtime::User user(name);

            maskUserServices(user);

            manifestFile.reset(xml::Parser::parseString(manifest));
            mode_t omask = ::umask(0077);
            manifestFile->write(ZONE_MANIFEST_DIR + name + ".xml", "UTF-8", true);
            ::umask(omask);

            //TODO: write container owner info

            //unlock the user
            setZoneState(user.getUid(), 1);

            //wait for launchpad in the zone
            sleep(1);

            auto it = createdZoneList.insert(createdZoneList.begin(), name);
            int noti = notification_register_detailed_changed_cb_for_uid(notiProxyCallback, &(*it), user.getUid());
            notiProxyCallbackMap.insert(std::make_pair(name, noti));
            context.notify("ZoneManager::created", name, std::string());
        } catch (runtime::Exception& e) {
            ERROR(e.what());
            context.notify("ZoneManager::removed", name, std::string());
        }

    };

    std::thread asyncWork(provisioningWorker);
    asyncWork.detach();

    return 0;
}

int ZoneManager::removeZone(const std::string& name)
{
    if (lockZone(name) != 0) {
        return -1;
    }

    try {
        runtime::File manifest(ZONE_MANIFEST_DIR + name + ".xml");
        manifest.remove();
    }  catch (runtime::Exception& e) {
        return -1;
    }

    auto remove = [name, this] {
        try {
            runtime::User user(name);

            //remove zone user
            GumUser *guser = gum_user_get_sync(user.getUid(), FALSE);
            gboolean ret = gum_user_delete_sync(guser, TRUE);
            g_object_unref(guser);

            if (!ret) {
                throw runtime::Exception("Failed to remove user (" + name + ") by gumd");
            }

            for (std::vector<std::string>::iterator it = createdZoneList.begin();
                 it != createdZoneList.end(); it++) {
                createdZoneList.erase(it);
            }
            notiProxyCallbackMap.erase(name);
            context.notify("ZoneManager::removed", name, std::string());
        } catch (runtime::Exception& e) {
            ERROR(e.what());
            return;
        }
    };

    std::thread asyncWork(remove);
    asyncWork.detach();

    return 0;
}

int ZoneManager::lockZone(const std::string& name)
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

int ZoneManager::unlockZone(const std::string& name)
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

int ZoneManager::getZoneState(const std::string& name)
{
    auto it = std::find(createdZoneList.begin(), createdZoneList.end(), name);
    if (it == createdZoneList.end()) {
        return 0;
    }

    try {
        runtime::User user(name);
        dbus::Connection& systemDBus = dbus::Connection::getSystem();
        const dbus::Variant& var = systemDBus.methodcall
                                           ("org.freedesktop.login1",
                                            "/org/freedesktop/login1",
                                            "org.freedesktop.login1.Manager",
                                            "GetUser",
                                            -1, "(o)", "(u)", user.getUid());
        if (var) {
            return ZoneManager::State::Running;
        } else {
            return ZoneManager::State::Locked;
        }
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return 0;
    }

    return 0;
}

std::vector<std::string> ZoneManager::getZoneList(int state)
{
    std::vector<std::string> list;

    for (const std::string& name : createdZoneList) {
        if (getZoneState(name) & state) {
            list.push_back(name);
        }
    }
    return list;
}

int ZoneManager::resetZonePassword(const std::string& name, const std::string& newPassword)
{
    try {
        runtime::User user(name);
        int ret = auth_passwd_reset_passwd(AUTH_PWD_NORMAL, user.getUid(), newPassword.c_str());
        if (ret != AUTH_PASSWD_API_SUCCESS) {
            throw runtime::Exception("Failed to reset password for " + name);
        }
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

ZoneManager zoneManager(Server::instance());

} // namespace DevicePolicyManager

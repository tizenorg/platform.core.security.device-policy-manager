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

#include "user.hxx"
#include "smack.hxx"
#include "process.hxx"
#include "zone.hxx"
#include "filesystem.hxx"
#include "xml/parser.hxx"
#include "xml/document.hxx"
#include "audit/logger.hxx"

#define ZONE_UID_MIN       60001
#define ZONE_UID_MAX       65000

#define ZONE_GROUP         "zones"

#define ZONE_MANIFEST_DIR   CONF_PATH "/zone/"

#define FREEDESKTOP_LOGIN_INTERFACE \
    "org.freedesktop.login1",   \
    "/org/freedesktop/login1",  \
    "org.freedesktop.login1.Manager"

#define HOME_SMACKLABEL     "User::Home"
#define SHARED_SMACKLABEL   "User::App::Shared"

#define TEMPORARY_UMASK(mode)   \
        std::unique_ptr<mode_t, void(*)(mode_t *)> umask_##mode(new mode_t, \
        [](mode_t *prev) {umask(*prev);}); \
        *umask_##mode = mode;

namespace DevicePolicyManager {

static int callLogind(const char* function, GVariant* param)
{
    GDBusConnection* connection;
    GError* error = NULL;
    GVariant* var;

    connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);

    if (connection == NULL) {
        ERROR(std::string("Failed to get system DBUS : ") +  error->message);
        g_error_free(error);
        return -1;
    }

    var = g_dbus_connection_call_sync(connection, FREEDESKTOP_LOGIN_INTERFACE,
                                      function, param,
                                      NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
                                      &error);

    if (var == NULL) {
        ERROR(std::string("Failed to call ") + function + " : " + error->message);
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


Zone::Zone(PolicyControlContext& ctx)
    : context(ctx)
{
    Ipc::Service& manager = context.getServiceManager();

    manager.registerParametricMethod(this, (int)(Zone::create)(std::string, std::string));
    manager.registerParametricMethod(this, (int)(Zone::remove)(std::string));
    manager.registerParametricMethod(this, (int)(Zone::lock)(std::string));
    manager.registerParametricMethod(this, (int)(Zone::unlock)(std::string));
    manager.registerNonparametricMethod(this, (std::vector<std::string>)(Zone::getList)());

    manager.registerParametricMethod(this, (int)(Zone::getState)(std::string));
}

Zone::~Zone()
{
}

int Zone::create(const std::string& name, const std::string& setupWizAppid)
{
    Ipc::Service& manager = context.getServiceManager();
    std::unique_ptr<Xml::Document> bundleXml;
    Xml::Node::NodeList nodes;
    std::string manifest_path;
    bundle* b;
    int ret;

    try {
        b = ::bundle_create();
        if (b == nullptr) {
            throw Runtime::Exception("Failed to create bundle: out of memory");
        }

        ::bundle_add_str(b, "id", name.c_str());

        ret = ::aul_launch_app_for_uid(setupWizAppid.c_str(), b, manager.getPeerUid());
        ::bundle_free(b);
        if (ret < 0) {
            throw Runtime::Exception("Failed to launch application: " + std::to_string(ret));
        }

        //TODO : Get a manifest from setup wizard

        Runtime::User user = Runtime::User::create
                            (name, ZONE_GROUP, ZONE_UID_MIN, ZONE_UID_MAX);

        TEMPORARY_UMASK(0022);

        // Create zone home's directories
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

        ::tzplatform_set_user(user.getUid());
        for (int i = 0; dirs[i].dir != TZ_SYS_HOME; i++) {
            Runtime::File dir(std::string(::tzplatform_getenv(dirs[i].dir)));
            dir.makeDirectory();
            dir.chown(user.getUid(), user.getGid());
            Runtime::Smack::setAccess(dir, dirs[i].smack);
            Runtime::Smack::setTransmute(dir, true);
        }
        ::tzplatform_reset_user();

        bundleXml = std::unique_ptr<Xml::Document>(Xml::Parser::parseFile(manifest_path));

        //TODO : Execute hooks of creation

        bundleXml->write(ZONE_MANIFEST_DIR + name + ".xml", "UTF-8", true);
    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int Zone::remove(const std::string& name)
{
    std::unique_ptr<Xml::Document> bundleXml;
    Xml::Node::NodeList nodes;
    std::string manifest_path;
    int ret;

    //lock the user
    ret = lock(name);
    if (ret != 0) {
        return -1;
    }

    try {
        manifest_path = ZONE_MANIFEST_DIR + name + ".xml";

        bundleXml = std::unique_ptr<Xml::Document>(Xml::Parser::parseFile(manifest_path));

        //TODO : Execute hooks of destroy

        Runtime::User user(name);
        user.remove();

        Runtime::File bundle(manifest_path);
        bundle.remove();
    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int Zone::lock(const std::string& name)
{
    int result;

    try {
        Runtime::User user(name);

        //disable user linger by using systemd
        result = callLogind("SetUserLinger",
                            g_variant_new("(ubb)", user.getUid(), 0, 1));
        if (result != 0) {
            return -1;
        }

    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int Zone::unlock(const std::string& name)
{
    int result;

    try {
        Runtime::User user(name);

        //enable user linger by using systemd
        result = callLogind("SetUserLinger",
                            g_variant_new("(ubb)", user.getUid(), 1, 1));
        if (result != 0) {
            return -1;
        }

    } catch (Runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

std::vector<std::string> Zone::getList()
{
    std::vector<std::string> list;
    try {
        Runtime::DirectoryIterator iter(Runtime::Path(ZONE_MANIFEST_DIR)), end;

        while (iter != end) {
            const std::string& path = (*iter).getPath();
            size_t name = path.rfind('/') + 1;
            size_t ext = path.rfind(".xml");
            list.push_back(path.substr(name, ext - name));
            ++iter;
        }
    } catch (Runtime::Exception& e) {}

    return list;
}

int Zone::getState(const std::string& name)
{
    //TODO : should implement

    return 0;
}

Zone zonePolicy(DevicePolicyServer::Server::instance());

} // namespace DevicePolicyManager

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

#include "zone.hxx"

#include "smack.h"
#include "process.h"
#include "filesystem.h"
#include "auth/user.h"
#include "xml/parser.h"
#include "xml/document.h"
#include "audit/logger.h"

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
                                      "SetLingerUser", param,
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


Zone::Zone(PolicyControlContext& ctx)
    : context(ctx)
{
    rmi::Service& manager = context.getServiceManager();

    manager.registerParametricMethod(this, (int)(Zone::createZone)(std::string, std::string));
    manager.registerParametricMethod(this, (int)(Zone::removeZone)(std::string));
    manager.registerParametricMethod(this, (int)(Zone::lockZone)(std::string));
    manager.registerParametricMethod(this, (int)(Zone::unlockZone)(std::string));
    manager.registerNonparametricMethod(this, (std::vector<std::string>)(Zone::getZoneList)());

    manager.registerParametricMethod(this, (int)(Zone::getZoneState)(std::string));
}

Zone::~Zone()
{
}

int Zone::createZone(const std::string& name, const std::string& setupWizAppid)
{
    return -1;
}

int Zone::removeZone(const std::string& name)
{
    return -1;
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
    return std::vector<std::string>();
}

int Zone::getZoneState(const std::string& name)
{
    return 0;
}

Zone zonePolicy(Server::instance());

} // namespace DevicePolicyManager

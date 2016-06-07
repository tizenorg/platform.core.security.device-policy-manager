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
#include <arpa/inet.h>

#include <cstdlib>
#include <functional>
#include <unordered_set>

#include <glib.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <wifi.h>

#include "wifi.hxx"

#include "policy-helper.h"

#include "app-bundle.h"
#include "syspopup.h"
#include "audit/logger.h"
#include "dbus/connection.h"

#define NETCONFIG_INTERFACE		\
	"net.netconfig",			\
	"/net/netconfig/network",	\
	"net.netconfig.network"

namespace DevicePolicyManager {

static std::unordered_set<std::string> blockSsidList;
static inline void applyBlocklist(wifi_ap_h ap)
{
    char *ssid;
    ::wifi_ap_get_essid(ap, &ssid);
    if (blockSsidList.find(ssid) != blockSsidList.end()) {
        ::wifi_forget_ap(ap);
    }

    ::free(ssid);
}
static std::atomic<bool> blockEnabled;
static void connectionStateChanged(wifi_connection_state_e state,
                                   wifi_ap_h ap, void *user_data)
{
    WifiPolicy* policy = reinterpret_cast<WifiPolicy*>(user_data);

    if (state == WIFI_CONNECTION_STATE_FAILURE ||
        state == WIFI_CONNECTION_STATE_DISCONNECTED) {
        return;
    }

    if (!policy->isNetworkAccessRestricted())
        return;

    applyBlocklist(ap);
}

static inline void applyBlocklistToConnectedAP() {
    wifi_ap_h ap;

    ::wifi_initialize();
    if (::wifi_get_connected_ap(&ap) == WIFI_ERROR_NONE) {
        applyBlocklist(ap);
        ::wifi_ap_destroy(ap);
    }
}

WifiPolicy::WifiPolicy(PolicyControlContext& ctx) :
    context(ctx)
{
    context.registerParametricMethod(this, (int)(WifiPolicy::setProfileChangeRestriction)(bool));
    context.registerNonparametricMethod(this, (bool)(WifiPolicy::isProfileChangeRestricted));

    context.registerParametricMethod(this, (int)(WifiPolicy::setNetworkAccessRestriction)(bool));
    context.registerNonparametricMethod(this, (bool)(WifiPolicy::isNetworkAccessRestricted));
    context.registerParametricMethod(this, (int)(WifiPolicy::addSsidToBlocklist)(std::string));
    context.registerParametricMethod(this, (int)(WifiPolicy::removeSsidFromBlocklist)(std::string));

    context.createNotification("wifi-profile-change");
    context.createNotification("wifi-ssid-restriction");

    ::wifi_initialize();
    ::wifi_set_connection_state_changed_cb(connectionStateChanged, this);
}

WifiPolicy::~WifiPolicy()
{
    ::wifi_unset_connection_state_changed_cb();
}

int WifiPolicy::setProfileChangeRestriction(bool enable)
{
    int ret;

    dbus::Connection &systemDBus = dbus::Connection::getSystem();
    systemDBus.methodcall(NETCONFIG_INTERFACE,
                          "DevicePolicySetWifiProfile",
                          -1,
                          "(i)",
                          "(i)",
                          enable).get("(i)", &ret);
    if (ret != 0) {
        return -1;
    }

    SetPolicyAllowed(context, "wifi-profile-change", enable);
    return 0;
}

bool WifiPolicy::isProfileChangeRestricted(void)
{
    return IsPolicyAllowed(context, "wifi-profile-change");
}

int WifiPolicy::setNetworkAccessRestriction(bool enable)
{
    SetPolicyEnabled(context, "wifi-ssid-restriction", enable);

    if (enable) {
        applyBlocklistToConnectedAP();
    }

    return 0;
}

bool WifiPolicy::isNetworkAccessRestricted(void)
{
    return IsPolicyEnabled(context, "wifi-ssid-restriction");
}

int WifiPolicy::addSsidToBlocklist(const std::string& ssid)
{
    try {
        blockSsidList.insert(ssid);
        if (IsPolicyEnabled(context, "wifi-ssid-restriction")) {
            applyBlocklistToConnectedAP();
        }
    } catch (runtime::Exception& e) {
        ERROR("Failed to allocate memory for wifi blocklist");
        return -1;
    }

    return 0;
}

int WifiPolicy::removeSsidFromBlocklist(const std::string& ssid)
{
    std::unordered_set<std::string>::iterator it = blockSsidList.find(ssid);

    if (it == blockSsidList.end())
        return -1;

    blockSsidList.erase(it);
    return 0;
}


WifiPolicy wifiPolicy(Server::instance());

} // namespace DevicePolicyManager

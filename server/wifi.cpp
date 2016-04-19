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

#include <wifi.h>

#include <cstdlib>
#include <iostream>
#include <functional>
#include <unordered_set>

#include "wifi.hxx"
#include "app-bundle.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

namespace {
    bool settingChangesRestriction;
    bool stateChangeRestriction;
    bool hotspotRestriction;
    bool networkAccessRestriction;
    std::unordered_set<std::string> blockSsidList;
};

static void connectionStateChanged(wifi_connection_state_e state,
                                   wifi_ap_h ap, void *user_data)
{
    char *ssid;

    if (state == WIFI_CONNECTION_STATE_FAILURE ||
        state == WIFI_CONNECTION_STATE_DISCONNECTED) {
        return;
    }

    ::wifi_ap_get_essid(ap, &ssid);
    if (blockSsidList.find(ssid) != blockSsidList.end()) {
        ::wifi_forget_ap(ap);
    }
    ::free(ssid);
}

Wifi::Wifi(PolicyControlContext& ctx) :
    context(ctx)
{
    context.registerParametricMethod(this, (int)(Wifi::setStateChangeRestriction)(bool));
    context.registerNonparametricMethod(this, (bool)(Wifi::isStateChangeRestricted));

    context.registerParametricMethod(this, (int)(Wifi::setSettingChangesRestriction)(bool));
    context.registerNonparametricMethod(this, (bool)(Wifi::isSettingChangesRestricted));

    context.registerParametricMethod(this, (int)(Wifi::setHotspotRestriction)(bool));
    context.registerNonparametricMethod(this, (bool)(Wifi::isHotspotRestricted));

    context.registerParametricMethod(this, (int)(Wifi::setNetworkAccessRestriction)(bool));
    context.registerNonparametricMethod(this, (bool)(Wifi::isNetworkAccessRestricted));

    context.registerParametricMethod(this, (int)(Wifi::addSsidFromBlocklist)(std::string));
    context.registerParametricMethod(this, (int)(Wifi::removeSsidFromBlocklist)(std::string));

    context.createNotification("Wifi::stateChange");
    context.createNotification("Wifi::settingChange");
    context.createNotification("Wifi::hotspot");
    context.createNotification("Wifi::networkAccess");
}

Wifi::~Wifi()
{
}

int Wifi::setStateChangeRestriction(bool restrict)
{
    if (restrict == stateChangeRestriction)
        return 0;

    stateChangeRestriction = restrict;

    context.notify("Wifi::stateChange", (restrict)? "Restriced":"Allowed");
    return 0;
}

bool Wifi::isStateChangeRestricted()
{
    return stateChangeRestriction;
}

int Wifi::setSettingChangesRestriction(bool restrict)
{
    if (restrict == settingChangesRestriction)
        return 0;

    settingChangesRestriction = restrict;

    context.notify("Wifi::settingChange", (restrict)? "Restriced":"Allowed");
    return 0;
}

bool Wifi::isSettingChangesRestricted()
{
    return settingChangesRestriction;
}

int Wifi::setHotspotRestriction(bool restrict)
{
    if (restrict == hotspotRestriction)
        return 0;

    hotspotRestriction = restrict;

    context.notify("Wifi::hotspot", (restrict)? "Restriced":"Allowed");
    return 0;
}

bool Wifi::isHotspotRestricted()
{
    return hotspotRestriction;
}

int Wifi::setNetworkAccessRestriction(bool restrict)
{
    if (restrict == networkAccessRestriction)
        return 0;

    if (restrict) {
        ::wifi_set_connection_state_changed_cb(connectionStateChanged, NULL);
    } else {
        ::wifi_unset_connection_state_changed_cb();
    }

    networkAccessRestriction = restrict;

    context.notify("Wifi::networkAccess", (restrict)? "Restriced":"Allowed");
    return 0;
}

bool Wifi::isNetworkAccessRestricted()
{
    return networkAccessRestriction;
}

int Wifi::addSsidFromBlocklist(const std::string& ssid)
{
    try {
        blockSsidList.insert(ssid);
    } catch (runtime::Exception& e) {
        ERROR("Failed to allocate memory for wifi blocklist");
        return -1;
    }
    return 0;
}

int Wifi::removeSsidFromBlocklist(const std::string& ssid)
{
    blockSsidList.clear();
    return 0;
}

Wifi wifiPolicy(Server::instance());

} // namespace DevicePolicyManager

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

#include <vconf.h>
#include <vconf-keys.h>
#include <wifi.h>
#include <syspopup_caller.h>
#include <network-pm-config.h>
#include <network-pm-intf.h>
#include <network-pm-wlan.h>
#include <network-cm-intf.h>

#include <cstdlib>
#include <iostream>
#include <functional>

#include "wifi.hxx"
#include "app-bundle.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

typedef wifi_ap_h AccessPointHandle;

namespace {

struct AccessPoint {
    AccessPoint(AccessPointHandle h, const std::string& name) :
        handle(h), ssid(name)
    {
    }

    AccessPointHandle handle;
    std::string ssid;
};

const std::string WIFI_PKG = "wifi-qs";

AccessPointHandle FindAccessPoint(const std::string& ssid)
{
    auto AccessPointFoundCallback = [](wifi_ap_h handle, void* user_data) {
        char *name;
        int ret = ::wifi_ap_get_essid(handle, &name);
        if (ret == WIFI_ERROR_NONE) {
            AccessPoint *ap = static_cast<AccessPoint*>(user_data);
            if (ap->ssid == name) {
                ap->handle = handle;
                ::free(name);
                // should return false to break iteration
                return true;
            }
            ::free(name);
        }
        // should return true to continue iteration
        return true;
    };

    AccessPoint ap(nullptr, ssid);
    int ret = ::wifi_foreach_found_aps(AccessPointFoundCallback, &ap);
    if (ret != WIFI_ERROR_NONE) {
        ERROR("Error in wifi_foreach_found_aps");
        return nullptr;
    }

    return ap.handle;
}

std::string WifiErrorMessage(const int error)
{
    switch (error) {
    case WIFI_ERROR_INVALID_PARAMETER:
        return "WIFI_ERROR_INVALID_PARAMETER";
    case WIFI_ERROR_OUT_OF_MEMORY:
        return "WIFI_ERROR_OUT_OF_MEMORY";
    case WIFI_ERROR_INVALID_OPERATION:
        return "WIFI_ERROR_INVALID_OPERATION";
    case WIFI_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED:
        return "WIFI_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED";
    case WIFI_ERROR_OPERATION_FAILED:
        return "WIFI_ERROR_OPERATION_FAILED";
    case WIFI_ERROR_NO_CONNECTION:
        return "WIFI_ERROR_NO_CONNECTION";
    case WIFI_ERROR_NOW_IN_PROGRESS:
        return "WIFI_ERROR_NOW_IN_PROGRESS";
    case WIFI_ERROR_ALREADY_EXISTS:
        return "WIFI_ERROR_ALREADY_EXISTS";
    case WIFI_ERROR_OPERATION_ABORTED:
        return "WIFI_ERROR_OPERATION_ABORTED";
    case WIFI_ERROR_DHCP_FAILED:
        return "WIFI_ERROR_DHCP_FAILED";
    case WIFI_ERROR_INVALID_KEY:
        return "WIFI_ERROR_INVALID_KEY";
    case WIFI_ERROR_NO_REPLY:
        return "WIFI_ERROR_NO_REPLY";
    case WIFI_ERROR_SECURITY_RESTRICTED:
        return "WIFI_ERROR_SECURITY_RESTRICTED";
    default:
        break;
    }

    return "Unknown WIFI Error";
}

} //namespace

Wifi::Wifi(PolicyControlContext& ctx) :
    context(ctx)
{
    rmi::Service& manager = context.getServiceManager();

    manager.registerParametricMethod(this, (int)(Wifi::setsetStateChangeRestriction)(bool));
    manager.registerParametricMethod(this, (int)(Wifi::setSettingChangesRestriction)(bool));
    manager.registerParametricMethod(this, (int)(Wifi::setApSettingModificationRestriction)(bool));
    manager.registerParametricMethod(this, (int)(Wifi::activateWifiSsidRestriction)(bool));
    manager.registerParametricMethod(this, (int)(Wifi::addSsidFromBlacklist)(std::string));
    manager.registerParametricMethod(this, (int)(Wifi::removeSsidFromBlacklist)(std::string));

    manager.registerNonparametricMethod(this, (bool)(Wifi::isWifiStateChanageAllowed)());
    manager.registerNonparametricMethod(this, (bool)(Wifi::isSettingChangesRestricted)());
    manager.registerNonparametricMethod(this, (bool)(Wifi::isApSettingModificationRestricted)());
    manager.registerNonparametricMethod(this, (bool)(Wifi::isNetworkAccessRestricted)());
}

Wifi::~Wifi()
{
}

int Wifi::setStateChangeRestriction(bool allow)
{
    int status;

    if (::vconf_get_int(VCONFKEY_NETWORK_WIFI_STATE, &status) != 0) {
        ERROR("Failed to read VCONFKEY_NETWORK_WIFI_STATE");
        return -1;
    }

    if (status != VCONFKEY_NETWORK_WIFI_OFF) {
        try {
            Bundle bundle;
            bundle.add("-t", "off");

            Syspopup syspopup(WIFI_PKG);
            if (syspopup.launch(bundle) < 0) {
                ERROR("Failed to lunch wifi syspopup");
                return -1;
            }
        } catch (runtime::Exception& e) {
            ERROR("Failed to lunch wifi syspopup");
            return -1;
        }
    }

    return 0;
}

bool Wifi::isWifiStateChanegAllowed()
{
    return false;
}

int Wifi::setSettingChangesRestriction(bool restrict)
{
        return -1;
}

bool Wifi::isSettingChangesRestricted()
{
    return false;
}

int Wifi::setApSettingModificationRestriction(bool restrict)
{
    return -1;
}

bool Wifi::isApSettingModificationRestricted()
{
    return false;
}

int Wifi::setNetworkAccessRestriction(bool restrict)
{
    return -1;
}

bool Wifi::isNetworkAccessRestricted()
{
    return false;
}

int Wifi::addSsidFromBlacklist(const std::string& ssid)
{
    return -1;
}

int Wifi::removeSsidFromBlacklist(const std::string& ssid)
{
    return -1;
}

Wifi wifiPolicy(Server::instance());

} // namespace DevicePolicyManager

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
#include <iostream>
#include <functional>

#include <vconf.h>
#include <vconf-keys.h>
#include <wifi.h>

#include "wifi.hxx"

#include "policy-helper.h"

#include "app-bundle.h"
#include "syspopup.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

WifiPolicy::WifiPolicy(PolicyControlContext& ctx) :
    context(ctx)
{
    context.registerParametricMethod(this, (int)(WifiPolicy::allowSettingsChange)(bool));
    context.registerNonparametricMethod(this, (bool)(WifiPolicy::isSettingsChangeAllowed));

    context.registerParametricMethod(this, (int)(WifiPolicy::setNetworkAccessRestriction)(bool));
    context.registerNonparametricMethod(this, (bool)(WifiPolicy::isNetworkAccessRestricted));
    context.registerParametricMethod(this, (int)(WifiPolicy::addSsidToBlocklist)(std::string));
    context.registerParametricMethod(this, (int)(WifiPolicy::removeSsidFromBlocklist)(std::string));

    context.createNotification("wifi-setting-changes");
    context.createNotification("wifi-ssid-restriction");
}

WifiPolicy::~WifiPolicy()
{
}

int WifiPolicy::allowSettingsChange(bool enable)
{
    SetPolicyAllowed(context, "wifi-setting-changes", enable);
    return 0;
}

bool WifiPolicy::isSettingsChangeAllowed(void)
{
    return IsPolicyAllowed(context, "wifi-setting-changes");
}

int WifiPolicy::setNetworkAccessRestriction(bool enable)
{
    SetPolicyEnabled(context, "wifi-ssid-restriction", enable);
    return 0;
}

bool WifiPolicy::isNetworkAccessRestricted(void)
{
    return IsPolicyEnabled(context, "wifi-ssid-restriction");
}

int WifiPolicy::addSsidToBlocklist(const std::string& ssid)
{
    return -1;
}

int WifiPolicy::removeSsidFromBlocklist(const std::string& ssid)
{
    return -1;
}


WifiPolicy wifiPolicy(Server::instance());

} // namespace DevicePolicyManager

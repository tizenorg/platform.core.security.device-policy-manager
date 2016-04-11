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
//#include <wifi.h>

#include <gio/gio.h>

#include "wifi.hxx"

namespace DevicePolicyManager {

Wifi::Wifi(PolicyControlContext& ctx)
    : context(ctx)
{
    rmi::Service& manager = context.getServiceManager();

    manager.registerNonparametricMethod(this, (bool)(Wifi::isAllowed)());
    manager.registerParametricMethod(this, (int)(Wifi::disallow)(bool));

    manager.registerNonparametricMethod(this, (bool)(Wifi::isSettingChangeAllowed)());
    manager.registerParametricMethod(this, (int)(Wifi::disallowSettingChange)(bool));

    manager.registerNonparametricMethod(this, (bool)(Wifi::isBlocked)());
    manager.registerNonparametricMethod(this, (std::vector<std::string>)(Wifi::getBlocked)());
    manager.registerParametricMethod(this, (int)(Wifi::addBlocked)(std::string));
    manager.registerNonparametricMethod(this, (int)(Wifi::clearBlocked)());

    manager.registerNonparametricMethod(this, (std::vector<std::string>)(Wifi::getBlacklist)());
    manager.registerParametricMethod(this, (int)(Wifi::addBlacklist)(std::string));
    manager.registerNonparametricMethod(this, (int)(Wifi::clearBlacklist)());
}

Wifi::~Wifi()
{
}


bool Wifi::isAllowed()
{
    return true;
}

int Wifi::disallow(bool disallow)
{
    return -1;
}


bool Wifi::isSettingChangeAllowed()
{
    return true;
}

int Wifi::disallowSettingChange(bool disallow)
{
    return -1;
}


bool Wifi::isBlocked()
{
    return false;
}

std::vector<std::string> Wifi::getBlocked()
{
    return std::vector<std::string>();
}

int Wifi::addBlocked(std::string ssid)
{
    return -1;
}

int Wifi::clearBlocked()
{
    return -1;
}


std::vector<std::string> Wifi::getBlacklist()
{
    return std::vector<std::string>();
}

int Wifi::addBlacklist(std::string ssid)
{
    return -1;
}

int Wifi::clearBlacklist()
{
    return -1;
}


Wifi wifiPolicy(Server::instance());

} // namespace DevicePolicyManager

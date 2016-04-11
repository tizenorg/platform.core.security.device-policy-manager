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

#include "wifi.hxx"

namespace DevicePolicyManager {

Wifi::Wifi(PolicyControlContext& ctx)
    : context(ctx)
{
}

Wifi::~Wifi()
{
}

bool Wifi::isAllowed()
{
    try {
        return context->methodCall<bool>("Wifi::isAllowed");
    } catch (runtime::Exception& e) {
        return true;
    }
}

int Wifi::disallow(bool disallow)
{
    try {
        return context->methodCall<int>("Wifi::disallow", disallow);
    } catch (runtime::Exception& e) {
        return -1;
    }
}


bool Wifi::isSettingChangeAllowed()
{
    try {
        return context->methodCall<bool>("Wifi::isSettingChangeAllowed");
    } catch (runtime::Exception& e) {
        return true;
    }
}

int Wifi::disallowSettingChange(bool disallow)
{
    try {
        return context->methodCall<int>("Wifi::disallowSettingChange", disallow);
    } catch (runtime::Exception& e) {
        return -1;
    }
}


bool Wifi::isBlocked()
{
    try {
        return context->methodCall<bool>("Wifi::isBlocked");
    } catch (runtime::Exception& e) {
        return false;
    }
}

std::vector<std::string> Wifi::getBlocked()
{
    std::vector<std::string> empty;
    try {
        return context->methodCall<std::vector<std::string>>("Wifi::getBlocked");
    } catch (runtime::Exception& e) {
        return empty;
    }
}

int Wifi::addBlocked(std::string ssid)
{
    try {
        return context->methodCall<int>("Wifi::addBlocked", ssid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Wifi::clearBlocked()
{
    try {
        return context->methodCall<int>("Wifi::clearBlocked");
    } catch (runtime::Exception& e) {
        return -1;
    }
}


std::vector<std::string> Wifi::getBlacklist()
{
    std::vector<std::string> empty;
    try {
        return context->methodCall<std::vector<std::string>>("Wifi::getBlacklist");
    } catch (runtime::Exception& e) {
        return empty;
    }
}

int Wifi::addBlacklist(std::string ssid)
{
    try {
        return context->methodCall<int>("Wifi::addBlacklist", ssid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Wifi::clearBlacklist()
{
    try {
        return context->methodCall<int>("Wifi::clearBlacklist");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

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
#include "audit/logger.h"

namespace DevicePolicyManager {

Wifi::Wifi(PolicyControlContext& ctxt) :
    context(ctxt)
{
}

Wifi::~Wifi()
{
}

int Wifi::setStateChangeRestriction(bool enable)
{
    try {
        return context->methodCall<int>("Wifi::setStateChangeRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Wifi::isStateChangeRestricted()
{
    try {
        return context->methodCall<bool>("Wifi::isStateChangeRestricted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

int Wifi::setSettingChangesRestriction(bool enable)
{
    try {
        return context->methodCall<int>("Wifi::setSettingChangesRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Wifi::isSettingChangesRestricted()
{
    try {
        return context->methodCall<bool>("Wifi::isSettingChangesRestricted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

int Wifi::setHotspotRestriction(bool enable)
{
    try {
        return context->methodCall<int>("Wifi::setHotspotRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Wifi::isHotspotRestricted()
{
    try {
        return context->methodCall<bool>("Wifi::isHotspotRestricted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

int Wifi::setNetworkAccessRestriction(bool enable)
{
    try {
        return context->methodCall<int>("Wifi::setNetworkAccessRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Wifi::isNetworkAccessRestricted()
{
    try {
        return context->methodCall<bool>("Wifi::isNetworkAccessRestricted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

int Wifi::addSsidFromBlocklist(const std::string& ssid)
{
    try {
        return context->methodCall<int>("Wifi::addSsidFromBlacklist", ssid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Wifi::removeSsidFromBlocklist(const std::string& ssid)
{
    try {
        return context->methodCall<int>("Wifi::removeSsidFromBlacklist", ssid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

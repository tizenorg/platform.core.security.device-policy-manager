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

WifiPolicy::WifiPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
}

WifiPolicy::~WifiPolicy()
{
}

int WifiPolicy::setState(bool enable)
{
    try {
        return context->methodCall<int>("WifiPolicy::setState", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool WifiPolicy::getState()
{
    try {
        return context->methodCall<bool>("WifiPolicy::getState");
    } catch (runtime::Exception &e) {
        return -1;
    }
}

int WifiPolicy::setHotspotState(bool enable)
{
    try {
        return context->methodCall<int>("WifiPolicy::setHotspotState", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool WifiPolicy::getHotspotState()
{
    try {
        return context->methodCall<bool>("WifiPolicy::getHotspotState");
    } catch (runtime::Exception &e) {
        return -1;
    }
}

int WifiPolicy::setProfileChangeRestriction(bool enable)
{
    try {
        return context->methodCall<int>("WifiPolicy::setProfileChangeRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool WifiPolicy::isProfileChangeRestricted(void)
{
    try {
        return context->methodCall<bool>("WifiPolicy::isProfileChangeRestricted");
    } catch (runtime::Exception &e) {
        return -1;
    }
}

int WifiPolicy::setNetworkAccessRestriction(bool enable)
{
    try {
        return context->methodCall<int>("WifiPolicy::setNetworkAccessRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool WifiPolicy::isNetworkAccessRestricted(void)
{
    try {
        return context->methodCall<bool>("WifiPolicy::isNetworkAccessRestricted");
    } catch (runtime::Exception &e) {
        return -1;
    }
}

int WifiPolicy::addSsidToBlocklist(const std::string& ssid)
{
    try {
        return context->methodCall<int>("WifiPolicy::addSsidToBlocklist", ssid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int WifiPolicy::removeSsidFromBlocklist(const std::string& ssid)
{
    try {
        return context->methodCall<int>("WifiPolicy::removeSsidFromBlocklist", ssid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

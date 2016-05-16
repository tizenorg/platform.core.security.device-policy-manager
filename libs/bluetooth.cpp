/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "bluetooth.hxx"
#include "audit/logger.h"

namespace DevicePolicyManager {

BluetoothPolicy::BluetoothPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
}

BluetoothPolicy::~BluetoothPolicy()
{
}

// for restriction CPIs
int BluetoothPolicy::setModeChangeState(const bool enable)
{
    try {
        return context->methodCall<int>("BluetoothPolicy::setModeChangeState", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool BluetoothPolicy::getModeChangeState()
{
    try {
        return context->methodCall<bool>("BluetoothPolicy::getModeChangeState");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int BluetoothPolicy::setDesktopConnectivityState(const bool enable)
{
    try {
        return context->methodCall<bool>("BluetoothPolicy::setDesktopConnectivityState");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool BluetoothPolicy::getDesktopConnectivityState()
{
    try {
        return context->methodCall<bool>("BluetoothPolicy::getDesktopConnectivityState");
    } catch (runtime::Exception& e) {
        return -1;
    }
}


// for bluetooth CAPIs
int BluetoothPolicy::addDeviceToBlacklist(const std::string& mac)
{
    try {
        return context->methodCall<int>("BluetoothPolicy::addDeviceToBlacklist", mac);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int BluetoothPolicy::removeDeviceFromBlacklist(const std::string& mac)
{
    try {
        return context->methodCall<int>("BluetoothPolicy::removeDeviceFromBlacklist", mac);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int BluetoothPolicy::setDeviceRestriction(const bool enable)
{
    try {
        return context->methodCall<int>("BluetoothPolicy::setDeviceRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool BluetoothPolicy::isDeviceRestricted()
{
    try {
        return context->methodCall<bool>("BluetoothPolicy::isDeviceRestricted");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int BluetoothPolicy::addUuidToBlacklist(const std::string& uuid)
{
    try {
        return context->methodCall<int>("BluetoothPolicy::addUuidToBlacklist", uuid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int BluetoothPolicy::removeUuidFromBlacklist(const std::string& uuid)
{
    try {
        return context->methodCall<int>("BluetoothPolicy::removeUuidFromBlacklist", uuid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int BluetoothPolicy::setUuidRestriction(const bool enable)
{
    try {
        return context->methodCall<int>("BluetoothPolicy::setUuidRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool BluetoothPolicy::isUuidRestricted()
{
    try {
        return context->methodCall<bool>("BluetoothPolicy::isUuidRestricted");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

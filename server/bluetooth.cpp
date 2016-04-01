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
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::addDeviceToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::removeDeviceFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setDeviceRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::isDeviceRestricted));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::addUuidToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::removeUuidFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setUuidRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::isUuidRestricted));
}

BluetoothPolicy::~BluetoothPolicy()
{
}

int BluetoothPolicy::addDeviceToBlacklist(const std::string& mac_address)
{
    return 0;
}

int BluetoothPolicy::removeDeviceFromBlacklist(const std::string& mac_address)
{
    return 0;
}

int BluetoothPolicy::setDeviceRestriction(const bool enable)
{
    return 0;
}

bool BluetoothPolicy::isDeviceRestricted()
{
    return false;
}

int BluetoothPolicy::addUuidToBlacklist(const std::string& uuid)
{
    return 0;
}

int BluetoothPolicy::removeUuidFromBlacklist(const std::string& uuid)
{
    return 0;
}

int BluetoothPolicy::setUuidRestriction(const bool enable)
{
    return 0;
}

bool BluetoothPolicy::isUuidRestricted()
{
    return false;
}

BluetoothPolicy bluetoothPolicy(Server::instance());

} // namespace DevicePolicyManager

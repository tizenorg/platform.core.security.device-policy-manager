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

Bluetooth::Bluetooth(PolicyControlContext& ctxt) :
    context(ctxt)
{
    ctxt.registerParametricMethod(this, (int)(Bluetooth::addDeviceToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(Bluetooth::removeDeviceFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(Bluetooth::setDeviceRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(Bluetooth::isDeviceRestricted));
    ctxt.registerParametricMethod(this, (int)(Bluetooth::addUuidToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(Bluetooth::removeUuidFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(Bluetooth::setUuidRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(Bluetooth::isUuidRestricted));
}

Bluetooth::~Bluetooth()
{
}

int Bluetooth::addDeviceToBlacklist(const std::string& mac_address)
{
    return 0;
}

int Bluetooth::removeDeviceFromBlacklist(const std::string& mac_address)
{
    return 0;
}

int Bluetooth::setDeviceRestriction(const bool enable)
{
    return 0;
}

bool Bluetooth::isDeviceRestricted()
{
    return true;
}

int Bluetooth::addUuidToBlacklist(const std::string& uuid)
{
    return 0;
}

int Bluetooth::removeUuidFromBlacklist(const std::string& uuid)
{
    return 0;
}

int Bluetooth::setUuidRestriction(const bool enable)
{
    return 0;
}

bool Bluetooth::isUuidRestricted()
{
    return true;
}

Bluetooth bluetoothPolicy(Server::instance());

} // namespace DevicePolicyManager

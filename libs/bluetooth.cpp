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
}

Bluetooth::~Bluetooth()
{
}

int Bluetooth::addDeviceToBlacklist(const std::string& mac_address)
{
    try {
        return context->methodCall<int>("Bluetooth::addDeviceToBlacklist", mac_address);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Bluetooth::removeDeviceFromBlacklist(const std::string& mac_address)
{
    try {
        return context->methodCall<int>("Bluetooth::removeDeviceFromBlacklist", mac_address);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Bluetooth::setDeviceRestriction(const bool enable)
{
    try {
        return context->methodCall<int>("Bluetooth::setDeviceRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Bluetooth::isDeviceRestricted()
{
    try {
        return context->methodCall<bool>("Bluetooth::isDeviceRestricted");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Bluetooth::addUuidToBlacklist(const std::string& uuid)
{
    try {
        return context->methodCall<int>("Bluetooth::addUuidToBlacklist", uuid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Bluetooth::removeUuidFromBlacklist(const std::string& uuid)
{
    try {
        return context->methodCall<int>("Bluetooth::removeUuidFromBlacklist", uuid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Bluetooth::setUuidRestriction(const bool enable)
{
    try {
        return context->methodCall<int>("Bluetooth::setUuidRestriction", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Bluetooth::isUuidRestricted()
{
    try {
        return context->methodCall<bool>("Bluetooth::isUuidRestricted");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

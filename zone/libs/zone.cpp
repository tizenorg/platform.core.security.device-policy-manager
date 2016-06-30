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

#include "zone/zone.hxx"

namespace DevicePolicyManager {

ZoneManager::ZoneManager(PolicyControlContext& ctx)
    : context(ctx)
{
}

ZoneManager::~ZoneManager()
{
}

int ZoneManager::createZone(const std::string& name, const std::string& setupWizAppid)
{
    try {
        return context->methodCall<int>("ZoneManager::createZone", name, setupWizAppid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneManager::removeZone(const std::string& name)
{
    try {
        return context->methodCall<int>("ZoneManager::removeZone", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneManager::lockZone(const std::string& name)
{
    try {
        return context->methodCall<int>("ZoneManager::lockZone", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneManager::unlockZone(const std::string& name)
{
    try {
        return context->methodCall<int>("ZoneManager::unlockZone", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneManager::getZoneState(const std::string& name)
{
    try {
        return context->methodCall<int>("ZoneManager::getZoneState", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> ZoneManager::getZoneList(int state)
{
    std::vector<std::string> empty;
    try {
        return context->methodCall<std::vector<std::string>>("ZoneManager::getZoneList", state);
    } catch (runtime::Exception& e) {
        return empty;
    }
}

int ZoneManager::resetZonePassword(const std::string& name, const std::string& newPassword)
{
    try {
        return context->methodCall<int>("ZoneManager::resetZonePassword", name, newPassword);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

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

#include "zone.hxx"

namespace DevicePolicyManager {

ZonePolicy::ZonePolicy(PolicyControlContext& ctx)
    : context(ctx)
{
}

ZonePolicy::~ZonePolicy()
{
}

int ZonePolicy::createZone(const std::string& name, const std::string& setupWizAppid)
{
    try {
        return context->methodCall<int>("ZonePolicy::createZone", name, setupWizAppid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePolicy::removeZone(const std::string& name)
{
    try {
        return context->methodCall<int>("ZonePolicy::removeZone", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePolicy::lockZone(const std::string& name)
{
    try {
        return context->methodCall<int>("ZonePolicy::lockZone", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePolicy::unlockZone(const std::string& name)
{
    try {
        return context->methodCall<int>("ZonePolicy::unlockZone", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> ZonePolicy::getZoneList()
{
    std::vector<std::string> empty;
    try {
        return  context->methodCall<std::vector<std::string>>("ZonePolicy::getZoneList");
    } catch (runtime::Exception& e) {
        return empty;
    }
}

int ZonePolicy::getZoneState(const std::string& name)
{
    try {
        return context->methodCall<int>("ZonePolicy::getZoneState", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

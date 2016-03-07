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

Zone::Zone(PolicyControlContext& ctx)
    : context(ctx)
{
}

Zone::~Zone()
{
}

int Zone::create(const std::string& name, const std::string& setupWizAppid)
{
    try {
        return context->methodCall<int>("Zone::create", name, setupWizAppid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Zone::remove(const std::string& name)
{
    try {
        return context->methodCall<int>("Zone::remove", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Zone::lock(const std::string& name)
{
    try {
        return context->methodCall<int>("Zone::lock", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Zone::unlock(const std::string& name)
{
    try {
        return context->methodCall<int>("Zone::unlock", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> Zone::getList()
{
    std::vector<std::string> empty;
    try {
        return  context->methodCall<std::vector<std::string>>("Zone::getList");
    } catch (runtime::Exception& e) {
        return empty;
    }
}

int Zone::getState(const std::string& name)
{
    try {
        return context->methodCall<int>("Zone::getState", name);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

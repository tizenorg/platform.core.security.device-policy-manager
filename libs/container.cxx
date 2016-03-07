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

#include "policy-client.hxx"

#include "container.hxx"

namespace DevicePolicyManager {

Container::Container(PolicyControlContext& ctx)
    : context(ctx)
{
}

Container::~Container()
{
}

int Container::create(const std::string& name, const std::string& setupWizAppid)
{
    try {
        return context->methodCall<int>("Container::create", name, setupWizAppid);
    } catch (Runtime::Exception& e) {
        return -1;
    }
}

int Container::remove(const std::string& name)
{
    try {
        return context->methodCall<int>("Container::remove", name);
    } catch (Runtime::Exception& e) {
        return -1;
    }
}

int Container::lock(const std::string& name)
{
    try {
        return context->methodCall<int>("Container::lock", name);
    } catch (Runtime::Exception& e) {
        return -1;
    }
}

int Container::unlock(const std::string& name)
{
    try {
        return context->methodCall<int>("Container::unlock", name);
    } catch (Runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> Container::getList()
{
    std::vector<std::string> empty;
    try {
        return  context->methodCall<std::vector<std::string>>("Container::getList");
    } catch (Runtime::Exception& e) {
        return empty;
    }
}

int Container::getState(const std::string& name)
{
    try {
        return context->methodCall<int>("Container::getState", name);
    } catch (Runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

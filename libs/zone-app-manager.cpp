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

#include "zone-app-manager.hxx"

namespace DevicePolicyManager {

ZoneAppManager::ZoneAppManager(PolicyControlContext& ctx)
    : context(ctx)
{
}

ZoneAppManager::~ZoneAppManager()
{
}

std::vector<ZoneAppManager::AppInfo> ZoneAppManager::getList(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::vector<ZoneAppManager::AppInfo>>("ZoneAppManager::getList", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::vector<ZoneAppManager::AppInfo>();;
}

int ZoneAppManager::launch(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZoneAppManager::launch", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneAppManager::resume(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZoneAppManager::resume", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneAppManager::terminate(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZoneAppManager::terminate", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool ZoneAppManager::isRunning(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<bool>("ZoneAppManager::isRunning", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

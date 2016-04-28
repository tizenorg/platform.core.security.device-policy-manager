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

#include "zone/app-proxy.hxx"

namespace DevicePolicyManager {

ZoneAppProxy::ZoneAppProxy(PolicyControlContext& ctx)
    : context(ctx)
{
}

ZoneAppProxy::~ZoneAppProxy()
{
}

ZoneAppProxy::AppInfo ZoneAppProxy::getAppInfo(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<ZoneAppProxy::AppInfo>("ZoneAppProxy::getAppInfo", name, appid);
    } catch (runtime::Exception& e) {}
    return ZoneAppProxy::AppInfo();;
}

std::vector<std::string> ZoneAppProxy::getAppList(const std::string& zone)
{
    try {
        return context->methodCall<std::vector<std::string>>("ZoneAppProxy::getAppList", zone);
    } catch (runtime::Exception& e) {}
    return std::vector<std::string>();;
}

int ZoneAppProxy::launch(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZoneAppProxy::launch", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneAppProxy::resume(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZoneAppProxy::resume", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZoneAppProxy::terminate(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZoneAppProxy::terminate", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool ZoneAppProxy::isRunning(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<bool>("ZoneAppProxy::isRunning", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

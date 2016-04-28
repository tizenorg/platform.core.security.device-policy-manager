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
        return context->methodCall<std::vector<std::string>>("ZonePolicy::getZoneList");
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

ZonePolicy::PkgInfo ZonePolicy::getPkg(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<ZonePolicy::PkgInfo>("ZonePolicy::getPkg", name, pkgid);
    } catch (runtime::Exception& e) {}
    return ZonePolicy::PkgInfo();
}

std::vector<ZonePolicy::PkgInfo> ZonePolicy::getPkgList(const std::string& zone)
{
    try {
        return context->methodCall<std::vector<ZonePolicy::PkgInfo>>("ZonePolicy::getPkgList", zone);
    } catch (runtime::Exception& e) {}
    return std::vector<ZonePolicy::PkgInfo>();;
}

int ZonePolicy::installPkg(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePolicy::installPkg", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePolicy::uninstallPkg(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePolicy::uninstallPkg", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

ZonePolicy::AppInfo ZonePolicy::getApp(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<ZonePolicy::AppInfo>("ZonePolicy::getApp", name, appid);
    } catch (runtime::Exception& e) {}
    return ZonePolicy::AppInfo();
}

std::vector<ZonePolicy::AppInfo> ZonePolicy::getUIAppList(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::vector<ZonePolicy::AppInfo>>("ZonePolicy::getUIAppList", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::vector<ZonePolicy::AppInfo>();;
}

int ZonePolicy::launchApp(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZonePolicy::launchApp", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePolicy::resumeApp(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZonePolicy::resumeApp", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePolicy::terminateApp(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<int>("ZonePolicy::terminateApp", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool ZonePolicy::isRunningApp(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<bool>("ZonePolicy::isRunningApp", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

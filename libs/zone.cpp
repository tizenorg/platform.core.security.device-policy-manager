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

std::vector<std::string> ZonePolicy::getPackageList(const std::string& zone)
{
    std::vector<std::string> empty;
    try {
        return context->methodCall<std::vector<std::string>>("ZonePolicy::getPackageList", zone);
    } catch (runtime::Exception& e) {
        return empty;
    }
}

std::string ZonePolicy::getPackageType(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::string>("ZonePolicy::getPackageType", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::string();
}

std::string ZonePolicy::getPackageIcon(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::string>("ZonePolicy::getPackageIcon", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::string();
}

std::string ZonePolicy::getPackageLabel(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::string>("ZonePolicy::getPackageLabel", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::string();
}

std::string ZonePolicy::getPackageVersion(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::string>("ZonePolicy::getPackageVersion", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::string();
}

bool ZonePolicy::isSystemPackage(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<bool>("ZonePolicy::isSystemPackage", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return false;
}

bool ZonePolicy::isRemovablePackage(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<bool>("ZonePolicy::isRemovablePackage", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return false;
}

bool ZonePolicy::isPreloadPackage(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<bool>("ZonePolicy::isPreloadPackage", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return false;
}

int ZonePolicy::installPackage(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePolicy::installPackage", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePolicy::uninstallPackage(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePolicy::uninstallPackage", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> ZonePolicy::getUIAppList(const std::string& zone, const std::string& pkgid)
{
    std::vector<std::string> empty;
    try {
        return context->methodCall<std::vector<std::string>>("ZonePolicy::getUIAppList", zone, pkgid);
    } catch (runtime::Exception& e) {
        return empty;
    }
}

std::string ZonePolicy::getAppIcon(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::string>("ZonePolicy::getAppIcon", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::string();
}

std::string ZonePolicy::getAppLabel(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<std::string>("ZonePolicy::getAppIcon", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return std::string();
}

bool ZonePolicy::isNoDisplayedApp(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<bool>("ZonePolicy::isNoDisplayedApp", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return false;
}

bool ZonePolicy::isTaskManagedApp(const std::string& zone, const std::string& pkgid)
{
    try {
        return context->methodCall<bool>("ZonePolicy::isTaskManagedApp", zone, pkgid);
    } catch (runtime::Exception& e) {}
    return false;
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

bool ZonePolicy::isAppRunning(const std::string& name, const std::string& appid)
{
    try {
        return context->methodCall<bool>("ZonePolicy::isAppRunning", name, appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

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
#include <sys/stat.h>
#include <sys/types.h>
#include <system_settings.h>

#include "zone/app-proxy.hxx"

#include "error.h"
#include "packman.h"
#include "launchpad.h"
#include "auth/user.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

ZoneAppProxy::ZoneAppProxy(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (ZoneAppProxy::AppInfo)(ZoneAppProxy::getAppInfo)(std::string, std::string));
    context.registerParametricMethod(this, (std::vector<std::string>)(ZoneAppProxy::getAppList)(std::string));

    context.registerParametricMethod(this, (int)(ZoneAppProxy::launch)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZoneAppProxy::resume)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZoneAppProxy::terminate)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZoneAppProxy::isRunning)(std::string, std::string));
}

ZoneAppProxy::~ZoneAppProxy()
{
}

ZoneAppProxy::AppInfo ZoneAppProxy::getAppInfo(const std::string& name, const std::string& appid)
{
    ZoneAppProxy::AppInfo appInfo;
    char* locale;

    system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
    if (locale == NULL) {
        locale = strdup("No locale");
    }

    appInfo.zone = name;
    appInfo.id = appid;
    appInfo.locale = locale;

    free(locale);

    try {
        runtime::User user(name);
        ApplicationInfo appinfo(appid, user.getUid());
        appInfo.package = appinfo.getPackageId();

        appInfo.type = appinfo.getType();
        appInfo.icon = appinfo.getIcon();
        appInfo.label = appinfo.getLabel();

        appInfo.componentType = appinfo.getComponentType();

        appInfo.isNoDisplayed = appinfo.isNoDisplayed();
        appInfo.isTaskManaged = appinfo.isTaskManaged();
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve application info installed in the zone: " + appid);
    }

    return appInfo;
}

std::vector<std::string> ZoneAppProxy::getAppList(const std::string& name)
{
    try {
        runtime::User user(name);
        PackageManager& packman = PackageManager::instance();
        return packman.getAppList(user.getUid());
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the zone");
    }
    return std::vector<std::string>();
}

int ZoneAppProxy::launch(const std::string& name, const std::string& appid)
{
    try {
        runtime::User user(name);
        Launchpad launchpad(user.getUid());
        launchpad.launch(appid);
    } catch (runtime::Exception& e) {
        ERROR("Failed to launch app in the zone");
        return -1;
    }
    return 0;
}

int ZoneAppProxy::resume(const std::string& name, const std::string& appid)
{
    try {
        runtime::User user(name);
        Launchpad launchpad(user.getUid());
        launchpad.resume(appid);
    } catch (runtime::Exception& e) {
        ERROR("Failed to terminate app in the zone");
        return -1;
    }
    return 0;
}

int ZoneAppProxy::terminate(const std::string& name, const std::string& appid)
{
    try {
        runtime::User user(name);
        Launchpad launchpad(user.getUid());
        launchpad.terminate(appid);
    } catch (runtime::Exception& e) {
        ERROR("Failed to terminate app in the zone");
        return -1;
    }
    return 0;
}

bool ZoneAppProxy::isRunning(const std::string& name, const std::string& appid)
{
    try {
        runtime::User user(name);
        Launchpad launchpad(user.getUid());
        return launchpad.isRunning(appid);
    } catch (runtime::Exception& e) {
        ERROR("Failed to get app running state in the zone");
    }
    return false;
}

ZoneAppProxy zoneAppManager(Server::instance());

} // namespace DevicePolicyManager

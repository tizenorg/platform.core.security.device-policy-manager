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

#include "zone-app-manager.hxx"

#include "error.h"
#include "packman.h"
#include "launchpad.h"
#include "auth/user.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

ZoneAppManager::ZoneAppManager(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (ZoneAppManager::AppInfo)(ZoneAppManager::get)(std::string, std::string));
    context.registerParametricMethod(this, (std::vector<ZoneAppManager::AppInfo>)(ZoneAppManager::getList)(std::string, std::string));

    context.registerParametricMethod(this, (int)(ZoneAppManager::launch)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZoneAppManager::resume)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZoneAppManager::terminate)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZoneAppManager::isRunning)(std::string, std::string));
}

ZoneAppManager::~ZoneAppManager()
{
}

ZoneAppManager::AppInfo ZoneAppManager::get(const std::string& name, const std::string& appid)
{
    ZoneAppManager::AppInfo application;
    char* locale;

    system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
    if (locale == NULL) {
        locale = strdup("No locale");
    }

    application.zone = name;
    application.id = appid;
    application.locale = locale;

    free(locale);

    try {
        runtime::User user(name);
        ApplicationInfo appinfo(appid, user.getUid());

        application.icon = appinfo.getIcon();
        application.label = appinfo.getLabel();

        application.isNoDisplayed = appinfo.isNoDisplayed();
        application.isTaskManaged = appinfo.isTaskManaged();
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve application info installed in the zone");
    }

    return application;
}

std::vector<ZoneAppManager::AppInfo> ZoneAppManager::getList(const std::string& name, const std::string& pkgid)
{
    std::vector<ZoneAppManager::AppInfo> result;
    try {
        runtime::User user(name);
        PackageInfo pkginfo(pkgid, user.getUid());
        std::vector<std::string> &&list = pkginfo.getAppList();
        for (std::vector<std::string>::iterator it = list.begin();
             it != list.end(); it++) {
            result.push_back(get(name, *it));
        }
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve app list installed in the zone");
    }
    return result;
}

int ZoneAppManager::launch(const std::string& name, const std::string& appid)
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

int ZoneAppManager::resume(const std::string& name, const std::string& appid)
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

int ZoneAppManager::terminate(const std::string& name, const std::string& appid)
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

bool ZoneAppManager::isRunning(const std::string& name, const std::string& appid)
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

ZoneAppManager zoneAppManager(Server::instance());

} // namespace DevicePolicyManager

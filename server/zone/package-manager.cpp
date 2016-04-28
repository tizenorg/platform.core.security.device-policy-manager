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

#include "zone/package-manager.hxx"

#include "error.h"
#include "packman.h"
#include "auth/user.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

ZonePackageManager::ZonePackageManager(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (ZonePackageManager::PkgInfo)(ZonePackageManager::get)(std::string, std::string));
    context.registerParametricMethod(this, (std::vector<ZonePackageManager::PkgInfo>)(ZonePackageManager::getList)(std::string));

    context.registerParametricMethod(this, (int)(ZonePackageManager::install)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZonePackageManager::uninstall)(std::string, std::string));
}

ZonePackageManager::~ZonePackageManager()
{
}

ZonePackageManager::PkgInfo ZonePackageManager::get(const std::string& name, const std::string& pkgid)
{
    ZonePackageManager::PkgInfo package;
    char* locale = NULL;

    system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
    if (locale == NULL) {
        locale = strdup("No locale");
    }

    package.zone = name;
    package.id = pkgid;
    package.locale = locale;

    free(locale);

    try {
        runtime::User user(name);
        PackageInfo pkginfo(pkgid, user.getUid());

        package.type = pkginfo.getType();
        package.icon = pkginfo.getIcon();
        package.label = pkginfo.getLabel();
        package.version = pkginfo.getVersion();

        package.isSystem = pkginfo.isSystem();
        package.isRemovable = pkginfo.isRemovable();
        package.isPreload = pkginfo.isPreload();

        package.applicationList = pkginfo.getAppList();
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the zone");
    }

    return package;
}

std::vector<ZonePackageManager::PkgInfo> ZonePackageManager::getList(const std::string& name)
{
    std::vector<ZonePackageManager::PkgInfo> result;
    try {
        runtime::User user(name);
        PackageManager& packman = PackageManager::instance();
        const auto& list = packman.getPackageList(user.getUid());
        for (const auto& pkgid : list) {
            result.push_back(get(name, pkgid));
        }
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the zone");
    }
    return result;
}


int ZonePackageManager::install(const std::string& name, const std::string& pkgpath)
{
    try {
        runtime::User user(name);
        PackageManager& packman = PackageManager::instance();
        packman.installPackage(pkgpath, user.getUid());
    } catch (runtime::Exception& e) {
        ERROR("Failed to install package in the zone");
        return -1;
    }

    return 0;
}

int ZonePackageManager::uninstall(const std::string& name, const std::string& pkgid)
{
    try {
        runtime::User user(name);
        PackageManager& packman = PackageManager::instance();
        packman.uninstallPackage(pkgid, user.getUid());
    } catch (runtime::Exception& e) {
        ERROR("Failed to uninstall package of pkgid in the zone");
        return -1;
    }
    return 0;
}

ZonePackageManager zonePackageManager(Server::instance());

} // namespace DevicePolicyManager

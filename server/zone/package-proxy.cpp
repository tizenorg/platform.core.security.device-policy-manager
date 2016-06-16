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

#include "zone/package-proxy.hxx"

#include "error.h"
#include "packman.h"
#include "auth/user.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

namespace {

struct IteratorData{
    std::string zone;
    std::string locale;
    std::vector<PackageInfo> list;
    unsigned int current;
};

std::unordered_map<int, IteratorData> iteratorMap;
int newIteratorId = 0;

} // namespace

ZonePackageProxy::ZonePackageProxy(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (ZonePackageProxy::PackageInfo)(ZonePackageProxy::getPackageInfo)(std::string, std::string));

    context.registerParametricMethod(this, (int)(ZonePackageProxy::createIterator)(std::string));
    context.registerParametricMethod(this, (ZonePackageProxy::PackageInfo)(ZonePackageProxy::getIteratorValue)(int));
    context.registerParametricMethod(this, (bool)(ZonePackageProxy::nextIterator)(int));
    context.registerParametricMethod(this, (int)(ZonePackageProxy::destroyIterator)(int));

    context.registerParametricMethod(this, (int)(ZonePackageProxy::install)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZonePackageProxy::uninstall)(std::string, std::string));
}

ZonePackageProxy::~ZonePackageProxy()
{
}

ZonePackageProxy::PackageInfo ZonePackageProxy::getPackageInfo(const std::string& name, const std::string& pkgid)
{
    PackageInfo package;

    try {
        runtime::User user(name);
        ::PackageInfo pkginfo(pkgid, user.getUid());
        char* locale = NULL;

        system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
        if (locale == NULL) {
            package.locale = "No locale";
        } else {
            package.locale = locale;
            free(locale);
        }

        package.zone = name;
        package.id = pkgid;
        package.type = pkginfo.getType();
        package.icon = pkginfo.getIcon();
        package.label = pkginfo.getLabel();
        package.description = pkginfo.getDescription();
        package.author.name = pkginfo.getAuthorName();
        package.author.email = pkginfo.getAuthorEmail();
        package.author.href = pkginfo.getAuthorHref();
        package.version = pkginfo.getVersion();
        package.apiVersion = pkginfo.getApiVersion();
        package.mainAppId = pkginfo.getMainAppId();
        package.isSystem = pkginfo.isSystem();
        package.isRemovable = pkginfo.isRemovable();
        package.isPreload = pkginfo.isPreload();
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the zone");
    }

    return package;
}

int ZonePackageProxy::createIterator(const std::string& name)
{
    int iteratorId = -1;
    try {
        PackageManager& packman = PackageManager::instance();
        runtime::User user(name);
        IteratorData data;
        char* locale;

        iteratorId = newIteratorId;

        data.zone = name;
        data.list = packman.getPackageList(user.getUid());
        data.current = 0;

        system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
        if (locale == NULL) {
            data.locale = "No locale";
        } else {
            data.locale = locale;
            free(locale);
        }

        iteratorMap.insert(std::make_pair(iteratorId, data));

        if (++newIteratorId < 0) {
            newIteratorId = 0;
        }
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the zone");
    }
    return iteratorId;
}


ZonePackageProxy::PackageInfo ZonePackageProxy::getIteratorValue(int iterator)
{
    PackageInfo package;

    auto it = iteratorMap.find(iterator);
    if (it == iteratorMap.end()) {
        return package;
    }
    if (it->second.current >= it->second.list.size()) {
        return package;
    }

    const ::PackageInfo& pkginfo = it->second.list.at(it->second.current);

    package.zone = it->second.zone;
    package.locale = it->second.locale;;
    package.id = pkginfo.getId();
    package.type = pkginfo.getType();
    package.icon = pkginfo.getIcon();
    package.label = pkginfo.getLabel();
    package.description = pkginfo.getDescription();
    package.author.name = pkginfo.getAuthorName();
    package.author.email = pkginfo.getAuthorEmail();
    package.author.href = pkginfo.getAuthorHref();
    package.version = pkginfo.getVersion();
    package.apiVersion = pkginfo.getApiVersion();
    package.mainAppId = pkginfo.getMainAppId();
    package.isSystem = pkginfo.isSystem();
    package.isRemovable = pkginfo.isRemovable();
    package.isPreload = pkginfo.isPreload();

    return package;
}

bool ZonePackageProxy::nextIterator(int iterator) {
    auto it = iteratorMap.find(iterator);
    if (it != iteratorMap.end()) {
        if (++it->second.current < it->second.list.size()) {
            return true;
        }
    }
    return false;
}

int ZonePackageProxy::destroyIterator(int iterator) {
    auto it = iteratorMap.find(iterator);
    if (it != iteratorMap.end()) {
        iteratorMap.erase(it);
        return 0;
    }
    return -1;
}

int ZonePackageProxy::install(const std::string& name, const std::string& pkgpath)
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

int ZonePackageProxy::uninstall(const std::string& name, const std::string& pkgid)
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

ZonePackageProxy zonePackageManager(Server::instance());

} // namespace DevicePolicyManager

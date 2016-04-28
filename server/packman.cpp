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
#include <signal.h>

#include <vector>

#include "packman.h"
#include "exception.h"
#include "audit/logger.h"

namespace {

int PackageEventCallback(uid_t uid, int id, const char* type, const char* name,
                         const char* key, const char* val, const void* msg, void* data)
{
    return 0;
}

int AppListCallback(pkgmgrinfo_appinfo_h handle, void *data)
{
    char* appid = nullptr;

    ::pkgmgrinfo_appinfo_get_appid(handle, &appid);
    std::vector<std::string>* appList = static_cast<std::vector<std::string>*>(data);
    appList->push_back(appid);

    return 0;
}

int PackageListCallback(pkgmgrinfo_pkginfo_h handle, void *data)
{
    char* pkgid = nullptr;
    ::pkgmgrinfo_pkginfo_get_pkgid(handle, &pkgid);

    std::vector<std::string>* packageList = static_cast<std::vector<std::string>*>(data);
    packageList->push_back(pkgid);

    return 0;
}

} // namespace

PackageInfo::PackageInfo(const std::string& pkgid, uid_t uid) :
    user(uid), handle(nullptr)
{
    if (uid == 0) {
        uid = getuid();
    }

    if (::pkgmgrinfo_pkginfo_get_usr_pkginfo(pkgid.c_str(), user, &handle) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid Package Id");
    }
}

PackageInfo::~PackageInfo()
{
    ::pkgmgrinfo_pkginfo_destroy_pkginfo(handle);
}

std::vector<std::string> PackageInfo::getAppList() const
{
    std::vector<std::string> appList;

    if (::pkgmgrinfo_appinfo_get_usr_list(handle, PMINFO_ALL_APP, AppListCallback, &appList, user) != PMINFO_R_OK) {
        ERROR("Error in pkgmgrinfo_appinfo_get_usr_list");
    }

    return appList;
}

std::string PackageInfo::getType() const
{
    char *pkgtype;
    if (::pkgmgrinfo_pkginfo_get_type(handle, &pkgtype) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return pkgtype;
}

std::string PackageInfo::getLabel() const
{
    char *label;
    if (::pkgmgrinfo_pkginfo_get_label(handle, &label) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return label;
}

std::string PackageInfo::getIcon() const
{
    char *icon;
    if (::pkgmgrinfo_pkginfo_get_icon(handle, &icon) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return icon;
}

std::string PackageInfo::getVersion() const
{
    char *version;
    if (::pkgmgrinfo_pkginfo_get_version(handle, &version) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return version;
}

bool PackageInfo::isSystem() const
{
    bool ret;
    if (::pkgmgrinfo_pkginfo_is_system(handle, &ret) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return ret;
}

bool PackageInfo::isRemovable() const
{
    bool ret;
    if (::pkgmgrinfo_pkginfo_is_removable(handle, &ret) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return ret;
}

bool PackageInfo::isPreload() const
{
    bool ret;
    if (::pkgmgrinfo_pkginfo_is_preload(handle, &ret) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return ret;
}

ApplicationInfo::ApplicationInfo(const std::string& aid, uid_t uid) :
    user(uid), appid(aid), handle(nullptr)
{
    if (user == 0) {
        user = getuid();
    }

    if (::pkgmgrinfo_appinfo_get_usr_appinfo(appid.c_str(), user, &handle) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid App Id");
    }
}

ApplicationInfo::~ApplicationInfo()
{
    ::pkgmgrinfo_appinfo_destroy_appinfo(handle);
}

std::string ApplicationInfo::getPackageId() const
{
    char *pkgid;
    if (::pkgmgrinfo_appinfo_get_pkgid(handle, &pkgid) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return pkgid;
}

std::string ApplicationInfo::getPackageType() const
{
    char *type;
    if (::pkgmgrinfo_appinfo_get_pkgtype(handle, &type) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return type;
}

std::string ApplicationInfo::getPackageName() const
{
    char *name;
    if (::pkgmgrinfo_appinfo_get_pkgname(handle, &name) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return name;
}

std::string ApplicationInfo::getIcon() const
{
    char *icon;
    if (::pkgmgrinfo_appinfo_get_icon(handle, &icon) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return icon;
}

std::string ApplicationInfo::getLabel() const
{
    char *label;
    if (::pkgmgrinfo_appinfo_get_label(handle, &label) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return label;
}

int ApplicationInfo::getComponentType() const
{
    pkgmgrinfo_app_component ret;
    if (::pkgmgrinfo_appinfo_get_component(handle, &ret) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return static_cast<int>(ret);
}

bool ApplicationInfo::isNoDisplayed() const
{
    bool ret;
    if (::pkgmgrinfo_appinfo_is_nodisplay(handle, &ret) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return ret;
}

bool ApplicationInfo::isTaskManaged() const
{
    bool ret;
    if (::pkgmgrinfo_appinfo_is_taskmanage(handle, &ret) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return ret;
}

PackageManager::PackageManager() :
    nativeHandle(nullptr)
{
    nativeHandle = ::pkgmgr_client_new(PC_REQUEST);
    if (nativeHandle == nullptr) {
        throw runtime::Exception("No package manager instance");
    }
}

PackageManager::~PackageManager()
{
    ::pkgmgr_client_free(nativeHandle);
}

PackageManager& PackageManager::instance()
{
    static PackageManager __instance__;
    return __instance__;
}

PackageInfo PackageManager::getPackageInfo(const std::string& pkgid, const uid_t user)
{
    return PackageInfo(pkgid, user);
}

void PackageManager::activatePackage(const std::string& pkgid, const uid_t user)
{
    PackageInfo package(pkgid, user);

    if (::pkgmgr_client_usr_activate(nativeHandle, package.getType().c_str(), pkgid.c_str(), user) != PKGMGR_R_OK) {
        ERROR("Failed to activate package: " + pkgid);
        throw runtime::Exception("Operation failed");
    }
}

void PackageManager::deactivatePackage(const std::string& pkgid, const uid_t user)
{
    PackageInfo package(pkgid, user);

    int ret = pkgmgr_client_usr_deactivate(nativeHandle, package.getType().c_str(), pkgid.c_str(), user);
    if (ret != PKGMGR_R_OK) {
        ERROR("Failed to deactivate package: " + std::to_string(ret));
        throw runtime::Exception("Operation failed");
    }
}

void PackageManager::installPackage(const std::string& pkgpath, const uid_t user)
{
    int ret = ::pkgmgr_client_usr_install(nativeHandle, NULL, NULL, pkgpath.c_str(), NULL, PM_QUIET, PackageEventCallback, nullptr, user);
    if (ret != PKGMGR_R_OK) {
        ERROR("Error in pkgmgr_client_install");
        throw runtime::Exception("Package installation failed");
    }
}

void PackageManager::uninstallPackage(const std::string& pkgid, const uid_t user)
{
    std::string pkgtype;

    PackageInfo pkgInfo(pkgid);
    pkgtype = pkgInfo.getType();

    int ret = ::pkgmgr_client_usr_uninstall(nativeHandle, pkgtype.c_str(), pkgid.c_str(), PM_QUIET, PackageEventCallback, nullptr, user);
    if (ret < PKGMGR_R_OK) {
        ERROR("Error in pkgmgr_client_uninstall");
        throw runtime::Exception("Package manager exception");
    }
}

void PackageManager::wipePackageData(const std::string& pkgid, const uid_t user)
{
    PackageInfo package(pkgid);

    int ret = ::pkgmgr_client_usr_clear_user_data(nativeHandle, package.getType().c_str(), pkgid.c_str(), PM_QUIET, user);
    if (ret != PKGMGR_R_OK) {
        ERROR("Error in pkgmgr_clear_user_data");
        throw runtime::Exception("Operation failed");
    }
}

std::vector<std::string> PackageManager::getPackageList(const uid_t user)
{
    std::vector<std::string> packageList;

    if (::pkgmgrinfo_pkginfo_get_usr_list(PackageListCallback, &packageList, user) != PMINFO_R_OK) {
        ERROR("Error in pkgmgrinfo_pkginfo_get_list");
    }

    return packageList;
}

std::vector<std::string> PackageManager::getAppList(const uid_t user)
{
    std::vector<std::string> appList;

    if (::pkgmgrinfo_appinfo_get_usr_installed_list(AppListCallback, user, &appList) != PMINFO_R_OK) {
        ERROR("Error in pkgmgrinfo_appinfo_get_installed_list");
    }

    return appList;
}

void PackageManager::addPackageToBlacklist(const std::string& pkgid, const uid_t user)
{
    if (::pkgmgr_client_usr_add_blacklist(nativeHandle, pkgid.c_str(), user) < 0) {
        ERROR("Error in pkgmgr_client_add_blacklist");
        throw runtime::Exception("Operation failed");
    }
}

void PackageManager::removePackageFromBlacklist(const std::string& pkgid, const uid_t user)
{
    if (::pkgmgr_client_usr_remove_blacklist(nativeHandle, pkgid.c_str(), user) < 0) {
        ERROR("Error in pkgmgr_client_remove_blacklist");
        throw runtime::Exception("Operation failed");
    }
}

bool PackageManager::checkPackageIsBlacklisted(const std::string& pkgid, const uid_t user)
{
    bool ret;
    if (::pkgmgr_client_usr_check_blacklist(nativeHandle, pkgid.c_str(), &ret, user) < 0) {
        ERROR("Error in pkgmgr_client_check_blacklist");
        throw runtime::Exception("Operation failed");
    }

    return ret;
}

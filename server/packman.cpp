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

/*
 int ApplicationListCallback(pkgmgrinfo_appinfo_h handle, void *data)
 {
     char* appid = nullptr;
     ::pkgmgrinfo_appinfo_get_appid(handle, &appid);

     std::vector<std::string>* appList = static_cast<std::vector<std::string>*>(data);
     appList->push_back(appid);

     return 0;
 }
*/

int PackageEventCallback(uid_t uid, int id, const char* type, const char* name,
                         const char* key, const char* val, const void* msg, void* data)
{
    return 0;
}

 int PackageListCallback(pkgmgrinfo_pkginfo_h handle, void *data)
 {
     char* pkgid = nullptr;
     ::pkgmgrinfo_pkginfo_get_pkgid(handle, &pkgid);

     std::cout << "PKG: " << pkgid << std::endl;
     std::vector<std::string>* packageList = static_cast<std::vector<std::string>*>(data);
     packageList->push_back(pkgid);

     return 0;
 }

 void PackageSizeCollector(pkgmgr_client* pc, const char *pkgid, const pkg_size_info_t *info, void *data)
 {
     pkg_size_info_t *i = static_cast<pkg_size_info_t*>(data);

     *i = *info;
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

std::string PackageInfo::getType() const
{
    char *pkgtype;
    if (::pkgmgrinfo_pkginfo_get_type(handle, &pkgtype) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return pkgtype;
}

std::string PackageInfo::getVersion() const
{
    char *version;
    if (::pkgmgrinfo_pkginfo_get_version(handle, &version) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid operation");
    }

    return version;
}

AppInfo::AppInfo(const std::string& aid, uid_t uid) :
    user(uid), appid(aid), handle(nullptr)
{
    if (user == 0) {
        user = getuid();
    }

    if (::pkgmgrinfo_appinfo_get_usr_appinfo(appid.c_str(), user, &handle) != PMINFO_R_OK) {
        throw runtime::Exception("Invalid App Id");
    }
}

AppInfo::~AppInfo()
{
    ::pkgmgrinfo_appinfo_destroy_appinfo(handle);
}

std::string AppInfo::getPackageId() const
{
    char *pkgid;
    if (::pkgmgrinfo_appinfo_get_pkgid(handle, &pkgid) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return pkgid;
}

std::string AppInfo::getPackageType() const
{
    char *type;
    if (::pkgmgrinfo_appinfo_get_pkgtype(handle, &type) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return type;
}

std::string AppInfo::getPackageName() const
{
    char *name;
    if (::pkgmgrinfo_appinfo_get_pkgname(handle, &name) != PMINFO_R_OK) {
        throw runtime::Exception("Unexpected operation");
    }

    return name;
}

long long AppInfo::getCodeSize() const
{
    pkgmgr_client *pc = ::pkgmgr_client_new(PC_REQUEST);
    if (pc == nullptr) {
        ERROR("Error in pkgmgr_client_new");
        throw runtime::Exception("Unexpected operation");
    }

    pkg_size_info_t size;
    int ret = ::pkgmgr_client_usr_get_package_size_info(pc, appid.c_str(), PackageSizeCollector, &size, user);
    if (ret != PKGMGR_R_OK) {
        ::pkgmgr_client_free(pc);
        ERROR("Error in pkgmgr_client_get_package_size_info");
        throw runtime::Exception("Unexpected operation");
    }

    ::pkgmgr_client_free(pc);
    return size.app_size;
}

long long AppInfo::getDataSize() const
{
    pkgmgr_client *pc = ::pkgmgr_client_new(PC_REQUEST);
    if (pc == nullptr) {
        ERROR("Error in pkgmgr_client_new");
        return -1;
    }

    pkg_size_info_t size;
    int ret = ::pkgmgr_client_usr_get_package_size_info(pc, appid.c_str(), PackageSizeCollector, &size, user);
    if (ret != PKGMGR_R_OK) {
        ::pkgmgr_client_free(pc);
        ERROR("Error in pkgmgr_client_get_package_size_info");
        return -1;
    }

    ::pkgmgr_client_free(pc);
    return size.data_size;
}

PackageInfo PackageManager::getPackageInfo(const std::string& pkgid, const uid_t user)
{
    return PackageInfo(pkgid, user);
}

void PackageManager::activatePackage(const std::string& pkgid, const uid_t user)
{
    PackageInfo package(pkgid, user);

    pkgmgr_client *pc = ::pkgmgr_client_new(PC_REQUEST);
    if (pc == nullptr) {
        throw runtime::Exception("Package Manager Error");
    }

    if (::pkgmgr_client_usr_activate(pc, package.getType().c_str(), pkgid.c_str(), user) != PKGMGR_R_OK) {
        ::pkgmgr_client_free(pc);

        throw runtime::Exception("Operation failed");
    }

    ::pkgmgr_client_free(pc);
}

void PackageManager::deactivatePackage(const std::string& pkgid, const uid_t user)
{
    PackageInfo package(pkgid, user);

    pkgmgr_client *pc = ::pkgmgr_client_new(PC_REQUEST);
    if (pc == nullptr) {
        throw runtime::Exception("Package Manager Error");
    }

    std::cout << "Package Type: " << package.getType() << std::endl;
    int ret = pkgmgr_client_usr_deactivate(pc, package.getType().c_str(), pkgid.c_str(), user);
    if (ret != PKGMGR_R_OK) {
        ::pkgmgr_client_free(pc);
        ERROR("Failed to deactivate package: " + std::to_string(ret));
        throw runtime::Exception("Operation failed");
    }

    ::pkgmgr_client_free(pc);
}

void PackageManager::installPackage(const std::string& pkgpath, const uid_t user)
{
    pkgmgr_client *pc = ::pkgmgr_client_new(PC_REQUEST);
    if (pc == nullptr) {
        ERROR("Error in pkgmgr_client_new");
        throw runtime::Exception("Package manager exception");
    }

    int ret = ::pkgmgr_client_usr_install(pc, NULL, NULL, pkgpath.c_str(), NULL, PM_QUIET, PackageEventCallback, nullptr, user);
    if (ret != PKGMGR_R_OK) {
        ::pkgmgr_client_free(pc);
        ERROR("Error in pkgmgr_client_install");
        throw runtime::Exception("Package installation failed");
    }

    // pkgmgr_client handle will be destroyed in the callback when package installation is completed
}

void PackageManager::uninstallPackage(const std::string& pkgid, const uid_t user)
{
    std::string pkgtype;

    PackageInfo pkgInfo(pkgid);
    pkgtype = pkgInfo.getType();

    pkgmgr_client *pc = ::pkgmgr_client_new(PC_REQUEST);
    if (pc == nullptr) {
        throw runtime::Exception("Package manager exception");
        ERROR("Error in pkgmgr_client_new");
    }

    int ret = ::pkgmgr_client_usr_uninstall(pc, pkgtype.c_str(), pkgid.c_str(), PM_QUIET, PackageEventCallback, nullptr, user);
    if (ret < PKGMGR_R_OK) {
        ::pkgmgr_client_free(pc);
        ERROR("Error in pkgmgr_client_uninstall");
        throw runtime::Exception("Package manager exception");
    }

    // pkgmgr_client handle will be destroyed in the callback when package installation is completed
}

void PackageManager::wipePackageData(const std::string& pkgid, const uid_t user)
{
    PackageInfo package(pkgid);

    pkgmgr_client *pc = ::pkgmgr_client_new(PC_REQUEST);
    if (pc == nullptr) {
        throw runtime::Exception("Invalid package manager");
    }

    int ret = ::pkgmgr_client_usr_clear_user_data(pc, package.getType().c_str(), pkgid.c_str(), PM_QUIET, user);
    if (ret != PKGMGR_R_OK) {
        ::pkgmgr_client_free(pc);
        throw runtime::Exception("Operation failed");
    }

    ::pkgmgr_client_free(pc);
}

std::vector<std::string> PackageManager::getInstalledPackageList(const uid_t user)
{
    std::vector<std::string> packageList;

    if (::pkgmgrinfo_pkginfo_get_usr_list(PackageListCallback, &packageList, user) != PMINFO_R_OK) {
        ERROR("Error in pkgmgrinfo_pkginfo_get_list");
        return std::vector<std::string>();
    }

    return packageList;
}

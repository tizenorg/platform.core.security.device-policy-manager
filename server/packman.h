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

#ifndef __DPM_PACKMAN_H__
#define __DPM_PACKMAN_H__

#include <sys/types.h>
#include <unistd.h>

#include <string>

#include <package-manager.h>
#include <pkgmgr-info.h>

 class PackageInfo {
 public:
     PackageInfo(const std::string& pkgid, uid_t uid = 0);
     ~PackageInfo();

     std::string getType() const;
     std::string getVersion() const;

 private:
     uid_t user;
     pkgmgrinfo_pkginfo_h handle;
 };

 class AppInfo {
 public:
     AppInfo(const std::string& aid, uid_t uid = 0);
     ~AppInfo();

     std::string getPackageId() const;
     std::string getPackageName() const;
     std::string getPackageType() const;
     std::string getVersion() const;
     long long getCodeSize() const;
     long long getDataSize() const;

 private:
     uid_t user;
     std::string appid;
     pkgmgrinfo_appinfo_h handle;
 };

 class PackageManager {
 public:
     static PackageInfo getPackageInfo(const std::string& pkgid, const uid_t user);
     static void activatePackage(const std::string& pkgid, const uid_t user);
     static void deactivatePackage(const std::string& pkgid, const uid_t user);
     static void installPackage(const std::string& pkgpath, const uid_t user);
     static void uninstallPackage(const std::string& pkgid, const uid_t user);
     static void wipePackageData(const std::string& pkgid, const uid_t user);
     static std::vector<std::string> getInstalledPackageList(const uid_t user);
 };
 #endif // __DPM_PACKMAN_H__

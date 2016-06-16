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

class ApplicationInfo {
public:
    ApplicationInfo(const std::string& aid, uid_t uid = 0);
    ApplicationInfo(pkgmgrinfo_appinfo_h handle);

    const std::string& getId() const;
    const std::string& getPackage() const;
    const std::string& getType() const;
    const std::string& getIcon() const;
    const std::string& getLabel() const;
    int getComponentType() const;
    bool isNoDisplayed() const;
    bool isTaskManaged() const;

private:
    void load(pkgmgrinfo_appinfo_h handle);

    std::string id;
    std::string package;
    std::string type;
    std::string icon;
    std::string label;
    int componentType;
    bool noDisplayed;
    bool taskManaged;
};

class PackageInfo {
public:
    PackageInfo(const std::string& pkgid, uid_t uid = 0);
    PackageInfo(pkgmgrinfo_pkginfo_h handle);

    const std::string& getId() const;
    const std::string& getType() const;
    const std::string& getIcon() const;
    const std::string& getLabel() const;
    const std::string& getDescription() const;
    const std::string& getAuthorName() const;
    const std::string& getAuthorEmail() const;
    const std::string& getAuthorHref() const;
    const std::string& getVersion() const;
    const std::string& getApiVersion() const;
    const std::string& getMainAppId() const;
    bool isSystem() const;
    bool isRemovable() const;
    bool isPreload() const;

private:
    void load(pkgmgrinfo_pkginfo_h handle);

    std::string id;
    std::string locale;
    std::string type;
    std::string icon;
    std::string label;
    std::string description;
    struct {
        std::string name;
        std::string email;
        std::string href;
    } author;
    std::string version;
    std::string apiVersion;
    std::string mainAppId;
    bool system;
    bool removable;
    bool preload;
};

class PackageManager {
public:
    void activatePackage(const std::string& pkgid, const uid_t user);
    void deactivatePackage(const std::string& pkgid, const uid_t user);
    void installPackage(const std::string& pkgpath, const uid_t user);
    void uninstallPackage(const std::string& pkgid, const uid_t user);
    void wipePackageData(const std::string& pkgid, const uid_t user);

    std::vector<PackageInfo> getPackageList(const uid_t user);
    std::vector<ApplicationInfo> getAppList(const uid_t user);
    std::vector<ApplicationInfo> getAppList(const uid_t user, const std::string& pkgid) const;

    void setEventCallback(pkgmgrinfo_handler callback, void* user_data);
    void unsetEventCallback();

    void setModeRestriction(int mode, uid_t user);
    void unsetModeRestriction(int mode, uid_t user);
    int getModeRestriction(uid_t user);

    void setPackageRestriction(const std::string& pkgid, int mode, uid_t user);
    void unsetPackageRestriction(const std::string& pkgid, int mode, uid_t user);
    int getPackageRestriction(const std::string& pkgid, uid_t user);

    static PackageManager& instance();

private:
    PackageManager();
    ~PackageManager();

private:
    pkgmgr_client *nativeRequestHandle, *nativeListenHandle;
};

#endif // __DPM_PACKMAN_H__

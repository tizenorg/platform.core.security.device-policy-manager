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

#ifndef __ZONE_POLICY__
#define __ZONE_POLICY__

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager {

/**
 * This class provides APIs to create/remove and manage names.
 */

class ZonePolicy {
public:
    ZonePolicy(PolicyControlContext& ctxt);
    ~ZonePolicy();

    //management
    int createZone(const std::string& name, const std::string& setupWizardAppid);
    int removeZone(const std::string& name);
    int lockZone(const std::string& name);
    int unlockZone(const std::string& name);

    std::vector<std::string> getZoneList(void);
    int getZoneState(const std::string& name);

    //package information
    struct PkgInfo {
        std::string zone, id;
        std::string type, icon, label, version;
        bool isSystem, isRemovable, isPreload;

        REFLECTABLE(zone, id,
                    type, icon, label, version,
                    isSystem, isRemovable, isPreload);
    };

    PkgInfo getPkg(const std::string& name, const std::string& pkgid);
    std::vector<PkgInfo> getPkgList(const std::string& name);

    //package manager request
    int installPkg(const std::string& name, const std::string& pkgpath);
    int uninstallPkg(const std::string& name, const std::string& pkgid);

    //application information
    struct AppInfo {
        std::string zone, id;
        std::string icon, label;
        bool isNoDisplayed, isTaskManaged;

        REFLECTABLE(zone, id,
                    icon, label,
                    isNoDisplayed, isTaskManaged);
    };

    AppInfo getApp(const std::string& name, const std::string& appid);
    std::vector<AppInfo> getUIAppList(const std::string& name, const std::string &pkgid);

    //app control for homescreen
    int launchApp(const std::string& name, const std::string& appid);

    //app control for task manager
    int resumeApp(const std::string& name, const std::string& appid);
    int terminateApp(const std::string& name, const std::string& appid);
    bool isRunningApp(const std::string& name, const std::string& appid);

private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __ZONE_POLICY__

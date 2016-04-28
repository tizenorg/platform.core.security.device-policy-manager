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
 * This class provides APIs to create/remove and manage zones.
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
    std::vector<std::string> getPackageList(const std::string& zone);
    std::string getPackageType(const std::string& zone, const std::string& pkgid);
    std::string getPackageIcon(const std::string& zone, const std::string& pkgid);
    std::string getPackageLabel(const std::string& zone, const std::string& pkgid);
    std::string getPackageVersion(const std::string& zone, const std::string& pkgid);
    bool isSystemPackage(const std::string& zone, const std::string& pkgid);
    bool isRemovablePackage(const std::string& zone, const std::string& pkgid);
    bool isPreloadPackage(const std::string& zone, const std::string& pkgid);

    //package manager request
    int installPackage(const std::string& zone, const std::string& pkgpath);
    int uninstallPackage(const std::string& zone, const std::string& pkgid);

    //application information
    std::vector<std::string> getUIAppList(const std::string& zone, const std::string &pkgid);
    std::string getAppIcon(const std::string& zone, const std::string& appid);
    std::string getAppLabel(const std::string& zone, const std::string& appid);
    bool isNoDisplayedApp(const std::string& zone, const std::string& appid);
    bool isTaskManagedApp(const std::string& zone, const std::string& appid);

    //app control for homescreen
    int launchApp(const std::string& zone, const std::string& appid);

    //app control for task manager
    int resumeApp(const std::string& zone, const std::string& appid);
    int terminateApp(const std::string& zone, const std::string& appid);
    bool isAppRunning(const std::string& zone, const std::string& appid);

private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __ZONE_POLICY__

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

#ifndef __APPLICATION_POLICY__
#define __APPLICATION_POLICY__

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager {

class ApplicationPolicy {
public:
    ApplicationPolicy(PolicyControlContext& ctxt);
    ~ApplicationPolicy();

    int setApplicationPolicyInstallationMode(const bool mode);
    bool getApplicationPolicyInstallationMode();

    int setApplicationPolicyUninstallationMode(const bool mode);
    bool getApplicationPolicyUninstallationMode();

    int setApplicationPolicyState(const std::string& appid, const int state);
    int getApplicationPolicyState(const std::string& appid);

    int disableApplicationPolicy(const std::string& appid);
    int enableApplicationPolicy(const std::string& appid);

    std::vector<std::string> getInstalledPackageList();

    int installPackage(const std::string& pkgpath);
    int uninstallPackage(const std::string& pkgid);

    bool isApplicationPolicyInstalled(const std::string& appid);
    bool isApplicationPolicyRunning(const std::string& appid);
    bool isPackageInstalled(const std::string& pkgid);

    int startApplicationPolicy(const std::string& appid);
    int stopApplicationPolicy(const std::string& appid);

    int wipeApplicationPolicyData(const std::string& appid);

    int addPackageToBlacklist(const std::string& pkgid);
    int removePackageFromBlacklist(const std::string& pkgid);
    int checkPackageIsBlacklisted(const std::string& pkgid);

private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __APPLICATION_POLICY__

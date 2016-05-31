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
    enum PrivilegeType {
        PRIVILEGE_TYPE_WRT = 0,
        PRIVILEGE_TYPE_CORE = 1
    };

    ApplicationPolicy(PolicyControlContext& ctxt);
    ~ApplicationPolicy();

    int setApplicationInstallationMode(int mode);
    int getApplicationInstallationMode();

    int setApplicationUninstallationMode(int mode);
    int getApplicationUninstallationMode();

    int setApplicationState(const std::string& appid, int state);
    int getApplicationState(const std::string& appid);

    int disableApplication(const std::string& appid);
    int enableApplication(const std::string& appid);

    int installPackage(const std::string& pkgpath);
    int uninstallPackage(const std::string& pkgid);

    int startApplication(const std::string& appid);
    int stopApplication(const std::string& appid);

    int wipeApplicationData(const std::string& appid);

    int addPackageToBlacklist(const std::string& pkgid);
    int removePackageFromBlacklist(const std::string& pkgid);
    int checkPackageIsBlacklisted(const std::string& pkgid);

    int addPrivilegeToBlacklist(int type, const std::string& privilege);
    int removePrivilegeFromBlacklist(int type, const std::string& privilege);
    int checkPrivilegeIsBlacklisted(int type, const std::string& privilege);

private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __APPLICATION_POLICY__

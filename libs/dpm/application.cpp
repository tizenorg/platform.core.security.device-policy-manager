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

#include <cassert>

#include "application.hxx"

#include "application.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_application_policy_h dpm_context_acquire_application_policy(dpm_context_h handle, const char* zone)
{
    assert(handle);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    //return client.createPolicyInterface<ApplicationPolicy>(zone);
    return client.createPolicyInterface<ApplicationPolicy>();
}

int dpm_context_release_application_policy(dpm_application_policy_h handle)
{
    assert(handle);

    delete &GetPolicyInterface<ApplicationPolicy>(handle);
    return 0;
}

int dpm_application_set_installation_mode(dpm_application_policy_h handle, int mode)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.setApplicationPolicyInstallationMode(mode);
}

int dpm_application_set_uninstallation_mode(dpm_application_policy_h handle, int mode)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.setApplicationPolicyUninstallationMode(mode);
}

int dpm_application_get_installation_mode(dpm_application_policy_h handle)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.getApplicationPolicyInstallationMode();
}

int dpm_application_get_uninstallation_mode(dpm_application_policy_h handle)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.getApplicationPolicyUninstallationMode();
}

int dpm_application_set_package_state(dpm_application_policy_h handle, const char* pkgid, dpm_package_state_e state)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.setApplicationPolicyState(pkgid, state);
}

int dpm_application_get_package_state(dpm_application_policy_h handle, const char* pkgid)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.getApplicationPolicyState(pkgid);
}

int dpm_application_add_package_to_blacklist(dpm_application_policy_h handle, const char* pkgid)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.addPackageToBlacklist(pkgid);
}

int dpm_application_remove_package_from_blacklist(dpm_application_policy_h handle, const char* pkgid)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.removePackageFromBlacklist(pkgid);
}

int dpm_application_check_package_is_blacklisted(dpm_application_policy_h handle, const char* pkgid)
{
    assert(handle);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.checkPackageIsBlacklisted(pkgid);
}

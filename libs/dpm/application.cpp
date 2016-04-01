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

#include "application.h"
#include "application.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_application_policy_h dpm_context_acquire_application_policy(dpm_context_h handle, const char* zone)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    //return client.createPolicyInterface<ApplicationPolicy>(zone);
    return client.createPolicyInterface<ApplicationPolicy>();
}

int dpm_context_release_application_policy(dpm_context_h context, dpm_application_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<ApplicationPolicy>(handle);
    return DPM_ERROR_NONE;
}

int dpm_application_set_installation_mode(dpm_application_policy_h handle, int mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.setApplicationInstallationMode(mode);
}

int dpm_application_set_uninstallation_mode(dpm_application_policy_h handle, int mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.setApplicationUninstallationMode(mode);
}

int dpm_application_get_installation_mode(dpm_application_policy_h handle, int *mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    int ret = application.getApplicationInstallationMode();
    if (ret < 0) {
        return -1;
    }
    *mode = ret;
    return DPM_ERROR_NONE;
}

int dpm_application_get_uninstallation_mode(dpm_application_policy_h handle, int *mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mode, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    int ret = application.getApplicationUninstallationMode();
    if (ret < 0) {
        return -1;
    }
    *mode = ret;
    return DPM_ERROR_NONE;
}

int dpm_application_set_package_state(dpm_application_policy_h handle, const char* pkgid, dpm_package_state_e state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.setApplicationState(pkgid, state);
}

int dpm_application_get_package_state(dpm_application_policy_h handle, const char* pkgid, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    int ret = application.getApplicationState(pkgid);
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_application_add_package_to_blacklist(dpm_application_policy_h handle, const char* pkgid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.addPackageToBlacklist(pkgid);
}

int dpm_application_remove_package_from_blacklist(dpm_application_policy_h handle, const char* pkgid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    return application.removePackageFromBlacklist(pkgid);
}

int dpm_application_check_package_is_blacklisted(dpm_application_policy_h handle, const char* pkgid, int *blacklisted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(blacklisted, DPM_ERROR_INVALID_PARAMETER);

    ApplicationPolicy& application = GetPolicyInterface<ApplicationPolicy>(handle);
    int ret = application.checkPackageIsBlacklisted(pkgid);
    if (ret < 0) {
        return -1;
    }

    *blacklisted = ret;
    return DPM_ERROR_NONE;
}

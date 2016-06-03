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

EXPORT_API int dpm_application_set_installation_mode(device_policy_manager_h handle, int mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.setApplicationInstallationMode(mode);
}

EXPORT_API int dpm_application_set_uninstallation_mode(device_policy_manager_h handle, int mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.setApplicationUninstallationMode(mode);
}

EXPORT_API int dpm_application_get_installation_mode(device_policy_manager_h handle, int *mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    int ret = application.getApplicationInstallationMode();
    if (ret < 0) {
        return -1;
    }
    *mode = ret;
    return DPM_ERROR_NONE;
}

EXPORT_API int dpm_application_get_uninstallation_mode(device_policy_manager_h handle, int *mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mode, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    int ret = application.getApplicationUninstallationMode();
    if (ret < 0) {
        return -1;
    }
    *mode = ret;
    return DPM_ERROR_NONE;
}

EXPORT_API int dpm_application_set_package_state(device_policy_manager_h handle, const char* pkgid, dpm_package_state_e state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.setApplicationState(pkgid, state);
}

EXPORT_API int dpm_application_get_package_state(device_policy_manager_h handle, const char* pkgid, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    int ret = application.getApplicationState(pkgid);
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

EXPORT_API int dpm_application_add_package_to_blacklist(device_policy_manager_h handle, const char* pkgid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.addPackageToBlacklist(pkgid);
}

EXPORT_API int dpm_application_remove_package_from_blacklist(device_policy_manager_h handle, const char* pkgid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.removePackageFromBlacklist(pkgid);
}

EXPORT_API int dpm_application_check_package_is_blacklisted(device_policy_manager_h handle, const char* pkgid, int *blacklisted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(blacklisted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    int ret = application.checkPackageIsBlacklisted(pkgid);
    if (ret < 0) {
        return -1;
    }

    *blacklisted = ret;
    return DPM_ERROR_NONE;
}

EXPORT_API int dpm_application_add_privilege_to_blacklist(device_policy_manager_h handle, int type, const char* privilege)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(privilege, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
	return application.addPrivilegeToBlacklist(type, privilege);
}

EXPORT_API int dpm_application_remove_privilege_from_blacklist(device_policy_manager_h handle, int type, const char* privilege)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(privilege, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
	return application.removePrivilegeFromBlacklist(type, privilege);
}

EXPORT_API int dpm_application_check_privilege_is_blacklisted(device_policy_manager_h handle, int type, const char* privilege, int *blacklisted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(privilege, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(blacklisted, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
	int ret = application.checkPrivilegeIsBlacklisted(type, privilege);
	if (ret < 0) {
		return -1;
	}

	*blacklisted = ret;
	return DPM_ERROR_NONE;
}

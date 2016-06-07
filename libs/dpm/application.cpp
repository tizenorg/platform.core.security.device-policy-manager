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

dpm_application_policy_h dpm_context_acquire_application_policy(dpm_context_h handle)
{
	return handle;
}

int dpm_context_release_application_policy(dpm_context_h context, dpm_application_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    return DPM_ERROR_NONE;
}

int dpm_application_set_mode_restriction(dpm_application_policy_h handle, int mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mode, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.setModeRestriction(mode);
}

int dpm_application_unset_mode_restriction(dpm_application_policy_h handle, int mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mode, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.unsetModeRestriction(mode);
}

int dpm_application_get_mode_restriction(dpm_application_policy_h handle, int *mode)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mode, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    int ret = application.getModeRestriction();
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

    DevicePolicyContext& client = GetDevicePolicyContext(handle);
    ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
    return application.setApplicationState(pkgid, state);
}

int dpm_application_get_package_state(dpm_application_policy_h handle, const char* pkgid, int *state)
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

int dpm_application_add_privilege_to_blacklist(dpm_application_policy_h handle, int type, const char* privilege)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(privilege, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
	return application.addPrivilegeToBlacklist(type, privilege);
}

int dpm_application_remove_privilege_from_blacklist(dpm_application_policy_h handle, int type, const char* privilege)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(privilege, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
	return application.removePrivilegeFromBlacklist(type, privilege);
}

int dpm_application_check_privilege_is_blacklisted(dpm_application_policy_h handle, int type, const char* privilege, int *blacklisted)
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

int dpm_application_install_package(dpm_application_policy_h handle, const char* pkgpath)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(pkgpath, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
	return application.installPackage(pkgpath);
}

int dpm_application_uninstall_package(dpm_application_policy_h handle, const char* pkgid)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	ApplicationPolicy application = client.createPolicyInterface<ApplicationPolicy>();
	return application.uninstallPackage(pkgid);
}

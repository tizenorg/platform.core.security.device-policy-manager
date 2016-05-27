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

#include "security.h"
#include "security.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_security_policy_h dpm_context_acquire_security_policy(dpm_context_h handle)
{
	return handle;
}

int dpm_context_release_security_policy(dpm_context_h context, dpm_security_policy_h handle)
{
    return DPM_ERROR_NONE;
}

int dpm_security_lockout_device(dpm_security_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.lockoutDevice();
}

int dpm_security_lockout_screen(dpm_security_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.lockoutScreen();
}

int dpm_security_set_internal_storage_encryption(dpm_security_policy_h handle, const int encrypt)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.setInternalStorageEncryption(encrypt);
}

int dpm_security_is_internal_storage_encrypted(dpm_security_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    int ret = security.isInternalStorageEncrypted();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_security_set_external_storage_encryption(dpm_security_policy_h handle, const int encrypt)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.setExternalStorageEncryption(encrypt);
}

int dpm_security_is_external_storage_encrypted(dpm_security_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    int ret = security.isExternalStorageEncrypted();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

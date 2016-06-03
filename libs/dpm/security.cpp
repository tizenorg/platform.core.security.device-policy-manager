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
#include "storage.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_security_lockout_device(device_policy_manager_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.lockoutDevice();
}

int dpm_security_lockout_screen(device_policy_manager_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.lockoutScreen();
}

int dpm_security_set_internal_storage_encryption(device_policy_manager_h handle, bool encrypt)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.setInternalStorageEncryption(encrypt);
}

int dpm_security_is_internal_storage_encrypted(device_policy_manager_h handle, bool *is_encrypted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_encrypted, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    int ret = security.isInternalStorageEncrypted();
    if (ret < 0) {
        return -1;
    }
    *is_encrypted = ret;
    return DPM_ERROR_NONE;
}

int dpm_security_set_external_storage_encryption(device_policy_manager_h handle, bool encrypt)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    return security.setExternalStorageEncryption(encrypt);
}

int dpm_security_is_external_storage_encrypted(device_policy_manager_h handle, bool *is_encrypted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_encrypted, DPM_ERROR_INVALID_PARAMETER);

	DevicePolicyContext &context = GetDevicePolicyContext(handle);
	SecurityPolicy security = context.createPolicyInterface<SecurityPolicy>();
    int ret = security.isExternalStorageEncrypted();
    if (ret < 0) {
        return -1;
    }
    *is_encrypted = ret;
    return DPM_ERROR_NONE;
}

int dpm_security_wipe_data(device_policy_manager_h handle, const dpm_security_wipe_type_e type)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    StoragePolicy storage = client.createPolicyInterface<StoragePolicy>();
    return storage.wipeData(type);
}


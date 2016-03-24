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

#include "capi-assert.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_lockout_device(dpm_client_h handle)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.lockoutDevice();
}

int dpm_lockout_screen(dpm_client_h handle)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.lockoutScreen();
}

int dpm_wipe_data(dpm_client_h handle, const dpm_wipe_type_e type)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.wipeData(type);
}

int dpm_reboot(dpm_client_h handle)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.reboot();
}

int dpm_poweroff_device(dpm_client_h handle)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.powerOffDevice();
}

int dpm_set_internal_storage_encryption(dpm_client_h handle, const int encrypt)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.setInternalStorageEncryption(encrypt);
}

int dpm_is_internal_storage_encrypted(dpm_client_h handle)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.isInternalStorageEncrypted();
}

int dpm_set_external_storage_encryption(dpm_client_h handle, const int encrypt)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.setExternalStorageEncryption(encrypt);
}

int dpm_is_external_storage_encrypted(dpm_client_h handle)
{
    ASSERT(handle, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    return security.isExternalStorageEncrypted();
}


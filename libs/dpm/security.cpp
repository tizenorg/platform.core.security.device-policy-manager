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

#include "security.hxx"

#include "security.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_lockout_device(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.lockoutDevice() == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_lockout_screen(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.lockoutScreen() == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_wipe_data(dpm_client_h handle, const dpm_wipe_type_e type)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.wipeData(type) == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_reboot(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.reboot() == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_poweroff_device(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.powerOffDevice() == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_set_internal_storage_encryption(dpm_client_h handle, const int encrypt)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.setInternalStorageEncryption(encrypt) == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_is_internal_storage_encrypted(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.isInternalStorageEncrypted() == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_set_external_storage_encryption(dpm_client_h handle, const int encrypt)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.setExternalStorageEncryption(encrypt) == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}

int dpm_is_external_storage_encrypted(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Security security = client.createPolicyInterface<Security>();
    if (security.isExternalStorageEncrypted() == 0) {
        return DPM_ERROR_NONE;
    }

    return DPM_ERROR_INVALID;
}


/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "bluetooth.h"
#include "bluetooth.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_bluetooth_policy_h dpm_context_acquire_bluetooth_policy(dpm_context_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<BluetoothPolicy>();
}

int dpm_context_release_bluetooth_policy(dpm_context_h context, dpm_bluetooth_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<BluetoothPolicy>(handle);
    return 0;
}

int dpm_bluetooth_add_device_to_blacklist(dpm_bluetooth_policy_h handle, const char* mac_address)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mac_address, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    return bluetooth.addDeviceToBlacklist(mac_address);
}

int dpm_bluetooth_remove_device_from_blacklist(dpm_bluetooth_policy_h handle, const char* mac_address)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mac_address, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    return bluetooth.removeDeviceFromBlacklist(mac_address);
}

int dpm_bluetooth_set_device_restriction(dpm_bluetooth_policy_h handle, const int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    return bluetooth.setDeviceRestriction(enable);
}

int dpm_bluetooth_is_device_restricted(dpm_bluetooth_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    int ret = bluetooth.isDeviceRestricted();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_bluetooth_add_uuid_to_blacklist(dpm_bluetooth_policy_h handle, const char* uuid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(uuid, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    return bluetooth.addUuidToBlacklist(uuid);
}

int dpm_bluetooth_remove_uuid_from_blacklist(dpm_bluetooth_policy_h handle, const char* uuid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(uuid, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    return bluetooth.removeUuidFromBlacklist(uuid);
}

int dpm_bluetooth_set_uuid_restriction(dpm_bluetooth_policy_h handle, const int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    return bluetooth.setUuidRestriction(enable);
}

int dpm_bluetooth_is_uuid_restricted(dpm_bluetooth_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    BluetoothPolicy& bluetooth = GetPolicyInterface<BluetoothPolicy>(handle);
    int ret = bluetooth.isUuidRestricted();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

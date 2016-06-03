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

EXPORT_API int dpm_bluetooth_add_device_to_blacklist(device_policy_manager_h handle, const char* mac)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mac, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.addDeviceToBlacklist(mac);
}

EXPORT_API int dpm_bluetooth_remove_device_from_blacklist(device_policy_manager_h handle, const char* mac)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mac, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.removeDeviceFromBlacklist(mac);
}

EXPORT_API int dpm_bluetooth_set_device_restriction(device_policy_manager_h handle, bool enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.setDeviceRestriction(enable);
}

EXPORT_API int dpm_bluetooth_is_device_restricted(device_policy_manager_h handle, bool *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    int ret = bluetooth.isDeviceRestricted();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

EXPORT_API int dpm_bluetooth_add_uuid_to_blacklist(device_policy_manager_h handle, const char* uuid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(uuid, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.addUuidToBlacklist(uuid);
}

EXPORT_API int dpm_bluetooth_remove_uuid_from_blacklist(device_policy_manager_h handle, const char* uuid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(uuid, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.removeUuidFromBlacklist(uuid);
}

EXPORT_API int dpm_bluetooth_set_uuid_restriction(device_policy_manager_h handle, bool enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.setUuidRestriction(enable);
}

EXPORT_API int dpm_bluetooth_is_uuid_restricted(device_policy_manager_h handle, bool *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    int ret = bluetooth.isUuidRestricted();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

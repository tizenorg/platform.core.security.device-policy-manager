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

#include "capi-assert.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_bluetooth_add_device_to_blacklist(dpm_client_h handle, const char* mac_address)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mac_address, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.addDeviceToBlacklist(mac_address);
}

int dpm_bluetooth_remove_device_from_blacklist(dpm_client_h handle, const char* mac_address)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(mac_address, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.removeDeviceFromBlacklist(mac_address);
}

int dpm_bluetooth_set_device_restriction(dpm_client_h handle, const int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.setDeviceRestriction(enable);
}

int dpm_bluetooth_is_device_restricted(dpm_client_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.isDeviceRestricted();
}

int dpm_bluetooth_add_uuid_to_blacklist(dpm_client_h handle, const char* uuid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(uuid, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.addUuidToBlacklist(uuid);
}

int dpm_bluetooth_remove_uuid_from_blacklist(dpm_client_h handle, const char* uuid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(uuid, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.removeUuidFromBlacklist(uuid);
}

int dpm_bluetooth_set_uuid_restriction(dpm_client_h handle, const int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.setUuidRestriction(enable);
}

int dpm_bluetooth_is_uuid_restricted(dpm_client_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Bluetooth bluetooth = client.createPolicyInterface<Bluetooth>();
    return bluetooth.isUuidRestricted();
}

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

#include "restriction.h"
#include "restriction.hxx"
#include "location.hxx"
#include "bluetooth.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_restriction_set_camera_state(device_policy_manager_h handle, bool restrict)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	return restriction.setCameraState(restrict);
}

int dpm_restriction_get_camera_state(device_policy_manager_h handle, bool *is_restricted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	int ret = restriction.getCameraState();
    if (ret < 0) {
        return -1;
    }

    *is_restricted = ret;
    return 0;
}

int dpm_restriction_set_microphone_state(device_policy_manager_h handle, bool restrict)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
    return restriction.setMicrophoneState(restrict);
}

int dpm_restriction_get_microphone_state(device_policy_manager_h handle, bool *is_restricted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	int ret = restriction.getMicrophoneState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;
    return 0;
}

int dpm_restriction_set_location_state(device_policy_manager_h handle, bool restrict)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    LocationPolicy location = client.createPolicyInterface<LocationPolicy>();
    return location.setLocationState(restrict);
}

int dpm_restriction_get_location_state(device_policy_manager_h handle, bool *is_restricted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    LocationPolicy location = client.createPolicyInterface<LocationPolicy>();
    int ret = location.getLocationState();
    if (ret < 0) {
        return -1;
    }

    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_usb_debugging_state(device_policy_manager_h handle, bool restrict)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	return restriction.setUsbDebuggingState(restrict);
}

int dpm_restriction_get_usb_debugging_state(device_policy_manager_h handle, bool *is_restricted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	int ret = restriction.getUsbDebuggingState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_usb_tethering_state(device_policy_manager_h handle, bool restrict)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
    return restriction.setUsbTetheringState(restrict);
}

int dpm_restriction_get_usb_tethering_state(device_policy_manager_h handle, bool *is_restricted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
    int ret = restriction.getUsbTetheringState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_settings_changes_state(device_policy_manager_h handle, bool restrict)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	return restriction.setSettingsChangesState(restrict);
}

int dpm_restriction_get_settings_changes_state(device_policy_manager_h handle, bool *is_restricted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	int ret = restriction.getSettingsChangesState();
    if (ret < 0) {
        return -1;
    }

    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_external_storage_state(device_policy_manager_h handle, bool restrict)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	return restriction.setExternalStorageState(restrict);
}

int dpm_restriction_get_external_storage_state(device_policy_manager_h handle, bool *is_restricted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	int ret = restriction.getExternalStorageState();
    if (ret < 0) {
        return 0;
    }
    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_clipboard_state(device_policy_manager_h handle, bool restrict)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	return restriction.setClipboardState(restrict);
}

int dpm_restriction_get_clipboard_state(device_policy_manager_h handle, bool *is_restricted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
	int ret = restriction.getClipboardState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_wifi_state(device_policy_manager_h handle, bool restrict)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
    return restriction.setWifiState(restrict);
}

int dpm_restriction_get_wifi_state(device_policy_manager_h handle, bool *is_restricted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
    int ret = restriction.getWifiState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_wifi_hotspot_state(device_policy_manager_h handle, bool restrict)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
    return restriction.setWifiHotspotState(restrict);
}

int dpm_restriction_get_wifi_hotspot_state(device_policy_manager_h handle, bool *is_restricted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    RestrictionPolicy restriction = client.createPolicyInterface<RestrictionPolicy>();
    int ret = restriction.getWifiHotspotState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_bluetooth_tethering_state(device_policy_manager_h handle, bool restrict)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.setTetheringState(restrict);
}

int dpm_restriction_get_bluetooth_tethering_state(device_policy_manager_h handle, bool *is_restricted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    int ret = bluetooth.getTetheringState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_bluetooth_mode_change_state(device_policy_manager_h handle, bool restrict)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.setModeChangeState(restrict);
}

int dpm_restriction_get_bluetooth_mode_change_state(device_policy_manager_h handle, bool *is_restricted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    int ret = bluetooth.getModeChangeState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;

    return DPM_ERROR_NONE;
}

int dpm_restriction_set_bluetooth_desktop_connectivity_state(device_policy_manager_h handle, bool restrict)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.setDesktopConnectivityState(restrict);
}

int dpm_restriction_get_bluetooth_desktop_connectivity_state(device_policy_manager_h handle, bool *is_restricted)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    int ret = bluetooth.getDesktopConnectivityState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;

    return DPM_ERROR_NONE;
}

int dpm_restriction_set_bluetooth_pairing_state(device_policy_manager_h handle, bool restrict)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    return bluetooth.setPairingState(restrict);
}

int dpm_restriction_get_bluetooth_pairing_state(device_policy_manager_h handle, bool *is_restricted)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(is_restricted, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    BluetoothPolicy bluetooth = client.createPolicyInterface<BluetoothPolicy>();
    int ret = bluetooth.getPairingState();
    if (ret < 0) {
        return -1;
    }
    *is_restricted = ret;

    return DPM_ERROR_NONE;
}

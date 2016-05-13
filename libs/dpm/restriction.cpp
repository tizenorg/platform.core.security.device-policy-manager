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

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_restriction_policy_h dpm_context_acquire_restriction_policy(dpm_context_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<RestrictionPolicy>();
}

int dpm_context_release_restriction_policy(dpm_context_h context, dpm_restriction_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<RestrictionPolicy>(handle);
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_camera_state(dpm_restriction_policy_h handle, int enable)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restrict = GetPolicyInterface<RestrictionPolicy>(handle);
	return restrict.setCameraState(enable);
}

int dpm_restriction_get_camera_state(dpm_restriction_policy_h handle, int *state)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	int ret = restriction.getCameraState();
    if (ret < 0) {
        return -1;
    }

    *state = ret;
    return 0;
}

int dpm_restriction_set_microphone_state(dpm_restriction_policy_h handle, int enable)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
         return restriction.setMicrophoneState(enable);
}

int dpm_restriction_get_microphone_state(dpm_restriction_policy_h handle, int *state)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	int ret = restriction.getMicrophoneState();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return 0;
}

int dpm_restriction_set_location_state(dpm_restriction_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    return restriction.setLocationState(enable);
}

int dpm_restriction_get_location_state(dpm_restriction_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    int ret = restriction.getLocationState();
    if (ret < 0) {
        return -1;
    }

    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_usb_debugging_state(dpm_restriction_policy_h handle, int enable)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setUsbDebuggingState(enable);
}

int dpm_restriction_get_usb_debugging_state(dpm_restriction_policy_h handle, int *state)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	int ret = restriction.getUsbDebuggingState();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_settings_changes_state(dpm_restriction_policy_h handle, int enable)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setSettingsChangesState(enable);
}

int dpm_restriction_get_settings_changes_state(dpm_restriction_policy_h handle, int *state)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	int ret = restriction.getSettingsChangesState();
    if (ret < 0) {
        return -1;
    }

    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_external_storage_state(dpm_restriction_policy_h handle, int enable)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setExternalStorageState(enable);
}

int dpm_restriction_get_external_storage_state(dpm_restriction_policy_h handle, int *state)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	int ret = restriction.getExternalStorageState();
    if (ret < 0) {
        return 0;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_clipboard_state(dpm_restriction_policy_h handle, int enable)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setClipboardState(enable);
}

int dpm_restriction_get_clipboard_state(dpm_restriction_policy_h handle, int *state)
{
	RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	int ret = restriction.getClipboardState();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_wifi_state(dpm_restriction_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    return restriction.setWifiState(enable);
}

int dpm_restriction_get_wifi_state(dpm_restriction_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    int ret = restriction.getWifiState();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_wifi_hotspot_state(dpm_restriction_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    return restriction.setWifiHotspotState(enable);
}

int dpm_restriction_get_wifi_hotspot_state(dpm_restriction_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    int ret = restriction.getWifiHotspotState();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

int dpm_restriction_set_bluetooth_tethering_state(dpm_restriction_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    return restriction.setBluetoothTetheringState(enable);
}

int dpm_restriction_get_bluetooth_tethering_state(dpm_restriction_policy_h handle, int *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(state, DPM_ERROR_INVALID_PARAMETER);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    int ret = restriction.getBluetoothTetheringState();
    if (ret < 0) {
        return -1;
    }
    *state = ret;
    return DPM_ERROR_NONE;
}

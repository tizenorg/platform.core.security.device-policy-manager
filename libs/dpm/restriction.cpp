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

#include <cassert>

#include "restriction.h"
#include "restriction.hxx"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_restriction_policy_h dpm_context_acquire_restriction_policy(dpm_context_h handle)
{
    assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<RestrictionPolicy>();
}

int dpm_context_release_restriction_policy(dpm_context_h context, dpm_restriction_policy_h handle)
{
    assert(handle);

    delete &GetPolicyInterface<RestrictionPolicy>(handle);
    return 0;
}

int dpm_restriction_set_camera_state(dpm_restriction_policy_h handle, int enable)
{
	assert(handle);

	RestrictionPolicy& restrict = GetPolicyInterface<RestrictionPolicy>(handle);
	return restrict.setCameraState(enable);
}

int dpm_restriction_get_camera_state(dpm_restriction_policy_h handle)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.getCameraState();
}

int dpm_restriction_set_microphone_state(dpm_restriction_policy_h handle, int enable)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setMicrophoneState(enable);
}

int dpm_restriction_get_microphone_state(dpm_restriction_policy_h handle)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.getMicrophoneState();
}

int dpm_restriction_set_location_state(dpm_restriction_policy_h handle, int enable)
{
    assert(handle);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    return restriction.setLocationState(enable);
}

int dpm_restriction_get_location_state(dpm_restriction_policy_h handle)
{
    assert(handle);

    RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
    return restriction.getLocationState();
}

int dpm_restriction_set_usb_debugging_state(dpm_restriction_policy_h handle, int enable)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setUsbDebuggingState(enable);
}

int dpm_restriction_get_usb_debugging_state(dpm_restriction_policy_h handle)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.getUsbDebuggingState();
}

int dpm_restriction_set_settings_change_state(dpm_restriction_policy_h handle, int enable)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setSettingsChangesState(enable);
}

int dpm_restriction_get_settings_change_state(dpm_restriction_policy_h handle)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.getSettingsChangesState();
}

int dpm_restriction_set_external_storage_state(dpm_restriction_policy_h handle, int enable)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.setExternalStorageState(enable);
}

int dpm_restriction_get_external_storage_state(dpm_restriction_policy_h handle)
{
	assert(handle);

	RestrictionPolicy& restriction = GetPolicyInterface<RestrictionPolicy>(handle);
	return restriction.getExternalStorageState();
}

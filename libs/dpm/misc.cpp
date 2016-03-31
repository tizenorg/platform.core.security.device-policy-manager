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

#include "misc.h"
#include "misc.hxx"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_set_camera_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.setCameraRestriction(enable);
}

int dpm_is_camera_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isCameraRestricted();
}

int dpm_set_microphone_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.setMicrophoneRestriction(enable);
}

int dpm_is_microphone_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isMicrophoneRestricted();
}

int dpm_set_location_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.setLocationRestriction(enable);
}

int dpm_is_location_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isLocationRestricted();
}

int dpm_set_sd_card_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.setSdCardRestriction(enable);
}

int dpm_is_sd_card_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isSdCardRestricted();
}

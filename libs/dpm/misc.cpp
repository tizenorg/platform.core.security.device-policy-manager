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

int dpm_allow_camera(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.allowCamera(allow);
}

int dpm_is_camera_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isCameraAllowed();
}

int dpm_allow_microphone(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.allowMicrophone(allow);
}

int dpm_is_microphone_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isMicrophoneAllowed();
}

int dpm_allow_location(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.allowLocation(allow);
}

int dpm_is_location_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isLocationAllowed();
}

int dpm_allow_sd_card(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.allowSdCard(allow);
}

int dpm_is_sd_card_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient &client = GetDevicePolicyClient(handle);
	Misc misc = client.createPolicyInterface<Misc>();
	return misc.isSdCardAllowed();
}

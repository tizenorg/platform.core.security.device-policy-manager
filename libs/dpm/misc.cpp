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

int dpm_location_allow_state_change(dpm_location_policy_h handle, int enable)
{
	assert(handle);

	Misc& misc = GetPolicyInterface<Misc>(handle);
	return misc.setLocationRestriction(enable);
}

int dpm_location_is_state_change_allowed(dpm_location_policy_h handle)
{
	assert(handle);

	Misc& misc = GetPolicyInterface<Misc>(handle);
	return misc.isLocationRestricted();
}

int dpm_storage_set_sdcard_restriction(dpm_storage_policy_h handle, int enable)
{
	assert(handle);

	Misc& misc = GetPolicyInterface<Misc>(handle);
	return misc.setSdCardRestriction(enable);
}

int dpm_storage_is_sdcard_restricted(dpm_storage_policy_h handle)
{
	assert(handle);

	Misc& misc = GetPolicyInterface<Misc>(handle);
	return misc.isSdCardRestricted();
}

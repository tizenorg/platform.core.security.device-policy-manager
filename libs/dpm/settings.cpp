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

#include "settings.h"
#include "settings.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_settings_policy_h dpm_context_acquire_settings_policy(dpm_context_h handle)
{
	assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<SettingsPolicy>();
}

int dpm_context_release_settings_policy(dpm_settings_policy_h handle)
{
	assert(handle);
    delete &GetPolicyInterface<SettingsPolicy>(handle);
    return 0;
}

int dpm_settings_set_restriction(dpm_settings_policy_h handle, int enable)
{
	assert(handle);

	SettingsPolicy& settingsPolicy = GetPolicyInterface<SettingsPolicy>(handle);
	return settingsPolicy.setSettingsChangesRestriction(enable);
}

int dpm_settings_is_restricted(dpm_settings_policy_h handle)
{
	assert(handle);

	SettingsPolicy& settingsPolicy = GetPolicyInterface<SettingsPolicy>(handle);
	return settingsPolicy.isSettingsChangesRestricted();
}
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

int dpm_clipboard_set_state(dpm_context_h handle, int enable)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setClipboardRestriction(enable);
}

int dpm_clipboard_get_state(dpm_context_h handle)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isClipboardRestricted();
}

int dpm_settings_set_restriction(dpm_context_h handle, int enable)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setSettingsChangesRestriction(enable);
}

int dpm_settings_is_restricted(dpm_context_h handle)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isSettingsChangesRestricted();
}

int dpm_developer_set_usb_debugging_state(dpm_context_h handle, int enable)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setUsbDebuggingRestriction(enable);
}

int dpm_developer_get_usb_debugging_state(dpm_context_h handle)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isUsbDebuggingRestricted();
}

int dpm_storage_set_usb_mass_storage_state(dpm_context_h handle, int enable)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setUsbMassStorageRestriction(enable);
}

int dpm_storage_get_usb_mass_storage_state(dpm_context_h handle)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isUsbMassStorageRestricted();
}

int dpm_device_wipe_data(dpm_context_h handle)
{
	assert(handle);

	DevicePolicyContext& client = GetDevicePolicyContext(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.wipeData();
}

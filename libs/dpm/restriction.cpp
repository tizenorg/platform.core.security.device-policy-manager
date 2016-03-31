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

int dpm_set_clipboard_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setClipboardRestriction(enable);
}

int dpm_is_clipboard_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isClipboardRestricted();
}

int dpm_set_settings_changes_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setSettingsChangesRestriction(enable);
}

int dpm_is_settings_changes_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isSettingsChangesRestricted();
}

int dpm_set_background_data_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setBackgroundDataRestriction(enable);
}

int dpm_is_background_data_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isBackgroundDataRestricted();
}

int dpm_set_usb_debugging_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setUsbDebuggingRestriction(enable);
}

int dpm_is_usb_debugging_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isUsbDebuggingRestricted();
}

int dpm_set_usb_mass_storage_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setUsbMassStorageRestriction(enable);
}

int dpm_is_usb_mass_storage_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isUsbMassStorageRestricted();
}

int dpm_set_factory_reset_restriction(dpm_client_h handle, int enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setFactoryResetRestriction(enable);
}

int dpm_is_factory_reset_restricted(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isFactoryResetRestricted();
}

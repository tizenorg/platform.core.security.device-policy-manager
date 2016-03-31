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

int dpm_allow_clipboard(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowClipboard(allow);
}

int dpm_is_clipboard_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isClipboardAllowed();
}

int dpm_allow_settings_changes(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowSettingsChanges(allow);
}

int dpm_is_settings_changes_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isSettingsChangesAllowed();
}

int dpm_allow_background_data(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowBackgroundData(allow);
}

int dpm_is_background_data_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isBackgroundDataAllowed();
}

int dpm_allow_usb_debugging(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowUsbDebugging(allow);
}

int dpm_is_usb_debugging_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isUsbDebuggingAllowed();
}

int dpm_allow_usb_mass_storage(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowUsbMassStorage(allow);
}

int dpm_is_usb_mass_storage_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isUsbMassStorageAllowed();
}

int dpm_allow_factory_reset(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowFactoryReset(allow);
}

int dpm_is_factory_reset_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isFactoryResetAllowed();
}

int dpm_allow_bluetooth_tethering(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowBluetoothTethering(allow);
}

int dpm_is_bluetooth_tethering_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isBluetoothTetheringAllowed();
}

int dpm_allow_mock_location(dpm_client_h handle, int allow)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.allowMockLocation(allow);
}

int dpm_is_mock_location_allowed(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.isMockLocationAllowed();
}

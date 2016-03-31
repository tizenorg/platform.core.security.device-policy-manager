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

int dpm_set_allow_clipboard(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowClipboard(enable);
}

bool dpm_get_allow_clipboard(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowClipboard();
}

int dpm_set_allow_settings_changes(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowSettingsChanges(enable);
}

bool dpm_get_allow_settings_changes(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowSettingsChanges();
}

int dpm_set_allow_background_data(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowBackgroundData(enable);
}

bool dpm_get_allow_background_data(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowBackgroundData();
}

int dpm_set_allow_usb_debugging(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowUsbDebugging(enable);
}

bool dpm_get_allow_usb_debugging(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowUsbDebugging();
}

int dpm_set_allow_usb_mass_storage(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowUsbMassStorage(enable);
}

bool dpm_get_allow_usb_mass_storage(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowUsbMassStorage();
}

int dpm_set_allow_factory_reset(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowFactoryReset(enable);
}

bool dpm_get_allow_factory_reset(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowFactoryReset();
}

int dpm_set_allow_bluetooth_tethering(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowBluetoothTethering(enable);
}

bool dpm_get_allow_bluetooth_tethering(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowBluetoothTethering();
}

int dpm_set_allow_mock_location(dpm_client_h handle, const bool enable)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.setAllowMockLocation(enable);
}

bool dpm_get_allow_mock_location(dpm_client_h handle)
{
	assert(handle);

	DevicePolicyClient& client = GetDevicePolicyClient(handle);
	Restriction restriction = client.createPolicyInterface<Restriction>();
	return restriction.getAllowMockLocation();
}

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

#include "storage.h"
#include "storage.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_storage_set_usb_mass_storage_state(dpm_storage_policy_h handle, int enable)
{
	assert(handle);

	StoragePolicy& storagePolicy = GetPolicyInterface<StoragePolicy>(handle);
	return storagePolicy.setUsbMassStorageRestriction(enable);
}

int dpm_storage_get_usb_mass_storage_state(dpm_storage_policy_h handle)
{
	assert(handle);

	StoragePolicy& storagePolicy = GetPolicyInterface<StoragePolicy>(handle);
	return storagePolicy.isUsbMassStorageRestricted();
}

int dpm_device_wipe_data(dpm_storage_policy_h handle)
{
	assert(handle);

	StoragePolicy& storagePolicy = GetPolicyInterface<StoragePolicy>(handle);
	return storagePolicy.wipeData();
}

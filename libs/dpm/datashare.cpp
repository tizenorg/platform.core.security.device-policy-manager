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

#include "datashare.h"
#include "datashare.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;


dpm_datashare_policy_h dpm_context_acquire_datashare_policy(dpm_context_h handle)
{
	assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<DataSharePolicy>();
}

int dpm_context_release_datashare_policy(dpm_datashare_policy_h handle)
{
	assert(handle);
    delete &GetPolicyInterface<DataSharePolicy>(handle);
    return 0;
}

int dpm_datashare_set_clipboard_state(dpm_datashare_policy_h handle, int enable)
{
	assert(handle);

	DataSharePolicy& dataSharePolicy = GetPolicyInterface<DataSharePolicy>(handle);
	return dataSharePolicy.setClipboardState(enable);
}

int dpm_datashare_get_clipboard_state(dpm_datashare_policy_h handle)
{
	assert(handle);

	DataSharePolicy& dataSharePolicy = GetPolicyInterface<DataSharePolicy>(handle);
	return dataSharePolicy.getClipboardState();
}
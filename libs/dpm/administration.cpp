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

#include <cstring>

#include "administration.h"
#include "administration.hxx"

#include "capi-assert.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_admin_register_client(dpm_admin_policy_h handle, const char* name)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Administration admin = client.createPolicyInterface<Administration>();
    return admin.registerPolicyClient(name);
}

int dpm_admin_deregister_client(dpm_admin_policy_h handle, const char* name)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Administration admin = client.createPolicyInterface<Administration>();
    return admin.deregisterPolicyClient(name);
}

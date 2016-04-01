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
#include <iostream>

#include "device-policy-client.h"

#include "policy-client.h"

#include "exception.h"

DevicePolicyContext& GetDevicePolicyContext(void* handle)
{
    assert(handle);
    return *reinterpret_cast<DevicePolicyContext*>(handle);
}

DPM_API dpm_context_h dpm_create_context(void)
{
    DevicePolicyContext *client = new(std::nothrow) DevicePolicyContext();

    assert(client);

    if (client->connect() < 0) {
        delete client;
        return NULL;
    }

    return reinterpret_cast<dpm_context_h>(client);
}

DPM_API void dpm_destroy_context(dpm_context_h handle)
{
    assert(handle);

    delete &GetDevicePolicyContext(handle);
}

DPM_API int dpm_add_policy_change_listener(dpm_context_h handle, const char* name, dpm_policy_change_cb handler, void* user_data)
{
    assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.subscribePolicyChange(name, handler, user_data);
}

DPM_API void dpm_remove_policy_change_listener(dpm_context_h handle, int id)
{
    assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    client.unsubscribePolicyChange(id);
}

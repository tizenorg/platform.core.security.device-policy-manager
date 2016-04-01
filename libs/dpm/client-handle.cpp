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

dpm_context_h dpm_context_create(void)
{
    DevicePolicyContext *client = new(std::nothrow) DevicePolicyContext();

    assert(client);

    if (client->connect() < 0) {
        delete client;
        return NULL;
    }

    return reinterpret_cast<dpm_context_h>(client);
}

int dpm_context_destroy(dpm_context_h handle)
{
    assert(handle);

    delete &GetDevicePolicyContext(handle);

    return 0;
}

int dpm_add_policy_change_listener(dpm_context_h handle, const char* name, dpm_policy_changed_cb handler, void* user_data)
{
    assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.subscribePolicyChange(name, handler, user_data);
}

void dpm_remove_policy_change_listener(dpm_context_h handle, int id)
{
    assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    client.unsubscribePolicyChange(id);
}

int dpm_context_add_signal_cb(dpm_context_h handle, const char* signal, dpm_signal_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(signal, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& context = GetDevicePolicyContext(handle);
    return context.subscribeSignal(signal, callback, user_data);
}

int dpm_context_remove_signal_cb(dpm_context_h context, int callback_id)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback_id >= 0, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext& context = GetDevicePolicyContext(handle);
    return context.unsubscribeSignal(callback_id);
}

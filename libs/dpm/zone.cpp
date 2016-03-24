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

#include "zone.h"
#include "zone.hxx"

#include "array.h"
#include "capi-assert.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_create_zone(dpm_client_h handle, const char* name, const char* pkgid)
{
    ASSERT(handle, DPM_ERROR_INVALID);
    ASSERT(name, DPM_ERROR_INVALID);
    ASSERT(pkgid, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();
    return zone.create(name, pkgid);
}

int dpm_remove_zone(dpm_client_h handle, const char* name)
{
    ASSERT(handle, DPM_ERROR_INVALID);
    ASSERT(name, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();
    return zone.remove(name);
}

typedef runtime::Array<std::string> dpm_zone_iterator;

dpm_zone_iterator_h dpm_get_zone_iterator(dpm_client_h handle)
{
    ASSERT(handle, NULL);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();

    return reinterpret_cast<dpm_zone_iterator_h>(new dpm_zone_iterator(zone.getList()));
}

const char* dpm_zone_iterator_next(dpm_zone_iterator_h iter)
{
    ASSERT(iter, NULL);

    std::string* result = reinterpret_cast<dpm_zone_iterator*>(iter)->next();

    ASSERT(result, NULL);

    return result->c_str();
}

void dpm_free_zone_iterator(dpm_zone_iterator_h iter)
{
    ASSERT(iter, void());

    delete reinterpret_cast<dpm_zone_iterator*>(iter);
}

int dpm_get_zone_state(dpm_client_h handle, const char* name)
{
    ASSERT(handle, DPM_ERROR_INVALID);
    ASSERT(name, DPM_ERROR_INVALID);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();

    /* TODO : should implement */

    return DPM_ERROR_INVALID;
}

int dpm_subscribe_zone_signal(dpm_client_h handle, dpm_zone_signal_cb callback, void* user_data)
{
    ASSERT(handle, DPM_ERROR_INVALID);
    ASSERT(callback, DPM_ERROR_INVALID);

    /* TODO : should implement */

    return DPM_ERROR_NOT_SUPPORTED;
}

int dpm_unsubscribe_zone_signal(dpm_client_h handle, dpm_zone_signal_cb callback)
{
    ASSERT(handle, DPM_ERROR_INVALID);
    ASSERT(callback, DPM_ERROR_INVALID);

    /* TODO : should implement */

    return DPM_ERROR_NOT_SUPPORTED;
}

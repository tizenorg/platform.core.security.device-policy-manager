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

#include <dpm/zone.h>

#include "dpm.hxx"
#include "zone.hxx"

using namespace DevicePolicyManager;

int dpm_create_zone(dpm_client_h handle, const char* name, const char* pkgid)
{
    assert(handle);
    assert(name);
    assert(pkgid);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();
    return zone.create(name, pkgid);
}

int dpm_remove_zone(dpm_client_h handle, const char* name)
{
    assert(handle);
    assert(name);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();
    return zone.remove(name);
}

struct dpm_zone_iterator_s {
    std::vector<std::string> list;
    std::vector<std::string>::iterator iter;
};

dpm_zone_iterator_h dpm_get_zone_iterator(dpm_client_h handle)
{
    assert(handle);
    assert(iter);

    dpm_zone_iterator_h iter;
    iter = new struct dpm_zone_iterator_s;

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();
        
    iter->list = zone.getList();
    iter->iter = iter->list.begin();

    return iter;
}

const char* dpm_zone_iterator_next(dpm_zone_iterator_h iter)
{
    assert(iter);
    return (*(iter->iter++)).c_str();
}

void dpm_free_zone_iterator(dpm_zone_iterator_h iter)
{
    assert(iter);
    delete iter;
}

zone_state_e dpm_get_zone_state(dpm_client_h handle, const char *name)
{
    assert(handle);
    assert(name);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Zone zone = client.createPolicyInterface<Zone>();

    /* TODO : should implement */

    return DPM_ZONE_DOESNT_EXIST;
}

int dpm_subscribe_zone_signal(dpm_client_h handle, dpm_zone_signal_cb callback, void* user_data)
{
    assert(handle);
    assert(callback);

    /* TODO : should implement */

    return DPM_ERROR_NOT_SUPPORTED;
}

int dpm_unsubscribe_zone_signal(dpm_client_h handle, dpm_zone_signal_cb callback)
{
    assert(handle);
    assert(callback);

    /* TODO : should implement */

    return DPM_ERROR_NOT_SUPPORTED;
}

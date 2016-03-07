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

#include <dpm/container.h>

#include "dpm.hxx"
#include "container.hxx"

using namespace DevicePolicyManager;

typedef struct container_list_iterator_s* container_list_iterator_h;

int dpm_create_container(dpm_client_h handle, const char* name, const char* pkgid)
{
    assert(handle);
    assert(name);
    assert(pkgid);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Container container = client.createPolicyInterface<Container>();
    return container.create(name, pkgid);
}

int dpm_remove_container(dpm_client_h handle, const char* name)
{
    assert(handle);
    assert(name);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Container container = client.createPolicyInterface<Container>();
    return container.remove(name);
}

int dpm_lock_container(dpm_client_h handle, const char* name)
{
    assert(handle);
    assert(name);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Container container = client.createPolicyInterface<Container>();
    return container.lock(name);
}

int dpm_unlock_container(dpm_client_h handle, const char* name)
{
    assert(handle);
    assert(name);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Container container = client.createPolicyInterface<Container>();
    return container.unlock(name);
}

struct dpm_container_list_iterator_s {
    std::vector<std::string> list;
    std::vector<std::string>::iterator iter;
};

dpm_container_list_iterator_h dpm_container_list_get_iterator(dpm_client_h handle)
{
    assert(handle);
    assert(iter);

    dpm_container_list_iterator_h iter;
    iter = new struct dpm_container_list_iterator_s;

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Container container = client.createPolicyInterface<Container>();
        
    iter->list = container.getList();
    iter->iter = iter->list.begin();

    return iter;
}

const char* dpm_container_iterator_next(dpm_container_list_iterator_h iter)
{
    assert(iter);
    return (*(iter->iter++)).c_str();
}

void dpm_container_free_iterator(dpm_container_list_iterator_h iter)
{
    assert(iter);
    delete iter;
}

container_state_e dpm_container_get_state(dpm_client_h handle, const char *name)
{
    assert(handle);
    assert(name);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Container container = client.createPolicyInterface<Container>();
//    return container.getState(name);
    return DPM_CONTAINER_DOESNT_EXIST;
}

int dpm_add_container_provisioning_listener(dpm_client_h handle, dpm_container_provisioning_cb callback, void* user_data)
{
    return 0;
}

int dpm_remove_container_provisioning_listener(dpm_client_h handle, dpm_container_provisioning_cb callback)
{
    return 0;
}

int dpm_add_container_state_listener(dpm_client_h handle, dpm_container_provisioning_cb callback, void* user_data)
{
    return 0;
}

int dpm_remove_state_provisioning_listener(dpm_client_h handle, dpm_container_provisioning_cb callback)
{
    return 0;
}

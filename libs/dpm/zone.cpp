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

dpm_zone_policy_h dpm_context_acquire_zone_policy(dpm_context_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<Zone>();
}

int dpm_context_release_zone_policy(dpm_zone_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<Zone>(handle);
    return 0;
}

int dpm_create_zone(dpm_zone_policy_h handle, const char* name, const char* pkgid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    Zone& zone = GetPolicyInterface<Zone>(handle);
    return zone.createZone(name, pkgid);
}

int dpm_remove_zone(dpm_zone_policy_h handle, const char* name)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    Zone& zone = GetPolicyInterface<Zone>(handle);
    return zone.removeZone(name);
}

typedef runtime::Array<std::string> dpm_zone_iterator;

dpm_zone_iterator_h dpm_get_zone_iterator(dpm_zone_policy_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    Zone& zone = GetPolicyInterface<Zone>(handle);

    return reinterpret_cast<dpm_zone_iterator_h>(new dpm_zone_iterator(zone.getZoneList()));
}

const char* dpm_zone_iterator_next(dpm_zone_iterator_h iter)
{
    RET_ON_FAILURE(iter, NULL);

    std::string* result = reinterpret_cast<dpm_zone_iterator*>(iter)->next();

    RET_ON_FAILURE(result, NULL);

    return result->c_str();
}

void dpm_free_zone_iterator(dpm_zone_iterator_h iter)
{
    RET_ON_FAILURE(iter, void());

    delete reinterpret_cast<dpm_zone_iterator*>(iter);
}

int dpm_get_zone_state(dpm_zone_policy_h handle, const char* name)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    //Zone& zone = GetPolicyInterface<Zone>(handle);

    /* TODO : should implement */

    return DPM_ERROR_INVALID_PARAMETER;
}

int dpm_subscribe_zone_signal(dpm_zone_policy_h handle, const char* signal, dpm_zone_signal_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(signal, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    /*
    std::string sigName = "Zone::";
    sigName += signal;

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.subscribeSignal(sigName, callback, user_data);
    */
    return 0;
}

int dpm_unsubscribe_zone_signal(dpm_zone_policy_h handle, int callback_id)
{
    /*
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback_id >= 0, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.unsubscribeSignal(callback_id);
    */
    return 0;
}

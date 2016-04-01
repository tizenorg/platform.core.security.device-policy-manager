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
#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_zone_policy_h dpm_context_acquire_zone_policy(dpm_context_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<ZonePolicy>();
}

int dpm_context_release_zone_policy(dpm_zone_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<ZonePolicy>(handle);
    return 0;
}

int dpm_zone_create(dpm_zone_policy_h handle, const char* name, const char* pkgname)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgname, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.createZone(name, pkgname);
}

int dpm_zone_destroy(dpm_zone_policy_h handle, const char* name)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.removeZone(name);
}

typedef runtime::Array<std::string> dpm_zone_iterator;

dpm_zone_iterator_h dpm_create_zone_iterator(dpm_zone_policy_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);

    return reinterpret_cast<dpm_zone_iterator_h>(new dpm_zone_iterator(zone.getZoneList()));
}

int dpm_zone_iterator_next(dpm_zone_iterator_h iter, const char ** result)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    *result = reinterpret_cast<dpm_zone_iterator*>(iter)->next()->c_str();

    RET_ON_FAILURE(*result, DPM_ERROR_INVALID_PARAMETER);

    return 0;
}

void dpm_zone_free_iterator(dpm_zone_iterator_h iter)
{
    RET_ON_FAILURE(iter, void());

    delete reinterpret_cast<dpm_zone_iterator*>(iter);
}

int dpm_zone_get_state(dpm_zone_policy_h handle, const char* name, dpm_zone_state_e *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    //ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);

    /* TODO : should implement */

    return DPM_ERROR_INVALID_PARAMETER;
}

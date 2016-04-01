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

#include "geofence.h"
#include "geofence.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_geofence_policy_h dpm_context_acquire_geofence_policy(dpm_context_h handle)
{
    assert(handle);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<GeofencePolicy>();
}

int dpm_context_release_geofence_policy(dpm_geofence_policy_h handle)
{
    assert(handle);

    delete &GetPolicyInterface<GeofencePolicy>(handle);
    return 0;
}

int dpm_geofence_allow_state_change(dpm_geofence_policy_h handle, int enable)
{
    assert(handle);

    GeofencePolicy& geofence = GetPolicyInterface<GeofencePolicy>(handle);
    return geofence.allowStateChange(enable);
}

int dpm_geofence_is_state_change_allowed(dpm_geofence_policy_h handle)
{
    assert(handle);

    GeofencePolicy& geofence = GetPolicyInterface<GeofencePolicy>(handle);
    return geofence.isStateChangeAllowed();
}

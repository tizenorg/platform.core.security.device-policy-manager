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

EXPORT_API int dpm_zone_create(device_policy_manager_h handle, const char* name, const char* pkgname)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgname, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    ZonePolicy zone = client.createPolicyInterface<ZonePolicy>();
    return zone.createZone(name, pkgname);
}

EXPORT_API int dpm_zone_destroy(device_policy_manager_h handle, const char* name)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    ZonePolicy zone = client.createPolicyInterface<ZonePolicy>();
    return zone.removeZone(name);
}

EXPORT_API int dpm_zone_get_state(device_policy_manager_h handle, const char* name, dpm_zone_state_e *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    ZonePolicy zone = client.createPolicyInterface<ZonePolicy>();

    int result = zone.getZoneState(name);
    if (result <0) {
        return DPM_ERROR_NO_DATA;
    }

    *state = (dpm_zone_state_e)result;
    return DPM_ERROR_NONE;
}

EXPORT_API int dpm_zone_foreach_name(device_policy_manager_h handle, dpm_zone_state_e state,
                                     dpm_zone_foreach_name_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    ZonePolicy zone = client.createPolicyInterface<ZonePolicy>();
    std::vector<std::string> list = zone.getZoneList(state);
    for (const std::string& name : list) {
        callback(name.c_str(), user_data);
    }

    return DPM_ERROR_NONE;
}

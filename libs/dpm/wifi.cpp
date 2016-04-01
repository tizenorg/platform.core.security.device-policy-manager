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

#include "wifi.h"
#include "wifi.hxx"

#include "capi-assert.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_wifi_policy_h dpm_context_acquire_wifi_policy(dpm_context_h handle, const char* zone)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<Wifi>();
}

int dpm_context_release_wifi_policy(dpm_wifi_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<Wifi>(handle);
    return 0;
}

int dpm_wifi_allow_state_change(dpm_wifi_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    return wifi.setStateChangeRestriction(enable);
}

int dpm_wifi_is_state_change_allowed(dpm_wifi_policy_h handle, int *enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(enable, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    *enable = wifi.isStateChangeRestricted();
    return DPM_ERROR_NONE;
}

int dpm_wifi_allow_settings_change(dpm_wifi_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    return wifi.setSettingChangesRestriction(enable);
}

int dpm_wifi_is_settings_change_allowed(dpm_wifi_policy_h handle, int *enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(enable, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    *enable = wifi.isSettingChangesRestricted();
    return DPM_ERROR_NONE;
}

int dpm_wifi_allow_hotspot_state_change(dpm_wifi_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    return wifi.setHotspotRestriction(enable);
}

int dpm_wifi_is_hotspot_state_change_allowed(dpm_wifi_policy_h handle, int *enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(enable, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    *enable = wifi.isHotspotRestricted();
    return DPM_ERROR_NONE;
}

int dpm_wifi_set_network_access_restriction(dpm_wifi_policy_h handle, int enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    return wifi.setNetworkAccessRestriction(enable);
}

int dpm_wifi_is_network_access_restricted(dpm_wifi_policy_h handle, int *enable)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(enable, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    *enable = wifi.isNetworkAccessRestricted();
    return DPM_ERROR_NONE;
}

int dpm_wifi_add_ssid_to_blocklist(dpm_wifi_policy_h handle, const char* ssid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(ssid, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    return wifi.addSsidFromBlacklist(ssid);
}

int dpm_wifi_remove_ssid_from_blocklist(dpm_wifi_policy_h handle, const char* ssid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(ssid, DPM_ERROR_INVALID_PARAMETER);

    Wifi& wifi = GetPolicyInterface<Wifi>(handle);
    return wifi.removeSsidFromBlacklist(ssid);
}

int dpm_wifi_clear_ssids_in_blocklist(dpm_wifi_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    return -1;
}

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

int dpm_set_wifi_state_change_restriction(dpm_client_h handle, int enable)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.setStateChangeRestriction(enable);
}

int dpm_is_wifi_state_change_restricted(dpm_client_h handle)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.isStateChangeRestricted();
}

int dpm_set_wifi_setting_changes_restriction(dpm_client_h handle, int enable)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.setSettingChangesRestriction(enable);
}

int dpm_is_wifi_setting_changes_restricted(dpm_client_h handle)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.isSettingChangesRestricted();
}

int dpm_allow_wifi_hotspot_restriction(dpm_client_h handle, int enable)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.setHotspotRestriction(enable);
}

int dpm_is_wifi_hotpot_restricted(dpm_client_h handle)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.isHotspotRestricted();
}

int dpm_set_wifi_network_access_restriction(dpm_client_h handle, int enable)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.setNetworkAccessRestriction(enable);
}

int dpm_is_wifi_network_access_restricted(dpm_client_h handle)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.isNetworkAccessRestricted();
}

int dpm_add_wifi_ssid_to_blocklist(dpm_client_h handle, const char* ssid)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.addSsidFromBlacklist(ssid);
}

int dpm_remove_wifi_ssid_from_blocklist(dpm_client_h handle, const char* ssid)
{
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Wifi wifi = client.createPolicyInterface<Wifi>();
    return wifi.removeSsidFromBlacklist(ssid);
}

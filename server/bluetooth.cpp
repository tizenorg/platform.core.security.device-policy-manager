/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <bluetooth.h>
#include <bluetooth_internal.h>
#include <bluetooth-api.h>

#include "bluetooth.hxx"

#include "policy-helper.h"
#include "audit/logger.h"

namespace {

int isBluetoothEnabled()
{
    if (bt_initialize() != BT_ERROR_NONE)
        return -1;
    bt_adapter_state_e state = BT_ADAPTER_DISABLED;
    if (bt_adapter_get_state(&state) != BT_ERROR_NONE)
        return -1;
    if (bt_deinitialize() != BT_ERROR_NONE)
        return -1;
    return state == BT_ADAPTER_ENABLED ? 1 : 0;
}

void bluetoothAdapterStateChangedCb(int result, bt_adapter_state_e state, void *user_data)
{
    ERROR("bt adapter state changed callback");
    ERROR("state : " + std::string(state == BT_ADAPTER_ENABLED ? "enabled" : "disabled"));
}

} // namespace

namespace DevicePolicyManager {

BluetoothPolicy::BluetoothPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
    // for restriction CPIs
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setModeChangeState)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::getModeChangeState));
    // for bluetooth CPIs
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::addDeviceToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::removeDeviceFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setDeviceRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::isDeviceRestricted));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::addUuidToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::removeUuidFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setUuidRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::isUuidRestricted));

    ctxt.createNotification("bluetooth");
    ctxt.createNotification("bluetooth-uuid-restriction");
    ctxt.createNotification("bluetooth-device-restriction");

    // callback
    int ret = bt_initialize();
    std::cout << "SHONG_DEBUG: ret: " << std::to_string(ret) << '\n';
    if (ret == BT_ERROR_NONE) {
        ret = bt_adapter_set_state_changed_cb(::bluetoothAdapterStateChangedCb, NULL);
        std::cout << "SHONG_DEBUG: ret: " << std::to_string(ret) << '\n';
    }
    // ERROR("initailze: ret = " + ret);
}

BluetoothPolicy::~BluetoothPolicy()
{
}

int BluetoothPolicy::setModeChangeState(const bool enable)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    ret = bluetooth_dpm_set_allow_mode(enable == true ? BLUETOOTH_DPM_BT_ALLOWED : BLUETOOTH_DPM_BT_RESTRICTED);
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    SetPolicyEnabled(context, "bluetooth", enable);

    return 0;
}

bool BluetoothPolicy::getModeChangeState()
{
    // return IsPolicyEnabled(context, "bluetooth");

    int bt_state = isBluetoothEnabled();
    if (bt_state == 1)
        ERROR("isBluetoothEnabled: true");
    else if (bt_state == 0)
        ERROR("isBluetoothEnabled: false");
    else
        ERROR("isBluetoothEnabled: error -> " + bt_state);

    return true;
}

int BluetoothPolicy::addDeviceToBlacklist(const std::string& mac)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    // to avoid mac address check and convert to bluetooth_device_address_t,
    // use capi-network-bluetooth CAPI without bluetooth-api CAPI.
    ret = bt_dpm_add_devices_to_blacklist(mac.c_str());
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::removeDeviceFromBlacklist(const std::string& mac)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    // to avoid mac address check and convert to bluetooth_device_address_t,
    // use capi-network-bluetooth CAPI without bluetooth-api CAPI.
    ret = bt_dpm_remove_device_from_blacklist(mac.c_str());
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::setDeviceRestriction(const bool enable)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    ret = bluetooth_dpm_activate_device_restriction(enable == true ? BLUETOOTH_DPM_RESTRICTED : BLUETOOTH_DPM_ALLOWED);
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    SetPolicyEnabled(context, "bluetooth-device-restriction", enable);

    return 0;
}

bool BluetoothPolicy::isDeviceRestricted()
{
    return IsPolicyEnabled(context, "bluetooth-device-restriction");
}

int BluetoothPolicy::addUuidToBlacklist(const std::string& uuid)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    ret = bluetooth_dpm_add_uuids_to_blacklist(uuid.c_str());
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::removeUuidFromBlacklist(const std::string& uuid)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    ret = bluetooth_dpm_remove_uuid_from_blacklist(uuid.c_str());
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::setUuidRestriction(const bool enable)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    ret = bluetooth_dpm_activate_uuid_restriction(enable == true ? BLUETOOTH_DPM_RESTRICTED : BLUETOOTH_DPM_ALLOWED);
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    SetPolicyEnabled(context, "bluetooth-uuid-restriction", enable);

    return 0;
}

bool BluetoothPolicy::isUuidRestricted()
{
    return IsPolicyEnabled(context, "bluetooth-uuid-restriction");
}

BluetoothPolicy bluetoothPolicy(Server::instance());

} // namespace DevicePolicyManager

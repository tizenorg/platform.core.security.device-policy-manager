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

#include <bluetooth-api.h>

#include "bluetooth.hxx"

#include "policy-helper.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

BluetoothPolicy::BluetoothPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
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
}

BluetoothPolicy::~BluetoothPolicy()
{
}

static int convert_bluetooth_device_address_t(const std::string addr_str, bluetooth_device_address_t *addr_hex)
{
    unsigned int addr[BLUETOOTH_ADDRESS_LENGTH] = { 0, };

    if (addr_str.empty()) {
        return -1;
    }

    int convert_cnt = sscanf(addr_str.c_str(), "%X:%X:%X:%X:%X:%X", &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5]);
    if (convert_cnt != BLUETOOTH_ADDRESS_LENGTH) {
        ERROR("Invalid format string: " + addr_str);
        return -1;
    }

    for (int addridx = 0; addridx < BLUETOOTH_ADDRESS_LENGTH; addridx++) {
        addr_hex->addr[addridx] = (unsigned char)addr[addridx];
    }

    return 0;
}

int BluetoothPolicy::addDeviceToBlacklist(const std::string& mac)
{
    ERROR("addDeviceToBlacklist + mac: " + mac);

    int ret = 0;
    bluetooth_device_address_t mac_addr = {0, };
    if (convert_bluetooth_device_address_t(mac, &mac_addr) == -1) {
        ERROR("convert_bluetooth_device_address_t Failed");
        return -1;
    }

    ret = bluetooth_dpm_add_devices_to_blacklist(&mac_addr);
    ERROR("ret: " + std::to_string(ret));
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        ERROR("bluetooth_dpm_add_bluetooth_devices_to_blacklist Failed, ret: " + std::to_string(ret));
        return -1;
    }

    return ret;
}

int BluetoothPolicy::removeDeviceFromBlacklist(const std::string& mac)
{
    ERROR("removeDeviceFromBlacklist + mac: " + mac);

    int ret = 0;
    bluetooth_device_address_t mac_addr = {0, };
    if (convert_bluetooth_device_address_t(mac, &mac_addr) == -1) {
        ERROR("convert_bluetooth_device_address_t Failed");
        return -1;
    }

    ret = bluetooth_dpm_remove_device_from_blacklist(&mac_addr);
    ERROR("ret: " + std::to_string(ret));
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        ERROR("bluetooth_dpm_add_bluetooth_devices_to_blacklist Failed, ret: " + std::to_string(ret));
        return -1;
    }

    return ret;
}

int BluetoothPolicy::setDeviceRestriction(const bool enable)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    ret = bluetooth_dpm_activate_device_restriction(enable == true ? BLUETOOTH_DPM_ALLOWED : BLUETOOTH_DPM_RESTRICTED);
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    SetPolicyEnabled(context, "bluetooth-device-restriction", enable);

    ERROR("setDeviceRestriction + enable: " + std::string(enable ? "true" : "false"));
    ERROR("setDeviceRestriction + policy_ret: " + std::string(IsPolicyEnabled(context, "bluetooth-device-restriction") ? "true" : "false"));

    return 0;
}

bool BluetoothPolicy::isDeviceRestricted()
{
    ERROR("isDeviceRestricted: " + std::string(IsPolicyEnabled(context, "bluetooth-device-restriction") ? "true" : "false"));
    return IsPolicyEnabled(context, "bluetooth-device-restriction");
}

int BluetoothPolicy::addUuidToBlacklist(const std::string& uuid)
{
    ERROR("addUuidToBlacklist + uuid: " + uuid);

    int ret = bluetooth_dpm_add_uuids_to_blacklist(uuid.c_str());
    ERROR("ret: " + std::to_string(ret));
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        ERROR("bluetooth_dpm_add_bluetooth_uuids_to_blacklist Failed, ret: " + std::to_string(ret));
        return -1;
    }

    return ret;
}

int BluetoothPolicy::removeUuidFromBlacklist(const std::string& uuid)
{
    ERROR("removeUuidFromBlacklist + uuid: " + uuid);

    int ret = bluetooth_dpm_remove_uuid_from_blacklist(uuid.c_str());
    ERROR("ret: " + std::to_string(ret));
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        ERROR("bluetooth_dpm_remove_bluetooth_uuid_from_blacklist Failed, ret: " + std::to_string(ret));
        return -1;
    }

    return ret;
}

int BluetoothPolicy::setUuidRestriction(const bool enable)
{
    int ret = BLUETOOTH_DPM_RESULT_SUCCESS;
    ret = bluetooth_dpm_activate_uuid_restriction(enable == true ? BLUETOOTH_DPM_ALLOWED : BLUETOOTH_DPM_RESTRICTED);
    if (ret == BLUETOOTH_DPM_RESULT_ACCESS_DENIED ||
        ret == BLUETOOTH_DPM_RESULT_FAIL) {
        return -1;
    }

    SetPolicyEnabled(context, "bluetooth-uuid-restriction", enable);

    ERROR("setUuidRestriction + enable: " + std::string(enable ? "true" : "false"));
    ERROR("setUuidRestriction + policy_ret: " + std::string(IsPolicyEnabled(context, "bluetooth-uuid-restriction") ? "true" : "false"));

    return 0;
}

bool BluetoothPolicy::isUuidRestricted()
{
    ERROR("isUuidRestricted: " + std::string(IsPolicyEnabled(context, "bluetooth-uuid-restriction") ? "true" : "false"));
    return IsPolicyEnabled(context, "bluetooth-uuid-restriction");
}

BluetoothPolicy bluetoothPolicy(Server::instance());

} // namespace DevicePolicyManager

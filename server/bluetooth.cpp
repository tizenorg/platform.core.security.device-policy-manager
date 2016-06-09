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
#include "restriction.hxx"

#include "policy-helper.h"
#include "audit/logger.h"
#include "dbus/connection.h"

#define POLICY_ENFORCING_FAILED(ret)                    \
    (((ret) == BLUETOOTH_DPM_RESULT_ACCESS_DENIED) ||   \
     ((ret) == BLUETOOTH_DPM_RESULT_FAIL))

#define POLICY_IS_ALLOWED(enable)                       \
    ((enable) ? BLUETOOTH_DPM_ALLOWED :                 \
                BLUETOOTH_DPM_RESTRICTED)

#define STATE_CHANGE_IS_ALLOWED(enable)                 \
    ((enable) ? BLUETOOTH_DPM_BT_ALLOWED :              \
                BLUETOOTH_DPM_BT_RESTRICTED)


#define MOBILEAP_INTERFACE		\
	"org.tizen.MobileapAgent",	\
	"/MobileapAgent",			\
	"org.tizen.tethering"

namespace DevicePolicyManager {

struct BluetoothPolicyContext {
    BluetoothPolicyContext(BluetoothPolicy* p, PolicyControlContext* c) :
        policy(p), context(c)
    {
    }

    BluetoothPolicy* policy;
    PolicyControlContext* context;
};

static void bluetoothAdapterStateChangedCallback(int result, bt_adapter_state_e state, void *user_data)
{
    if (state != BT_ADAPTER_ENABLED) {
        return;
    }

    BluetoothPolicyContext *bluetooth = (BluetoothPolicyContext *)user_data;
    BluetoothPolicy &policy = *bluetooth->policy;
    PolicyControlContext &context = *bluetooth->context;

    bool allowed = IsPolicyAllowed(context, "bluetooth");
    if (policy.setModeChangeState(allowed) != BLUETOOTH_DPM_RESULT_SUCCESS) {
        // TODO(seok85.hong): we can notify to admin client with this notification.
    }

    allowed = IsPolicyAllowed(context, "bluetooth-desktop-connectivity");
    if (policy.setDesktopConnectivityState(allowed) != BLUETOOTH_DPM_RESULT_SUCCESS) {
        // TODO(seok85.hong): we can notify to admin client with this notification.
    }

    allowed = IsPolicyAllowed(context, "bluetooth-pairing");
    if (policy.setPairingState(allowed) != BLUETOOTH_DPM_RESULT_SUCCESS) {
        // TODO(seok85.hong): we can notify to admin client with this notification.
    }

    bool enabled = IsPolicyEnabled(context, "bluetooth-device-restriction");
    if (policy.setDeviceRestriction(enabled) != BLUETOOTH_DPM_RESULT_SUCCESS) {
        // TODO(seok85.hong): we can notify to admin client with this notification.
    }

    enabled = IsPolicyEnabled(context, "bluetooth-uuid-restriction");
    if (policy.setUuidRestriction(enabled) != BLUETOOTH_DPM_RESULT_SUCCESS) {
        // TODO(seok85.hong): we can notify to admin client with this notification.
    }
}

BluetoothPolicy::BluetoothPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setModeChangeState)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::getModeChangeState));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setDesktopConnectivityState)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::getDesktopConnectivityState));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setTetheringState)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::getTetheringState));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setPairingState)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::getPairingState));

    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::addDeviceToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::removeDeviceFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setDeviceRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::isDeviceRestricted));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::addUuidToBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::removeUuidFromBlacklist)(std::string));
    ctxt.registerParametricMethod(this, (int)(BluetoothPolicy::setUuidRestriction)(bool));
    ctxt.registerNonparametricMethod(this, (bool)(BluetoothPolicy::isUuidRestricted));

    ctxt.createNotification("bluetooth");
    ctxt.createNotification("bluetooth-tethering");
    ctxt.createNotification("bluetooth-desktop-connectivity");
    ctxt.createNotification("bluetooth-pairing");
    ctxt.createNotification("bluetooth-uuid-restriction");
    ctxt.createNotification("bluetooth-device-restriction");

    if (::bt_initialize() != BT_ERROR_NONE) {
        ERROR("Failed to initialize the Bluetooth");
        return;
    }

    if (bt_adapter_set_state_changed_cb(bluetoothAdapterStateChangedCallback,
                                        new BluetoothPolicyContext(this, &ctxt)) != BT_ERROR_NONE) {
        ERROR("Failed to register a bluetooth adaptor callback");
        return;
    }
}

BluetoothPolicy::~BluetoothPolicy()
{
    bt_deinitialize();
}

int BluetoothPolicy::setModeChangeState(const bool enable)
{
    int ret = bluetooth_dpm_set_allow_mode(STATE_CHANGE_IS_ALLOWED(enable));
    if (POLICY_ENFORCING_FAILED(ret)) {
        return -1;
    }

    SetPolicyAllowed(context, "bluetooth", enable);
    return 0;
}

bool BluetoothPolicy::getModeChangeState()
{
    return IsPolicyAllowed(context, "bluetooth");
}

int BluetoothPolicy::setDesktopConnectivityState(const bool enable)
{
    int ret = bluetooth_dpm_set_desktop_connectivity_state(POLICY_IS_ALLOWED(enable));
    if (POLICY_ENFORCING_FAILED(ret)) {
        return -1;
    }

    SetPolicyAllowed(context, "bluetooth-desktop-connectivity", enable);
    return 0;
}

bool BluetoothPolicy::getDesktopConnectivityState()
{
    return IsPolicyAllowed(context, "bluetooth-desktop-connectivity");
}

int BluetoothPolicy::setPairingState(const bool enable)
{
    int ret = bluetooth_dpm_set_pairing_state(POLICY_IS_ALLOWED(enable));
    if (POLICY_ENFORCING_FAILED(ret)) {
        return -1;
    }

    SetPolicyAllowed(context, "bluetooth-pairing", enable);
    return 0;
}

bool BluetoothPolicy::getPairingState()
{
    return IsPolicyAllowed(context, "bluetooth-pairing");
}


int BluetoothPolicy::addDeviceToBlacklist(const std::string& mac)
{
    int ret = bt_dpm_add_devices_to_blacklist(mac.c_str());
    if (POLICY_ENFORCING_FAILED(ret)) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::setTetheringState(bool enable)
{
    try {
        dbus::Connection &systemDBus = dbus::Connection::getSystem();
        systemDBus.methodcall(MOBILEAP_INTERFACE,
                              "change_policy",
                              -1,
                              "",
                              "(sb)",
                              "bluetooth-tethering",
                              enable);
    } catch (runtime::Exception& e) {
        ERROR("Failed to change bluetooth tethering state");
        return -1;
    }

    SetPolicyAllowed(context, "bluetooth-tethering", enable);
    return 0;
}

bool BluetoothPolicy::getTetheringState()
{
    return IsPolicyAllowed(context, "bluetooth-tethering");
}


int BluetoothPolicy::removeDeviceFromBlacklist(const std::string& mac)
{
    int ret = bt_dpm_remove_device_from_blacklist(mac.c_str());
    if (POLICY_ENFORCING_FAILED(ret)) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::setDeviceRestriction(const bool enable)
{
    int ret = bluetooth_dpm_activate_device_restriction(POLICY_IS_ALLOWED(!enable));
    if (POLICY_ENFORCING_FAILED(ret)) {
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
    int ret = bluetooth_dpm_add_uuids_to_blacklist(uuid.c_str());
    if (POLICY_ENFORCING_FAILED(ret)) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::removeUuidFromBlacklist(const std::string& uuid)
{
    int ret = bluetooth_dpm_remove_uuid_from_blacklist(uuid.c_str());
    if (POLICY_ENFORCING_FAILED(ret)) {
        return -1;
    }

    return ret;
}

int BluetoothPolicy::setUuidRestriction(const bool enable)
{
    int ret = bluetooth_dpm_activate_uuid_restriction(POLICY_IS_ALLOWED(!enable));
    if (POLICY_ENFORCING_FAILED(ret)) {
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

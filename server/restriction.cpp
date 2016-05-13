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

#include <vconf.h>

#include "restriction.hxx"
#include "policy-helper.h"
#include "audit/logger.h"
#include "dbus/connection.h"

#define PULSEAUDIO_LOGIN_INTERFACE \
    "org.pulseaudio.Server",   \
    "/org/pulseaudio/StreamManager",  \
    "org.pulseaudio.StreamManager"

namespace DevicePolicyManager {

RestrictionPolicy::RestrictionPolicy(PolicyControlContext& ctxt) :
	context(ctxt)
{
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setCameraState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getCameraState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setMicrophoneState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getMicrophoneState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setClipboardState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getClipboardState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setSettingsChangesState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getSettingsChangesState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setUsbDebuggingState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getUsbDebuggingState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setExternalStorageState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getExternalStorageState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setLocationState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getLocationState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setWifiState)(bool));
	context.registerNonparametricMethod(this, (bool)(RestrictionPolicy::getWifiState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setWifiHotspotState)(bool));
	context.registerNonparametricMethod(this, (bool)(RestrictionPolicy::getWifiHotspotState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setBluetoothTetheringState)(bool));
	context.registerNonparametricMethod(this, (bool)(RestrictionPolicy::getBluetoothTetheringState));

	context.createNotification("camera");
	context.createNotification("clipboard");
	context.createNotification("external-storage");
	context.createNotification("microphone");
	context.createNotification("location");
	context.createNotification("settings-changes");
	context.createNotification("usb-debugging");
	context.createNotification("wifi");
	context.createNotification("wifi-hotspot");
	context.createNotification("bluetooth-tethering");
}

RestrictionPolicy::~RestrictionPolicy()
{
}

int RestrictionPolicy::setCameraState(int enable)
{
    SetPolicyAllowed(context, "camera", enable);
    return 0;
}

int RestrictionPolicy::getCameraState()
{
    return IsPolicyAllowed(context, "camera");
}

int RestrictionPolicy::setMicrophoneState(int enable)
{
    char *result = NULL;
    dbus::Connection &systemDBus = dbus::Connection::getSystem();
    systemDBus.methodcall(PULSEAUDIO_LOGIN_INTERFACE, "UpdateRestriction",
                          -1, "(s)", "(su)", "block_recording_media", enable).get("(s)", &result);
    if (strcmp(result, "STREAM_MANAGER_RETURN_OK") == 0) {
        SetPolicyAllowed(context, "microphone", enable);
    } else
        return -1;

    return 0;
}

int RestrictionPolicy::getMicrophoneState()
{
    return IsPolicyAllowed(context, "microphone");
}

int RestrictionPolicy::setClipboardState(int enable)
{
    SetPolicyAllowed(context, "clipboard", enable);
    return 0;
}

int RestrictionPolicy::getClipboardState()
{
    return IsPolicyAllowed(context, "clipboard");
}

int RestrictionPolicy::setSettingsChangesState(int enable)
{
    SetPolicyAllowed(context, "settings-changes", enable);
    return 0;
}

int RestrictionPolicy::getSettingsChangesState()
{
    return IsPolicyAllowed(context, "settings-changes");
}

int RestrictionPolicy::setUsbDebuggingState(int enable)
{
    SetPolicyAllowed(context, "usb-debugging", enable);
    return 0;
}

int RestrictionPolicy::getUsbDebuggingState()
{
    return IsPolicyAllowed(context, "usb-debugging");
}

int RestrictionPolicy::setExternalStorageState(int enable)
{
    SetPolicyAllowed(context, "external-storage", enable);
    return 0;
}

int RestrictionPolicy::getExternalStorageState()
{
    return IsPolicyAllowed(context, "external-storage");
}

int RestrictionPolicy::setLocationState(int enable)
{
    SetPolicyAllowed(context, "location", enable);
    return 0;
}

int RestrictionPolicy::getLocationState()
{
    return IsPolicyAllowed(context, "location");
}

int RestrictionPolicy::setWifiState(bool enable)
{
    SetPolicyAllowed(context, "wifi", enable);
    return 0;
}

bool RestrictionPolicy::getWifiState()
{
    return IsPolicyAllowed(context, "wifi");
    return 0;
}

int RestrictionPolicy::setWifiHotspotState(bool enable)
{
    SetPolicyAllowed(context, "wifi-hotspot", enable);
    return 0;
}

bool RestrictionPolicy::getWifiHotspotState()
{
    return IsPolicyAllowed(context, "wifi-hotspot");
}

int RestrictionPolicy::setBluetoothTetheringState(bool enable)
{
	SetPolicyAllowed(context, "bluetooth-tethering", enable);
	return 0;
}

bool RestrictionPolicy::getBluetoothTetheringState()
{
	return IsPolicyAllowed(context, "bluetooth-tethering");
}

RestrictionPolicy restrictionPolicy(Server::instance());

} // namespace DevicePolicyManager

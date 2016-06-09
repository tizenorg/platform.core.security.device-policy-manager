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

#define PULSEAUDIO_LOGIN_INTERFACE          \
    "org.pulseaudio.Server",                \
    "/org/pulseaudio/StreamManager",        \
    "org.pulseaudio.StreamManager"

#define DEVICED_SYSNOTI_INTERFACE           \
    "org.tizen.system.deviced",             \
    "/Org/Tizen/System/DeviceD/SysNoti",    \
    "org.tizen.system.deviced.SysNoti",     \
    "control"

#define MOBILEAP_INTERFACE                  \
    "org.tizen.MobileapAgent",              \
    "/MobileapAgent",                       \
    "org.tizen.tethering"


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
    context.registerParametricMethod(this, (int)(RestrictionPolicy::setUsbTetheringState)(int));
    context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getUsbTetheringState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setExternalStorageState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getExternalStorageState));

	context.createNotification("camera");
	context.createNotification("clipboard");
	context.createNotification("external-storage");
	context.createNotification("microphone");
	context.createNotification("settings-changes");
	context.createNotification("usb-debugging");
    context.createNotification("usb-tethering");
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

int RestrictionPolicy::setUsbTetheringState(bool enable)
{
    try {
        dbus::Connection &systemDBus = dbus::Connection::getSystem();
        systemDBus.methodcall(MOBILEAP_INTERFACE,
                              "change_policy",
                              -1,
                              "",
                              "(sb)",
                              "usb-tethering",
                              enable);
    } catch (runtime::Exception& e) {
        ERROR("Failed to change USB tethering state");
        return -1;
    }

    SetPolicyAllowed(context, "usb-tethering", enable);
    return 0;
}

bool RestrictionPolicy::getUsbTetheringState()
{
    return IsPolicyAllowed(context, "usb-tethering");
}

int RestrictionPolicy::setExternalStorageState(int enable)
{
    int ret;
    std::string pid(std::to_string(::getpid()));
    std::string state(std::to_string(enable));

    dbus::Connection &systemDBus = dbus::Connection::getSystem();
    systemDBus.methodcall(DEVICED_SYSNOTI_INTERFACE, -1,
                          "(i)", "(sisss)", "control",
                          3, pid.c_str(), "2", state.c_str()).get("(i)", &ret);
    if (ret != 0) {
        return -1;
    }

    SetPolicyAllowed(context, "external-storage", enable);
    return 0;
}

int RestrictionPolicy::getExternalStorageState()
{
    return IsPolicyAllowed(context, "external-storage");
}

RestrictionPolicy restrictionPolicy(Server::instance());

} // namespace DevicePolicyManager

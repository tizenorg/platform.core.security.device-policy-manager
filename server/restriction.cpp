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

#include "restriction.hxx"
#include "audit/logger.h"

#include <vconf.h>

namespace DevicePolicyManager
{

RestrictionPolicy::RestrictionPolicy(PolicyControlContext& ctxt)
	: context(ctxt)
{
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setCameraState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getCameraState));
	context.registerParametricMethod(this, (int)(RestrictionPolicy::setMicrophoneState)(int));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getMicrophoneState));
	context.registerNonparametricMethod(this, (int)(RestrictionPolicy::getClipboardState));
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
}

RestrictionPolicy::~RestrictionPolicy()
{
}

int RestrictionPolicy::setCameraState(int state)
{
	return 0;
}

int RestrictionPolicy::getCameraState()
{
	return 0;
}

int RestrictionPolicy::setMicrophoneState(int state)
{
	return 0;
}

int RestrictionPolicy::getMicrophoneState()
{
	return 0;
}

int RestrictionPolicy::setClipboardState(int enable)
{
	return 0;
}

int RestrictionPolicy::getClipboardState()
{
	return true;
}

int RestrictionPolicy::setSettingsChangesState(int enable)
{
	return 0;
}

int RestrictionPolicy::getSettingsChangesState()
{
	return true;
}

int RestrictionPolicy::setUsbDebuggingState(int enable)
{
    INFO("Start Restriction::setUsbDebuggingRestriction");

    // if enable is true, restrication will be working (0).
    if (enable == true) {
        if (::vconf_set_int(VCONFKEY_SETAPPL_USB_DEBUG_MODE_BOOL, 0) != 0) {
            ERROR("Failed to set usb debugging mode status");
            return -1;
        }
    } else {
        if (::vconf_set_int(VCONFKEY_SETAPPL_USB_DEBUG_MODE_BOOL, 1) != 0) {
            ERROR("Failed to set usb debugging mode status");
            return -1;
        }
    }

	return 0;
}

int RestrictionPolicy::getUsbDebuggingState()
{
	int status;
	INFO("Start Restriction::isUsbDebuggingRestricted");

	// 0 is restrication true, 1 is restrication false
    if (::vconf_get_int(VCONFKEY_SETAPPL_USB_DEBUG_MODE_BOOL, &status) != 0) {
        ERROR("Failed to get usb debugging mode status");
        return -1;
    }

    // How to return for error??
    if (status == 1) {
    	return false;
    }
    return true;
}

int RestrictionPolicy::setExternalStorageState(int enable)
{
	return 0;
}

int RestrictionPolicy::getExternalStorageState()
{
	return 0;
}

int RestrictionPolicy::setLocationState(int enable)
{
	return 0;
}

int RestrictionPolicy::getLocationState()
{
	return 0;
}

int RestrictionPolicy::setWifiState(bool enable)
{
	return 0;
}

bool RestrictionPolicy::getWifiState()
{
	return 0;
}

int RestrictionPolicy::setWifiHotspotState(bool enable)
{
	return 0;
}

bool RestrictionPolicy::getWifiHotspotState()
{
	return 0;
}

RestrictionPolicy restrictionPolicy(Server::instance());

} // namespace DevicePolicyManager

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

Restriction::Restriction(PolicyControlContext& ctxt)
	: context(ctxt)
{
	context.registerParametricMethod(this, (int)(Restriction::setClipboardRestriction)(bool));
	context.registerNonparametricMethod(this, (bool)(Restriction::isClipboardRestricted));
	context.registerParametricMethod(this, (int)(Restriction::setSettingsChangesRestriction)(bool));
	context.registerNonparametricMethod(this, (bool)(Restriction::isSettingsChangesRestricted));
	context.registerParametricMethod(this, (int)(Restriction::setUsbDebuggingRestriction)(bool));
	context.registerNonparametricMethod(this, (bool)(Restriction::isUsbDebuggingRestricted));
	context.registerParametricMethod(this, (int)(Restriction::setUsbMassStorageRestriction)(bool));
	context.registerNonparametricMethod(this, (bool)(Restriction::isUsbMassStorageRestricted));
}

Restriction::~Restriction()
{
}

int Restriction::setClipboardRestriction(bool enable)
{
	return 0;
}

bool Restriction::isClipboardRestricted()
{
	return true;
}

int Restriction::setSettingsChangesRestriction(bool enable)
{
	return 0;
}

bool Restriction::isSettingsChangesRestricted()
{
	return true;
}

int Restriction::setUsbDebuggingRestriction(bool enable)
{
    INFO("Start Restriction::setUsbDebuggingRestriction");

    // if enable is true, restrication will be working (0).
    if(enable == true) {
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

bool Restriction::isUsbDebuggingRestricted()
{
	int status;
	INFO("Start Restriction::isUsbDebuggingRestricted");

	// 0 is restrication true, 1 is restrication false
    if (::vconf_get_int(VCONFKEY_SETAPPL_USB_DEBUG_MODE_BOOL, &status) != 0) {
        ERROR("Failed to get usb debugging mode status");
        return -1;
    }

    // How to return for error??
    if(status == 1) {
    	return false;
    }
    return true;
}

int Restriction::setUsbMassStorageRestriction(bool enable)
{
	return 0;
}

bool Restriction::isUsbMassStorageRestricted()
{
	return true;
}

int Restriction::wipeData()
{
	return 0;
}

Restriction restrictionPolicy(Server::instance());

} // namespace DevicePolicyManager

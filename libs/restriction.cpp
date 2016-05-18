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

namespace DevicePolicyManager
{

RestrictionPolicy::RestrictionPolicy(PolicyControlContext& ctxt)
	: context(ctxt)
{
}

RestrictionPolicy::~RestrictionPolicy()
{
}

int RestrictionPolicy::setMicrophoneState(int enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setMicrophoneState", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int RestrictionPolicy::getMicrophoneState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getMicrophoneState");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int RestrictionPolicy::setCameraState(int enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setCameraState", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int RestrictionPolicy::getCameraState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getCameraState");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int RestrictionPolicy::setClipboardState(int enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setClipboardState", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::getClipboardState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getClipboardState");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::setSettingsChangesState(int enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setSettingsChangesState", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::getSettingsChangesState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getSettingsChangesState");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::setUsbDebuggingState(int enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setUsbDebuggingState", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::getUsbDebuggingState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getUsbDebuggingState");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::setUsbTetheringState(bool enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setUsbTetheringState", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}
bool RestrictionPolicy::getUsbTetheringState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getUsbTetheringState");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int RestrictionPolicy::setExternalStorageState(int enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setExternalStorageState", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::getExternalStorageState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getExternalStorageState");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::setLocationState(int enable)
{
	try {
		return context->methodCall<int>("RestrictionPolicy::setLocationState", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int RestrictionPolicy::getLocationState()
{
	try {
		return context->methodCall<int>("RestrictionPolicy::getLocationState");
	} catch (runtime::Exception& e) {
		return -1;
	}
}


int RestrictionPolicy::setWifiState(bool enable)
{
    try {
        return context->methodCall<int>("RestrictionPolicy::setWifiState", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool RestrictionPolicy::getWifiState()
{
    try {
        return context->methodCall<bool>("RestrictionPolicy::getWifiState");
    } catch (runtime::Exception &e) {
        return -1;
    }
}

int RestrictionPolicy::setWifiHotspotState(bool enable)
{
    try {
        return context->methodCall<int>("RestrictionPolicy::setWifiHotspotState", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool RestrictionPolicy::getWifiHotspotState()
{
    try {
        return context->methodCall<bool>("RestrictionPolicy::getWifiHotspotState");
    } catch (runtime::Exception &e) {
        return -1;
    }
}

int RestrictionPolicy::setBluetoothTetheringState(bool enable)
{
    try {
        return context->methodCall<int>("RestrictionPolicy::setBluetoothTetheringState", enable);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool RestrictionPolicy::getBluetoothTetheringState()
{
    try {
        return context->methodCall<bool>("RestrictionPolicy::getBluetoothTetheringState");
    } catch (runtime::Exception &e) {
        return -1;
    }
}

} //namespace DevicePolicyManager

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

#ifndef __RESTRICTION_POLICY__
#define __RESTRICTION_POLICY__

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager
{

class RestrictionPolicy
{
public:
	RestrictionPolicy(PolicyControlContext& ctxt);
	~RestrictionPolicy();

	int setCameraState(int enable);
	int getCameraState();

	int setMicrophoneState(int enable);
	int getMicrophoneState();

	int setClipboardState(int enable);
	int getClipboardState();

	int setSettingsChangesState(int enable);
	int getSettingsChangesState();

	int setUsbDebuggingState(int enable);
	int getUsbDebuggingState();

	int setExternalStorageState(int enable);
	int getExternalStorageState();

	int setLocationState(int enable);
	int getLocationState();

	int setWifiState(bool enable);
	bool getWifiState();

	int setWifiHotspotState(bool enable);
	bool getWifiHotspotState();

private:
	PolicyControlContext& context;
};

} // namespace DevicePolicyManager

#endif /* __RESTRICTION_POLICY__ */

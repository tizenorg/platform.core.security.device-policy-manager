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

class Restriction
{
public:
	Restriction(PolicyControlContext& ctxt);
	~Restriction();

	int allowClipboard(bool allow);
	bool isClipboardAllowed();

	int allowSettingsChanges(bool allow);
	bool isSettingsChangesAllowed();

	int allowBackgroundData(bool allow);
	bool isBackgroundDataAllowed();

	int allowUsbDebugging(bool allow);
	bool isUsbDebuggingAllowed();

	int allowUsbMassStorage(bool allow);
	bool isUsbMassStorageAllowed();

	int allowFactoryReset(bool allow);
	bool isFactoryResetAllowed();

	int allowBluetoothTethering(bool allow);
	bool isBluetoothTetheringAllowed();

	int allowMockLocation(bool allow);
	bool isMockLocationAllowed();

private:
	PolicyControlContext& context;
};

} // namespace DevicePolicyManager

#endif /* __RESTRICTION_POLICY__ */

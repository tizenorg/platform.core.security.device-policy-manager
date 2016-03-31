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

Restriction::Restriction(PolicyControlContext& ctxt)
	: context(ctxt)
{
	rmi::Service& manager = context.getServiceManager();

	manager.registerParametricMethod(this, (int)(Restriction::allowClipboard)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isClipboardAllowed));
	manager.registerParametricMethod(this, (int)(Restriction::allowSettingsChanges)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isSettingsChangesAllowed));
	manager.registerParametricMethod(this, (int)(Restriction::allowBackgroundData)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isBackgroundDataAllowed));
	manager.registerParametricMethod(this, (int)(Restriction::allowUsbDebugging)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isUsbDebuggingAllowed));
	manager.registerParametricMethod(this, (int)(Restriction::allowUsbMassStorage)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isUsbMassStorageAllowed));
	manager.registerParametricMethod(this, (int)(Restriction::allowFactoryReset)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isFactoryResetAllowed));
	manager.registerParametricMethod(this, (int)(Restriction::allowBluetoothTethering)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isBluetoothTetheringAllowed));
	manager.registerParametricMethod(this, (int)(Restriction::allowMockLocation)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::isMockLocationAllowed));
}

Restriction::~Restriction()
{
}

int Restriction::allowClipboard(bool allow)
{
	return 0;
}

bool Restriction::isClipboardAllowed()
{
	return true;
}


int Restriction::allowSettingsChanges(bool allow)
{
	return 0;
}

bool Restriction::isSettingsChangesAllowed()
{
	return true;
}

int Restriction::allowBackgroundData(bool allow)
{
	return 0;
}

bool Restriction::isBackgroundDataAllowed()
{
	return true;
}

int Restriction::allowUsbDebugging(bool allow)
{
	return 0;
}

bool Restriction::isUsbDebuggingAllowed()
{
	return true;
}

int Restriction::allowUsbMassStorage(bool allow)
{
	return 0;
}

bool Restriction::isUsbMassStorageAllowed()
{
	return true;
}

int Restriction::allowFactoryReset(bool allow)
{
	return 0;
}

bool Restriction::isFactoryResetAllowed()
{
	return true;
}

int Restriction::allowBluetoothTethering(bool allow)
{
	return 0;
}

bool Restriction::isBluetoothTetheringAllowed()
{
	return true;
}

int Restriction::allowMockLocation(bool allow)
{
	return 0;
}

bool Restriction::isMockLocationAllowed()
{
	return true;
}

} // namespace DevicePolicyManager

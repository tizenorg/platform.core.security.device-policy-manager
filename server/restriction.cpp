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

	manager.registerParametricMethod(this, (int)(Restriction::setAllowClipboard)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowClipboard));
	manager.registerParametricMethod(this, (int)(Restriction::setAllowSettingsChanges)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowSettingsChanges));
	manager.registerParametricMethod(this, (int)(Restriction::setAllowBackgroundData)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowBackgroundData));
	manager.registerParametricMethod(this, (int)(Restriction::setAllowUsbDebugging)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowUsbDebugging));
	manager.registerParametricMethod(this, (int)(Restriction::setAllowUsbMassStorage)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowUsbMassStorage));
	manager.registerParametricMethod(this, (int)(Restriction::setAllowFactoryReset)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowFactoryReset));
	manager.registerParametricMethod(this, (int)(Restriction::setAllowBluetoothTethering)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowBluetoothTethering));
	manager.registerParametricMethod(this, (int)(Restriction::setAllowMockLocation)(bool));
	manager.registerNonparametricMethod(this, (bool)(Restriction::getAllowMockLocation));
}

Restriction::~Restriction()
{
}

int Restriction::setAllowClipboard(const bool enable)
{
	return 0;
}

bool Restriction::getAllowClipboard()
{
	bool ret = true;
	return ret;
}


int Restriction::setAllowSettingsChanges(const bool enable)
{
	return 0;
}

bool Restriction::getAllowSettingsChanges()
{
	bool ret = true;
	return ret;
}

int Restriction::setAllowBackgroundData(const bool enable)
{
	return 0;
}

bool Restriction::getAllowBackgroundData()
{
	bool ret = true;
	return ret;
}

int Restriction::setAllowUsbDebugging(const bool enable)
{
	return 0;
}

bool Restriction::getAllowUsbDebugging()
{
	bool ret = true;
	return ret;
}

int Restriction::setAllowUsbMassStorage(const bool enable)
{
	return 0;
}

bool Restriction::getAllowUsbMassStorage()
{
	bool ret = true;
	return ret;
}

int Restriction::setAllowFactoryReset(const bool enable)
{
	return 0;
}

bool Restriction::getAllowFactoryReset()
{
	bool ret = true;
	return ret;
}

int Restriction::setAllowBluetoothTethering(const bool enable)
{
	return 0;
}

bool Restriction::getAllowBluetoothTethering()
{
	bool ret = true;
	return ret;
}

int Restriction::setAllowMockLocation(const bool enable)
{
	return 0;
}

bool Restriction::getAllowMockLocation()
{
	bool ret = true;
	return ret;
}

} // namespace DevicePolicyManager

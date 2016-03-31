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
}

Restriction::~Restriction()
{
}

int Restriction::setAllowClipboard(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowClipboard", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowClipboard()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowClipboard");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setAllowSettingsChanges(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowSettingsChanges", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowSettingsChanges()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowSettingsChanges");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setAllowBackgroundData(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowBackgroundData", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowBackgroundData()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowBackgroundData");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setAllowUsbDebugging(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowUsbDebugging", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowUsbDebugging()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowUsbDebugging");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setAllowUsbMassStorage(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowUsbMassStorage", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowUsbMassStorage()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowUsbMassStorage");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setAllowFactoryReset(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowFactoryReset", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowFactoryReset()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowFactoryReset");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setAllowBluetoothTethering(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowBluetoothTethering", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowBluetoothTethering()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowBluetoothTethering");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setAllowMockLocation(const bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setAllowMockLocation", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::getAllowMockLocation()
{
	try {
		return context->methodCall<bool>("Restriction::getAllowMockLocation");
	} catch (runtime::Exception& e) {
		return -1;
	}
}
} //namespace DevicePolicyManager

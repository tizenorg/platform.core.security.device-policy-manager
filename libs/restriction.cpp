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

int Restriction::allowClipboard(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowClipboard", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isClipboardAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isClipboardAllowed");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::allowSettingsChanges(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowSettingsChanges", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isSettingsChangesAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isSettingsChanges");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::allowBackgroundData(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowBackgroundData", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isBackgroundDataAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isBackgroundDataAllowed");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::allowUsbDebugging(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowUsbDebugging", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isUsbDebuggingAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isUsbDebuggingAllowed");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::allowUsbMassStorage(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowUsbMassStorage", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isUsbMassStorageAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isUsbMassStorageAllowed");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::allowFactoryReset(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowFactoryReset", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isFactoryResetAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isFactoryResetAllowed");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::allowBluetoothTethering(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowBluetoothTethering", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isBluetoothTetheringAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isBluetoothTetheringAllowed");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::allowMockLocation(bool allow)
{
	try {
		return context->methodCall<int>("Restriction::allowMockLocation", allow);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isMockLocationAllowed()
{
	try {
		return context->methodCall<bool>("Restriction::isMockLocationAllowed");
	} catch (runtime::Exception& e) {
		return -1;
	}
}
} //namespace DevicePolicyManager

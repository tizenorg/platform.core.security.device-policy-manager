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

int Restriction::setClipboardRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setClipboardRestriction", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isClipboardRestricted()
{
	try {
		return context->methodCall<bool>("Restriction::isClipboardRestricted");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setSettingsChangesRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setSettingsChangesRestriction",enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isSettingsChangesRestricted()
{
	try {
		return context->methodCall<bool>("Restriction::isSettingsChangesRestricted");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setUsbDebuggingRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setUsbDebuggingRestriction", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isUsbDebuggingRestricted()
{
	try {
		return context->methodCall<bool>("Restriction::isUsbDebuggingRestricted");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::setUsbMassStorageRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Restriction::setUsbMassStorageRestriction", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool Restriction::isUsbMassStorageRestricted()
{
	try {
		return context->methodCall<bool>("Restriction::isUsbMassStorageRestricted");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int Restriction::wipeData(bool enable)
{
	try {
		return context->methodCall<int>("Restriction::wipeData", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

} //namespace DevicePolicyManager
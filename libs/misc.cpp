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

#include "misc.hxx"
#include "audit/logger.h"

namespace DevicePolicyManager {

Misc::Misc(PolicyControlContext &ctxt) :
    context(ctxt)
{
}

Misc::~Misc()
{
}

int Misc::setCameraRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Misc::setCameraRestriction", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isCameraRestricted()
{
	try {
		return context->methodCall<bool>("Misc::isCameraRestricted");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::setMicrophoneRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Misc::setMicrophoneRestriction", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isMicrophoneRestricted()
{
	try {
		return context->methodCall<bool>("Misc::isMicrophoneRestricted");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::setLocationRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Misc::setLocationRestriction", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isLocationRestricted()
{
	try {
		return context->methodCall<bool>("Misc::isLocationRestricted");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::setSdCardRestriction(bool enable)
{
	try {
		return context->methodCall<int>("Misc::setSdCardRestriction", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isSdCardRestricted()
{
	try {
		return context->methodCall<bool>("Misc::isSdCardRestricted");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

} //namespace DevicePolicyManager

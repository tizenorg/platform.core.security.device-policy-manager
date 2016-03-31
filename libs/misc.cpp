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

int Misc::allowCamera(bool allow)
{
	try {
		return context->methodCall<int>("Misc::allowCamera", allow);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isCameraAllowed()
{
	try {
		return context->methodCall<bool>("Misc::isCameraAllowed");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::allowMicrophone(bool allow)
{
	try {
		return context->methodCall<int>("Misc::allowMicrophone", allow);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isMicrophoneAllowed()
{
	try {
		return context->methodCall<bool>("Misc::isMicrophoneAllowed");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::allowLocation(bool allow)
{
	try {
		return context->methodCall<int>("Misc::allowLocation", allow);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isLocationAllowed()
{
	try {
		return context->methodCall<bool>("Misc::isLocationAllowed");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::allowSdCard(bool allow)
{
	try {
		return context->methodCall<int>("Misc::allowSdCard", allow);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::isSdCardAllowed()
{
	try {
		return context->methodCall<bool>("Misc::isSdCardAllowed");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

} //namespace DevicePolicyManager

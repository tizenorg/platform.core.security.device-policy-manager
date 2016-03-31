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

Misc::Misc(PolicyControlContext &ctxt)
	: context(ctxt)
{
}

Misc::~Misc()
{
}

int Misc::setAllowCamera(const bool enable)
{
	try {
		return context->methodCall<int>("Misc::setAllowCamera", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::getAllowCamera()
{
	try {
		return context->methodCall<bool>("Misc::getAllowCamera");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::setAllowMicrophone(const bool enable)
{
	try {
		return context->methodCall<int>("Misc::setAllowMicrophone", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::getAllowMicrophone()
{
	try {
		return context->methodCall<bool>("Misc::getAllowMicrophone");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::setAllowLocation(const bool enable)
{
	try {
		return context->methodCall<int>("Misc::setAllowLocation", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::getAllowLocation()
{
	try {
		return context->methodCall<bool>("Misc::getAllowLocation");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

int Misc::setAllowSDCard(const bool enable)
{
	try {
		return context->methodCall<int>("Misc::setAllowSDCard", enable);
	} catch (runtime::Exception &e) {
		return -1;
	}
}

bool Misc::getAllowSDCard()
{
	try {
		return context->methodCall<bool>("Misc::getAllowSDCard");
	} catch (runtime::Exception &e) {
		return -1;
	}
}

} //namespace DevicePolicyManager

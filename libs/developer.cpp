/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "policy-client.h"

#include "developer.hxx"

namespace DevicePolicyManager {

DeveloperPolicy::DeveloperPolicy(PolicyControlContext& ctx) :
    context(ctx)
{
}

DeveloperPolicy::~DeveloperPolicy()
{
}


int DeveloperPolicy::setUsbDebuggingState(bool enable)
{
	try {
		return context->methodCall<int>("DeveloperPolicy::setUsbDebuggingState", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool DeveloperPolicy::getUsbDebuggingState()
{
	try {
		return context->methodCall<bool>("DeveloperPolicy::getUsbDebuggingState");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

} //namespace DevicePolicyManager

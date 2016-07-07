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

#include "administration.hxx"

namespace DevicePolicyManager {

AdministrationPolicy::AdministrationPolicy(PolicyControlContext& ctx) :
	context(ctx)
{
}

AdministrationPolicy::~AdministrationPolicy()
{
}

int AdministrationPolicy::registerPolicyClient(const std::string& name)
{
	try {
		return context->methodCall<int>("AdministrationPolicy::registerPolicyClient", name);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int AdministrationPolicy::deregisterPolicyClient(const std::string& name)
{
	try {
		return context->methodCall<int>("AdministrationPolicy::deregisterPolicyClient", name);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

} // namespace DevicePolicyManager

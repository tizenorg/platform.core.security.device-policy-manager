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

#ifndef __ADMINISTRATION_POLICY__
#define __ADMINISTRATION_POLICY__

#include <string>

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager {

class AdministrationPolicy {
public:
	AdministrationPolicy(PolicyControlContext& ctxt);
	~AdministrationPolicy();

	int registerPolicyClient(const std::string& name, const int uid);
	int deregisterPolicyClient(const std::string& name, const int uid);

private:
	PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __ADMINISTRATION_POLICY__

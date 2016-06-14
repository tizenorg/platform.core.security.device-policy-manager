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

#ifndef __DPM_POLICY_STORAGE_ADAPTOR_H__
#define __DPM_POLICY_STORAGE_ADAPTOR_H__

#include <string>

#include "policy-context.hxx"

inline bool IsPolicyAllowed(PolicyControlContext& context, const std::string& name)
{
	return context.getPolicy(name) == "allowed" ? true : false;
}

inline bool IsPolicyEnabled(PolicyControlContext& context, const std::string& name)
{
	return context.getPolicy(name) == "enabled" ? true : false;
}

inline int SetPolicyAllowed(PolicyControlContext& context, const std::string& name, bool allow)
{
	return context.updatePolicy(name, allow ? "allowed" : "disallowed");
}

inline int SetPolicyEnabled(PolicyControlContext& context, const std::string& name, bool enable)
{
	return context.updatePolicy(name, enable ? "enabled" : "disabled");
}

#endif //! __DPM_POLICY_STORAGE_ADAPTOR_H__

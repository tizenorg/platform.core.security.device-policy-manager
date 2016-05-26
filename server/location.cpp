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
#include "policy-helper.h"
#include "audit/logger.h"

#define CONSTRUCTOR __attribute__ ((constructor))

namespace DevicePolicyManager {

extern RestrictionPolicy restrictionPolicy;

static void CONSTRUCTOR ContributeRestrictionPolicy()
{
	PolicyControlContext& context = Server::instance();

	context.registerParametricMethod(&restrictionPolicy, (int)(RestrictionPolicy::setLocationState)(int));
	context.registerNonparametricMethod(&restrictionPolicy, (int)(RestrictionPolicy::getLocationState));

	context.createNotification("location");
}

int RestrictionPolicy::setLocationState(int enable)
{
    SetPolicyAllowed(context, "location", enable);
    return 0;
}

int RestrictionPolicy::getLocationState()
{
    return IsPolicyAllowed(context, "location");
}


} // namespace DevicePolicyManager

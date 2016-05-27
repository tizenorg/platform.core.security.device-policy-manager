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

#include <location_batch.h>

#include "location.hxx"

#include "policy-helper.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

LocationPolicy::LocationPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
	context.registerParametricMethod(this, (int)(LocationPolicy::setLocationState)(int));
	context.registerNonparametricMethod(this, (int)(LocationPolicy::getLocationState));

	context.createNotification("location");
}

LocationPolicy::~LocationPolicy()
{
}

int LocationPolicy::setLocationState(int enable)
{
    if (location_manager_enable_restriction(!enable) != LOCATIONS_ERROR_NONE)
	return -1;

    SetPolicyAllowed(context, "location", enable);
    return 0;
}

int LocationPolicy::getLocationState()
{
    return IsPolicyAllowed(context, "location");
}


} // namespace DevicePolicyManager

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

#include "datashare.hxx"

#include "exception.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

DataSharePolicy::DataSharePolicy(PolicyControlContext& ctx) :
    context(ctx)
{
	context.registerParametricMethod(this, (int)(DataSharePolicy::setClipboardState)(bool));
	context.registerNonparametricMethod(this, (bool)(DataSharePolicy::getClipboardState));
}

DataSharePolicy::~DataSharePolicy()
{
}



int DataSharePolicy::setClipboardState(bool enable)
{
	INFO("Start DataSharePolicy::setClipboardState");
	return 0;
}

bool DataSharePolicy::getClipboardState()
{
	INFO("Start DataSharePolicy::getClipboardState");
	return true;
}

DataSharePolicy dataSharePolicy(Server::instance());

} //namespace DevicePolicyManager
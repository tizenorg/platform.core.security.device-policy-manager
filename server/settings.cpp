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

#include "settings.hxx"

#include "exception.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

SettingsPolicy::SettingsPolicy(PolicyControlContext& ctx) :
    context(ctx)
{
	context.registerParametricMethod(this, (int)(SettingsPolicy::setSettingsChangesRestriction)(bool));
	context.registerNonparametricMethod(this, (bool)(SettingsPolicy::isSettingsChangesRestricted));
}

SettingsPolicy::~SettingsPolicy()
{
}

int SettingsPolicy::setSettingsChangesRestriction(bool enable)
{
	INFO("Start SettingsPolicy::setSettingsChangesRestriction");
	return 0;
}

bool SettingsPolicy::isSettingsChangesRestricted()
{
	INFO("Start SettingsPolicy::isSettingsChangesRestricted");
	return true;
}

SettingsPolicy settingsPolicy(Server::instance());

} //namespace DevicePolicyManager

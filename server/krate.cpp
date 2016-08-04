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

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include <regex>
#include <algorithm>

#include <tzplatform_config.h>
#include <klay/auth/user.h>
#include <klay/audit/logger.h>
#include <krate/krate.h>

#include "krate.hxx"

#include "launchpad.h"
#include "privilege.h"
#include "policy-builder.h"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

namespace DevicePolicyManager {

namespace {

krate_manager_h krate_manager;
std::regex krateNamePattern(NAME_PATTERN);

bool isAllowedName(const std::string& name) {
	if (!std::regex_match(name, krateNamePattern)) {
		return false;
	}

	bool exists = false;
	try {
		runtime::User user(name);
		exists = true;
	} catch (runtime::Exception& e) {}

	return !exists;
}

bool foreach_krate_cb(const char* name, void* user_data) {
	auto pList = (std::vector<std::string>*)user_data;
	pList->push_back(name);
	return true;
}

}

KratePolicy::KratePolicy(PolicyControlContext& ctx) :
	context(ctx)
{
	context.registerParametricMethod(this, DPM_PRIVILEGE_ZONE, (int)(KratePolicy::createKrate)(std::string, std::string));
	context.registerParametricMethod(this, DPM_PRIVILEGE_ZONE, (int)(KratePolicy::removeKrate)(std::string));
	context.registerParametricMethod(this, "", (int)(KratePolicy::getKrateState)(std::string));
	context.registerParametricMethod(this, "", (std::vector<std::string>)(KratePolicy::getKrateList)(int));

	context.createNotification("KratePolicy::created");
	context.createNotification("KratePolicy::removed");

	krate_manager_create(&krate_manager);
}

KratePolicy::~KratePolicy()
{
	krate_manager_destroy(krate_manager);
}


int KratePolicy::createKrate(const std::string& name, const std::string& setupWizAppid)
{
	if (!std::regex_match(name, krateNamePattern)) {
		return -1;
	}

	if (!isAllowedName(name)) {
		return -1;
	}

	try {
		std::vector<std::string> data = {"app-id", "org.tizen.krate-setup-wizard",
										 "mode", "create",
										 "krate", name};
		Bundle bundle;
		bundle.add("id", "krate-create");
		bundle.add("user-data", data);

		Launchpad launchpad(context.getPeerUid());
		launchpad.launch("org.tizen.dpm-syspopup", bundle);
	} catch (runtime::Exception& e) {
		ERROR(e.what());
		return -1;
	}

	return 0;
}

int KratePolicy::removeKrate(const std::string& name)
{
	if (getKrateState(name) == 0) {
		return -1;
	}

	try {
		std::vector<std::string> data = {"app-id", "org.tizen.krate-setup-wizard",
										 "mode", "remove",
										 "krate", name};
		Bundle bundle;
		bundle.add("id", "krate-remove");
		bundle.add("user-data", data);

		Launchpad launchpad(context.getPeerUid());
		launchpad.launch("org.tizen.dpm-syspopup", bundle);
	} catch (runtime::Exception& e) {
		ERROR(e.what());
		return -1;
	}

	return 0;
}

int KratePolicy::getKrateState(const std::string& name)
{
	krate_state_e state = (krate_state_e)0;
	krate_manager_get_krate_state(krate_manager, name.c_str(), &state);
	return (int)state;
}

std::vector<std::string> KratePolicy::getKrateList(int state)
{
	std::vector<std::string> list;
	krate_manager_foreach_name(krate_manager, (krate_state_e)state, foreach_krate_cb, &list);
	return list;
}

DEFINE_POLICY(KratePolicy);

} // namespace DevicePolicyManager

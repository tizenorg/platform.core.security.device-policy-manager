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
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "policy-manager.h"

#include "exception.h"
#include "filesystem.h"
#include "audit/logger.h"

namespace {

enum policyStateOperator {
	POLICY_STATE_OPERATOR_NONE = 0,
	POLICY_STATE_OPERATOR_OR,
	POLICY_STATE_OPERATOR_AND,
	POLICY_STATE_OPERATOR_MIN,
	POLICY_STATE_OPERATOR_MAX,
	POLICY_STATE_OPERATOR_ALLOW,
};

std::unordered_map<std::string,std::pair<policyStateOperator, std::string>>
	defaultPolicies = {
	{"password-quality", {POLICY_STATE_OPERATOR_OR, "0"}},
	{"password-minimum-length", {POLICY_STATE_OPERATOR_MAX, "0"}},
	{"password-maximum-failure-count", {POLICY_STATE_OPERATOR_MIN, "0"}},
	{"password-expired", {POLICY_STATE_OPERATOR_MIN,"0"}},
	{"password-minimum-complexity", {POLICY_STATE_OPERATOR_MAX, "0"}},
	{"password-pattern", {POLICY_STATE_OPERATOR_NONE,""}},
	{"password-history", {POLICY_STATE_OPERATOR_MAX, "0"}},
	{"password-recovery", {POLICY_STATE_OPERATOR_MAX, "0"}},
	{"password-lock-delay", {POLICY_STATE_OPERATOR_MAX, "0"}},
	{"password-inactivity-timeout", {POLICY_STATE_OPERATOR_MAX, "1000"}},
	{"password-status", {POLICY_STATE_OPERATOR_NONE, "0"}},
	{"password-maximum-character-occurrences", {POLICY_STATE_OPERATOR_MIN, "0"}},
	{"password-numeric-sequences-length", {POLICY_STATE_OPERATOR_MIN, "0"}},
	{"password-forbidden-strings", {POLICY_STATE_OPERATOR_NONE, ""}},
	{"internal-storage-encryption", {POLICY_STATE_OPERATOR_NONE, ""}},
	{"external-storage-encryption", {POLICY_STATE_OPERATOR_NONE, ""}},
	{"package-installation-mode", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"package-uninstallation-mode", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"internet-sharing", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-pairing", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-outgoing-call", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-a2dp", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-avrcp", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-bpp", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-dun", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-ftp", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-hfp", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-hsp", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-pbap", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-sap", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-profile-spp", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-desktop-connectivity", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-descoverable", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-limited-discoverable", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-data-transfer", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-uuid-restriction", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-device-restriction", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"wifi", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"wifi-hotspot", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"camera", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"microphone", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"location", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"external-storage", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"messaging", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"popimap-email", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"browser", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"settings-changes", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"bluetooth-tethering", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"clipboard", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"screen-capture", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"usb-debugging", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"usb-tethering", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"browser-autofill-setting", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"browser-cookies-setting", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"force-fraud-warning_setting", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"javascript-setting", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
	{"popup-setting", {POLICY_STATE_OPERATOR_ALLOW, "allowed"}},
};

}

PolicyManager::PolicyManager(const std::string& path) :
	location(path)
{
	runtime::File policyDirectory(location);

	if (policyDirectory.exists()) {
		if (policyDirectory.isDirectory()) {
			return;
		} else {
			policyDirectory.remove(true);
		}
	}

	initialize();
}

PolicyManager::~PolicyManager()
{
}

void PolicyManager::setEffectivePolicy(const std::string& policy, const std::string& state)
{
	std::string result = state;

	auto it = defaultPolicies.find(policy);
	if (it == defaultPolicies.end()) {
		throw runtime::Exception("invalid policy name");
	}

	policyStateOperator& stateOperator = it->second.first;
	ClientManager& clientManager = ClientManager::instance();

	for (Client& client : clientManager.getClients()) {
		PolicyStorage& policyStorage = client.getPolicyStorage();
		const std::string& newState = policyStorage.getPolicy(policy).getContent();

		switch (stateOperator) {
		case POLICY_STATE_OPERATOR_ALLOW:
			if (newState == "disallowed") {
				result = "disallowed";
			}
			break;
		case POLICY_STATE_OPERATOR_OR:
			result = std::to_string(stoi(state) | stoi(newState));
			break;
		case POLICY_STATE_OPERATOR_AND:
			result = std::to_string(stoi(state) & stoi(newState));
			break;
		case POLICY_STATE_OPERATOR_MIN:
			result = std::min(stoi(state), stoi(newState));
			break;
		case POLICY_STATE_OPERATOR_MAX:
			result = std::max(stoi(state), stoi(newState));
			break;
		default:
			result = newState;
			break;
		}
	}

	runtime::File file(location + "/" + policy);
	file.open(O_WRONLY | O_TRUNC);
	file.write(result.c_str(), result.size());
	file.close();
}

void PolicyManager::initialize()
{
	runtime::File policyDirectory(location);
	policyDirectory.makeDirectory(true);

	for (const auto& policy : defaultPolicies) {
		runtime::File file(location + "/" + policy.first);
		file.create(0640);
		file.close();

		setEffectivePolicy(policy.first, policy.second.second);
	}
}

const std::string PolicyManager::getPolicy(const std::string& name)
{
	runtime::File file(location + "/" + name);
	size_t size = file.size();

	std::unique_ptr<char[]> data(new char[size + 1]);
	data[size] = '\0';

	file.open(O_RDONLY);
	file.read(data.get(), size);
	file.close();

	return std::string(data.get());
}

void PolicyManager::setPolicy(Client& client, const std::string& name, const std::string& state)
{
	setEffectivePolicy(name, state);

	PolicyStorage& policyStorage = client.getPolicyStorage();
	policyStorage.getPolicy(name).setContent(state);
	policyStorage.flush();
}

void PolicyManager::remove()
{
	runtime::File policyDirectory(location);
	if (policyDirectory.exists()) {
		policyDirectory.remove(true);
	}
}

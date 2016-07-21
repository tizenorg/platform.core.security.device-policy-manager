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

const std::vector<std::pair<std::string, std::string>> defaultPolicies = {
	{"package-installation-mode", "allowed"},
	{"package-uninstallation-mode", "allowed"},
	{"password-quality", "0"},
	{"password-minimum-length", "0"},
	{"password-maximum-failure-count", "0"},
	{"password-expired", "0"},
	{"password-minimum-complexity", "0"},
	{"password-pattern", ""},
	{"password-history", "0"},
	{"password-recovery", "0"},
	{"password-lock-delay", "0"},
	{"password-inactivity-timeout", "1000"},
	{"password-status", "0"},
	{"password-change-timeout", "0"},
	{"password-maximum-character-occurrences", "0"},
	{"password-numeric-sequences-length", "0"},
	{"password-forbidden-strings", ""},
	{"internal-storage-encryption", ""},
	{"external-storage-encryption", ""},
	{"internet-sharing", "allowed"},
	{"bluetooth-pairing", "allowed"},
	{"bluetooth-outgoing-call", "allowed"},
	{"bluetooth-profile-a2dp", "allowed"},
	{"bluetooth-profile-avrcp", "allowed"},
	{"bluetooth-profile-bpp", "allowed"},
	{"bluetooth-profile-dun", "allowed"},
	{"bluetooth-profile-ftp", "allowed"},
	{"bluetooth-profile-hfp", "allowed"},
	{"bluetooth-profile-hsp", "allowed"},
	{"bluetooth-profile-pbap", "allowed"},
	{"bluetooth-profile-sap", "allowed"},
	{"bluetooth-profile-spp", "allowed"},
	{"bluetooth-desktop-connectivity", "allowed"},
	{"bluetooth-descoverable", "allowed"},
	{"bluetooth-limited-discoverable", "allowed"},
	{"bluetooth-data-transfer", "allowed"},
	{"bluetooth-uuid-restriction", "allowed"},
	{"bluetooth-device-restriction", "allowed"},
	{"wifi", "allowed"},
	{"wifi-hotspot", "allowed"},
	{"bluetooth", "allowed"},
	{"camera", "allowed"},
	{"microphone", "allowed"},
	{"location", "allowed"},
	{"external-storage", "allowed"},
	{"messaging", "allowed"},
	{"popimap-email", "allowed"},
	{"browser", "allowed"},
	{"settings-changes", "allowed"},
	{"bluetooth-tethering", "allowed"},
	{"clipboard", "allowed"},
	{"screen-capture", "allowed"},
	{"usb-debugging", "allowed"},
	{"usb-tethering", "allowed"},
	{"browser-autofill-setting", "allowed"},
	{"browser-cookies-setting", "allowed"},
	{"force-fraud-warning_setting", "allowed"},
	{"javascript-setting", "allowed"},
	{"popup-setting", "allowed"},
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

void PolicyManager::initialize()
{
	runtime::File policyDirectory(location);
	policyDirectory.makeDirectory(true);

	for (const auto& policy : defaultPolicies) {
		runtime::File file(location + "/" + policy.first);
		file.create(640);
		file.write(policy.second.c_str(), policy.second.size());
		file.close();
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

void PolicyManager::setPolicy(const std::string& name, const std::string& state)
{
	runtime::File file(location + "/" + name);
	file.open(O_WRONLY | O_TRUNC);
	file.write(state.c_str(), state.size());
	file.close();
}

void PolicyManager::remove()
{
	runtime::File policyDirectory(location);
	if (policyDirectory.exists()) {
		policyDirectory.remove(true);
	}
}

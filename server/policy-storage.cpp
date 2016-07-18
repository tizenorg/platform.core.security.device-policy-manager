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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "policy-storage.h"

#include "error.h"
#include "exception.h"
#include "audit/logger.h"

namespace {

const std::string defaultPolicyTemplate =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<manifest>\n"
"	<policy-version>0.1.0</policy-version>\n"
"	<policy-group name=\"application\">\n"
"		<policy name=\"package-installation-mode\">allowed</policy>\n"
"		<policy name=\"package-uninstallation-mode\">allowed</policy>\n"
"	</policy-group>\n"
"	<policy-group name=\"password\">\n"
"		<policy name=\"password-quality\">0</policy>\n"
"		<policy name=\"password-minimum-length\">0</policy>\n"
"		<policy name=\"password-maximum-failure-count\">0</policy>\n"
"		<policy name=\"password-expired\">0</policy>\n"
"		<policy name=\"password-minimum-complexity\">0</policy>\n"
"		<policy name=\"password-pattern\"></policy>\n"
"		<policy name=\"password-history\">0</policy>\n"
"		<policy name=\"password-recovery\">0</policy>\n"
"		<policy name=\"password-lock-delay\">0</policy>\n"
"		<policy name=\"password-inactivity-timeout\">1000</policy>\n"
"		<policy name=\"password-status\">0</policy>\n"
"		<policy name=\"password-change-timeout\">0</policy>\n"
"		<policy name=\"password-maximum-character-occurrences\">0</policy>\n"
"		<policy name=\"password-numeric-sequences-length\">0</policy>\n"
"		<policy name=\"password-forbidden-strings\"></policy>\n"
"	</policy-group>\n"
"	<policy-group name=\"security\">\n"
"		<policy name=\"internal-storage-encryption\"></policy>\n"
"		<policy name=\"external-storage-encryption\"></policy>\n"
"		<policy name=\"internet-sharing\">allowed</policy>\n"
"	</policy-group>\n"
"	<policy-group name=\"wifi\">\n"
"		<policy name=\"wifi-profile-change\">allowed</policy>\n"
"		<policy name=\"wifi-ssid-restriction\">disabled</policy>\n"
"	</policy-group>\n"
"	<policy-group name=\"bluetooth\">\n"
"		<policy name=\"bluetooth-pairing\">allowed</policy>\n"
"		<policy name=\"bluetooth-outgoing-call\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-a2dp\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-avrcp\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-bpp\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-dun\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-ftp\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-hfp\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-hsp\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-pbap\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-sap\">allowed</policy>\n"
"		<policy name=\"bluetooth-profile-spp\">allowed</policy>\n"
"		<policy name=\"bluetooth-desktop-connectivity\">allowed</policy>\n"
"		<policy name=\"bluetooth-descoverable\">allowed</policy>\n"
"		<policy name=\"bluetooth-limited-discoverable\">allowed</policy>\n"
"		<policy name=\"bluetooth-data-transfer\">allowed</policy>\n"
"		<policy name=\"bluetooth-uuid-restriction\">disabled</policy>\n"
"		<policy name=\"bluetooth-device-restriction\">disabled</policy>\n"
"	</policy-group>\n"
"	<policy-group name=\"restriction\">\n"
"		<policy name=\"wifi\">allowed</policy>\n"
"		<policy name=\"wifi-hotspot\">allowed</policy>\n"
"		<policy name=\"bluetooth\">allowed</policy>\n"
"		<policy name=\"camera\">allowed</policy>\n"
"		<policy name=\"microphone\">allowed</policy>\n"
"		<policy name=\"location\">allowed</policy>\n"
"		<policy name=\"external-storage\">allowed</policy>\n"
"		<policy name=\"messaging\">allowed</policy>\n"
"		<policy name=\"popimap-email\">allowed</policy>\n"
"		<policy name=\"browser\">allowed</policy>\n"
"		<policy name=\"settings-changes\">allowed</policy>\n"
"		<policy name=\"tethering\">allowed</policy>\n"
"		<policy name=\"bluetooth-tethering\">allowed</policy>\n"
"		<policy name=\"clipboard\">allowed</policy>\n"
"		<policy name=\"screen-capture\">allowed</policy>\n"
"		<policy name=\"usb-debugging\">allowed</policy>\n"
"		<policy name=\"usb-tethering\">allowed</policy>\n"
"	</policy-group>\n"
"	<policy-group name=\"browser\">\n"
"		<policy name=\"browser-autofill-setting\">allowed</policy>\n"
"		<policy name=\"browser-cookies-setting\">allowed</policy>\n"
"		<policy name=\"force-fraud-warning_setting\">allowed</policy>\n"
"		<policy name=\"javascript-setting\">allowed</policy>\n"
"		<policy name=\"popup-setting\">allowed</policy>\n"
"	</policy-group>\n"
"</manifest>\n";

}

PolicyStorage::PolicyStorage(const std::string& path, bool create) :
	location(path),
	data(nullptr)
{
	// TODO(seok85.hong): refactoring check logic
	//
	bool useDefaultPolicyTemplate = false;

	if (create) {
		struct stat st;
		if ((stat(location.c_str(), &st) == -1)) {
			if (errno == ENOENT) {
				useDefaultPolicyTemplate = true;
			} else {
				throw runtime::Exception(runtime::GetSystemErrorMessage());
			}
		}
	}

	if (useDefaultPolicyTemplate) {
		data = std::unique_ptr<xml::Document>(xml::Parser::parseString(defaultPolicyTemplate));
	}
	else
		data = std::unique_ptr<xml::Document>(xml::Parser::parseFile(location));

	xml::Node::NodeList nodes = data->evaluate("/manifest/policy-group/policy");
	xml::Node::NodeList::iterator it = nodes.begin();
	while (it != nodes.end()) {
		policyMap.emplace(it->getProp("name"), std::move(*it));
		++it;
	}

	if (useDefaultPolicyTemplate)
		flush();
}

PolicyStorage::~PolicyStorage()
{
}

Policy& PolicyStorage::getPolicy(const std::string& name)
{
	if (policyMap.count(name) == 0) {
		throw runtime::Exception("Failed to find policy");
	}

	return policyMap.at(name);
}

void PolicyStorage::flush()
{
	data->write(location, "UTF-8", true);
}

void PolicyStorage::remove()
{
	if (::unlink(location.c_str()) == -1)
		::unlink(location.c_str());
}
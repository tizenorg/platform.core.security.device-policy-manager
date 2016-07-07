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

const std::string defaultPolicyTemplate = CONF_PATH "/policy/PolicyManifest.xml";

}

PolicyStorage::PolicyStorage(const std::string& path, bool create) :
	location(path),
	data(nullptr)
{
	std::string& source = location;
	if (create) {
		struct stat st;
		if ((stat(location.c_str(), &st) == -1)) {
			if (errno == ENOENT) {
				source = defaultPolicyTemplate;
			} else {
				throw runtime::Exception(runtime::GetSystemErrorMessage());
			}
		}
	}

	data = std::unique_ptr<xml::Document>(xml::Parser::parseFile(source));
	xml::Node::NodeList nodes = data->evaluate("/manifest/policy-group/policy");
	xml::Node::NodeList::iterator it = nodes.begin();
	while (it != nodes.end()) {
		policyMap.emplace(it->getProp("name"), std::move(*it));
		++it;
	}
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

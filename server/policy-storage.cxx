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

#include <memory>
#include <stdexcept>
#include <string>

#include "policy-storage.hxx"
#include "error.hxx"
#include "exception.hxx"

namespace DevicePolicyServer {

namespace {

const std::string defaultPolicyTemplate = CONF_PATH "/policy/PolicyManifest.xml";

}

PolicyStorage::PolicyStorage(const std::string& path, bool create)
    : location(path),
      data(nullptr)
{
    std::string& source = location;

    if (create) {
        struct stat st;
        if ((stat(location.c_str(), &st) == -1)) {
            if (errno == ENOENT) {
                source = defaultPolicyTemplate;
            } else {
                throw Runtime::Exception(Runtime::GetSystemErrorMessage());
            }
        }
    }

    data = std::unique_ptr<Xml::Document>(Xml::Parser::parseFile(source));
}

PolicyStorage::~PolicyStorage()
{
}

} // namespace DevicePolicyServer

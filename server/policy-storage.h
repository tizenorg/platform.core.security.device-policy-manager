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

#ifndef __DPM_POLICY_STORAGE_H__
#define __DPM_POLICY_STORAGE_H__

#include <memory>
#include <stdexcept>
#include <string>

#include "policy.h"

#include "xml/parser.h"
#include "xml/document.h"
#include "xml/node.h"

typedef xml::Node::NodeList PolicyGroupList;
typedef xml::Node PolicyData;

class PolicyStorage {
public:
    PolicyStorage(const std::string& path, bool create = true);
    ~PolicyStorage();

    PolicyStorage(const PolicyStorage&) = delete;
    PolicyStorage& operator=(const PolicyStorage&) = delete;

    Policy evaluate(const std::string& expression);
    PolicyData getPolicyData(const std::string& name);

    PolicyGroupList loadAllPolicies();

    void flush();

private:
    std::string location;
    std::unique_ptr<xml::Document> data;
};

#endif //__DPM_POLICY_STORAGE_H__

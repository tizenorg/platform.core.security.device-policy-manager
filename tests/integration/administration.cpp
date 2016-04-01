// Copyright (c) 2015 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "administration.hxx"

#include "policy-client.h"
#include "testbench/testbench.h"

#include "audit/logger.h"

namespace {

const char* pkgName = "dpm_test_admin_client";

}; //namespace

TESTCASE(AdministrationInterfaceTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Administration admin = client.createPolicyInterface<DevicePolicyManager::Administration>();

    TEST_EXPECT(0, admin.registerPolicyClient(pkgName));
    TEST_EXPECT(0, admin.deregisterPolicyClient(pkgName));
}

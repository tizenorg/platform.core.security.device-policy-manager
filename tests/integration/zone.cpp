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

#include <unistd.h>
#include <sys/types.h>

#include "zone.hxx"

#include "policy-client.h"
#include "audit/logger.h"
#include "testbench/testbench.h"

const std::string testSetupWizardAppid = "org.tizen.zone-setup-wizard";
const std::string testZonePolicyName = "zone1";

TESTCASE(ZonePolicyCreateTest)
{
    DevicePolicyContext client;

    if (getuid() == 0) {
        TEST_FAIL("this test should be done as non-root user");
    }

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::ZonePolicy zone = client.createPolicyInterface<DevicePolicyManager::ZonePolicy>();

    int error = zone.createZonePolicy(testZonePolicyName, testSetupWizardAppid);
    TEST_EXPECT(0, error);
}

TESTCASE(GetZonePolicyListTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::ZonePolicy zone = client.createPolicyInterface<DevicePolicyManager::ZonePolicy>();

    std::vector<std::string> list = zone.getZonePolicyList();

    for (std::string &word : list) {
        if (word == testZonePolicyName) {
            return;
        }
    }

    TEST_FAIL("list doesn't contain the created zone name");
}

TESTCASE(GetZonePolicyStateTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::ZonePolicy zone = client.createPolicyInterface<DevicePolicyManager::ZonePolicy>();

    zone.getZonePolicyState(testZonePolicyName);

    //TODO : should implement checking if the zone exists
}

TESTCASE(ZonePolicyRemoveTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::ZonePolicy zone = client.createPolicyInterface<DevicePolicyManager::ZonePolicy>();

    int error = zone.removeZonePolicy(testZonePolicyName);
    TEST_EXPECT(0, error);
}

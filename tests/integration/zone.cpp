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
const std::string testZoneName = "zone1";

TESTCASE(ZoneCreateTest)
{
    DevicePolicyContext client;

    if (getuid() == 0) {
        TEST_FAIL("this test should be done as non-root user");
    }

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Zone zone = client.createPolicyInterface<DevicePolicyManager::Zone>();

    int error = zone.createZone(testZoneName, testSetupWizardAppid);
    TEST_EXPECT(0, error);
}

TESTCASE(GetZoneListTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Zone zone = client.createPolicyInterface<DevicePolicyManager::Zone>();

    std::vector<std::string> list = zone.getZoneList();

    for (std::string &word : list) {
        if (word == testZoneName) {
            return;
        }
    }

    TEST_FAIL("list doesn't contain the created zone name");
}

TESTCASE(GetZoneStateTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Zone zone = client.createPolicyInterface<DevicePolicyManager::Zone>();

    zone.getZoneState(testZoneName);

    //TODO : should implement checking if the zone exists
}

TESTCASE(ZoneRemoveTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Zone zone = client.createPolicyInterface<DevicePolicyManager::Zone>();

    int error = zone.removeZone(testZoneName);
    TEST_EXPECT(0, error);
}

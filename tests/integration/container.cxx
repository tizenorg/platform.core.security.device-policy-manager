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

#include "policy-client.hxx"
#include "container.hxx"
#include "testbench/testbench.hxx"

#include "audit/logger.hxx"

const std::string testSetupWizardAppid = "org.tizen.container-setup-wizard";
const std::string testContainerName = "container1";

TESTCASE(ContainerCreateTest)
{
    DevicePolicyClient client;

    if (getuid() == 0) {
        TEST_FAIL("this test should be done as non-root user");
    }

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Container container = client.createPolicyInterface<DevicePolicyManager::Container>();

    int error = container.create(testContainerName, testSetupWizardAppid);
    TEST_EXPECT(0, error);
}

TESTCASE(GetContainerListTest)
{
    DevicePolicyClient client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Container container = client.createPolicyInterface<DevicePolicyManager::Container>();

    Vector<String> list = container.getList();

    for (String & word : list.value) {
        if (word.value == testContainerName) {
            return;
        }
    }

    TEST_FAIL("list doesn't contain the created container name");
}

TESTCASE(ContainerRemoveTest)
{
    DevicePolicyClient client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Container container = client.createPolicyInterface<DevicePolicyManager::Container>();

    int error = container.remove(testContainerName);
    TEST_EXPECT(0, error);
}

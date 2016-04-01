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

#include "security.hxx"

#include "policy-client.h"
#include "testbench/testbench.h"

#include "audit/logger.h"

TESTCASE(ScreenLockTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    int error = security.lockoutScreen();
    TEST_EXPECT(0, error);
}

TESTCASE(InternalMemoryEncryptionTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    int error = security.setInternalStorageEncryption(true);
    TEST_EXPECT(0, error);
}

TESTCASE(ExternalMemoryEncryptionTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    int error = security.setExternalStorageEncryption(true);
    TEST_EXPECT(0, error);
}

TESTCASE(DeviceWipeTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    int error = security.wipeData(DevicePolicyManager::SecurityPolicy::WIPE_INTERNAL_STORAGE);
    TEST_EXPECT(0, error);
}

TESTCASE(ExternalMemoryWipeTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    int error = security.wipeData(DevicePolicyManager::SecurityPolicy::WIPE_EXTERNAL_STORAGE);
    TEST_EXPECT(0, error);
}

TESTCASE(EmptyDirectoryIterationTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    std::vector<std::string> files = security.getFileNamesOnDevice("/etc");
    TEST_EXPECT(true, files.empty());

    std::vector<std::string> filesWithAttr = security.getFileNamesWithAttributes("/etc");
    TEST_EXPECT(true, filesWithAttr.empty());
}

TESTCASE(NonemptyDirectoryIterationTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    std::vector<std::string> files = security.getFileNamesOnDevice("/etc");
    TEST_EXPECT(false, files.empty());

    std::vector<std::string> filesWithAttr = security.getFileNamesWithAttributes("/etc");
    TEST_EXPECT(false, filesWithAttr.empty());
}

TESTCASE(InvalidDirectoryIterationTest)
{
    DevicePolicyContext client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::SecurityPolicy security = client.createPolicyInterface<DevicePolicyManager::SecurityPolicy>();

    std::vector<std::string> files = security.getFileNamesOnDevice("/invalid");
    TEST_EXPECT(true, files.empty());

    std::vector<std::string> filesWithAttr = security.getFileNamesWithAttributes("/invalid");
    TEST_EXPECT(true, filesWithAttr.empty());
}

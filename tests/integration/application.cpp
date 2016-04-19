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

#include "policy-client.h"
#include "application.hxx"

#include "audit/logger.h"
#include "testbench/testbench.h"

namespace {

const std::string TestAppId = "org.tizen.music-player";

}; //namespace

TESTCASE(AppInfoTest)
{
    DevicePolicyClient client;
    std::cout << "Begin connection" << std::endl;
    TEST_EXPECT(0, client.connect());

    std::cout << "End connection" << std::endl;
    DevicePolicyManager::Application app = client.createPolicyInterface<DevicePolicyManager::Application>();

    bool ret = app.isApplicationInstalled(TestAppId);
    TEST_EXPECT(true, ret);

    ret = app.isApplicationInstalled("unknown-application");
    TEST_EXPECT(false, ret);
}

TESTCASE(PkgInfoTest)
{
    DevicePolicyClient client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Application app = client.createPolicyInterface<DevicePolicyManager::Application>();

    bool ret = app.isPackageInstalled(TestAppId);
    TEST_EXPECT(true, ret);

    ret = app.isPackageInstalled("unknown-package");
    TEST_EXPECT(false, ret);

    std::cout << "Iterate installed packages" << std::endl;
    std::vector<std::string> packages = app.getInstalledPackageList();
    for (std::string& pkg : packages) {
        std::cout << pkg << std::endl;
    }
}

TESTCASE(AppRunStateTest)
{
    DevicePolicyClient client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Application app = client.createPolicyInterface<DevicePolicyManager::Application>();

    int ret = app.startApplication(TestAppId);
    TEST_EXPECT(0, ret);

    ret = app.isApplicationRunning(TestAppId);
    TEST_EXPECT(true, ret);

    ret = app.isApplicationRunning("unknown-application");
    TEST_EXPECT(false, ret);
}

TESTCASE(AppActivationTest)
{
    DevicePolicyClient client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Application app = client.createPolicyInterface<DevicePolicyManager::Application>();

    int ret = app.disableApplication(TestAppId);
    TEST_EXPECT(0, ret);
}

TESTCASE(AppStartTest)
{
    DevicePolicyClient client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Application app = client.createPolicyInterface<DevicePolicyManager::Application>();

    int ret = app.startApplication(TestAppId);
    TEST_EXPECT(0, ret);
    sleep(3);
    ret = app.stopApplication(TestAppId);
    TEST_EXPECT(0, ret);
}

TESTCASE(BlacklistTest)
{
    DevicePolicyClient client;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::Application app = client.createPolicyInterface<DevicePolicyManager::Application>();

    int ret = app.addPackageToBlacklist("org.tizen.ode");
    std::cout << "Add org.tizen.ode to blacklist: " << ret << std::endl;

    ret = app.checkPackageIsBlacklisted("org.tizen.ode");
    std::cout << "Blacklist status for org.tizen.ode: " << ret << std::endl;

    ret = app.removePackageFromBlacklist("org.tizen.ode");
    std::cout << "Remove org.tizen.ode from blacklist " << ret << std::endl;
}

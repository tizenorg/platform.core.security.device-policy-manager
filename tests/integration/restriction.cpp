/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "policy-client.h"
#include "restriction.hxx"
#include "testbench/testbench.h"

#include "audit/logger.h"

TESTCASE(ClipboardTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.allowClipboard(true);
	TEST_EXPECT(0, error);

	status = restriction.isClipboardAllowed();
	TEST_EXPECT(true, status);

	error = restriction.allowClipboard(false);
	TEST_EXPECT(0, error);

	status = restriction.isClipboardAllowed();
	TEST_EXPECT(false, status);
}

TESTCASE(SettingsChangesTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.allowSettingsChanges(true);
	TEST_EXPECT(0, error);

	status = restriction.isSettingsChangesAllowed();
	TEST_EXPECT(true, status);

	error = restriction.allowSettingsChanges(false);
	TEST_EXPECT(0, error);

	status = restriction.isSettingsChangesAllowed();
	TEST_EXPECT(false, status);
}

TESTCASE(SettingsBackgroundData)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.allowBackgroundData(true);
	TEST_EXPECT(0, error);

	status = restriction.isBackgroundDataAllowed();
	TEST_EXPECT(true, status);

	error = restriction.allowBackgroundData(false);
	TEST_EXPECT(0, error);

	status = restriction.isBackgroundDataAllowed();
	TEST_EXPECT(false, status);
}

TESTCASE(UsbDebuggingTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.allowUsbDebugging(true);
	TEST_EXPECT(0, error);

	status = restriction.isUsbDebuggingAllowed();
	TEST_EXPECT(true, status);

	error = restriction.allowUsbDebugging(false);
	TEST_EXPECT(0, error);

	status = restriction.isUsbDebuggingAllowed();
	TEST_EXPECT(false, status);
}

TESTCASE(FactoryResetTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.allowFactoryReset(true);
	TEST_EXPECT(0, error);

	status = restriction.isFactoryResetAllowed();
	TEST_EXPECT(true, status);

	error = restriction.allowFactoryReset(false);
	TEST_EXPECT(0, error);

	status = restriction.isFactoryResetAllowed();
	TEST_EXPECT(false, status);
}

TESTCASE(BluetoothTethering)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.allowBluetoothTethering(true);
	TEST_EXPECT(0, error);

	status = restriction.isBluetoothTetheringAllowed();
	TEST_EXPECT(true, status);

	error = restriction.allowBluetoothTethering(false);
	TEST_EXPECT(0, error);

	status = restriction.isBluetoothTetheringAllowed();
	TEST_EXPECT(false, status);
}

TESTCASE(MockLocation)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.allowMockLocation(true);
	TEST_EXPECT(0, error);

	status = restriction.isMockLocationAllowed();
	TEST_EXPECT(true, status);

	error = restriction.allowMockLocation(false);
	TEST_EXPECT(0, error);

	status = restriction.isMockLocationAllowed();
	TEST_EXPECT(false, status);
}

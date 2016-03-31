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

TESTCASE(SetAllowClipboardTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowClipboard(true);
	TEST_EXPECT(0, error);

	error = restriction.setAllowClipboard(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowClipboardTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowClipboard(true);
	TEST_EXPECT(0, error);

	status = restriction.getAllowClipboard();
	TEST_EXPECT(true, status);

	error = restriction.setAllowClipboard(false);
	TEST_EXPECT(0, error);

	status = restriction.getAllowClipboard();
	TEST_EXPECT(false, status);
}

TESTCASE(SetAllowSettinsChangesTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowSettingsChanges(true);
	TEST_EXPECT(0, error);

	error = restriction.setAllowSettingsChanges(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowSettingsChangesTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowSettingsChanges(true);
	TEST_EXPECT(0, error);

	status = restriction.getAllowSettingsChanges();
	TEST_EXPECT(true, status);

	error = restriction.setAllowSettingsChanges(false);
	TEST_EXPECT(0, error);

	status = restriction.getAllowSettingsChanges();
	TEST_EXPECT(false, status);
}

TESTCASE(SetAllowBackgroundDataTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowBackgroundData(true);
	TEST_EXPECT(0, error);

	error = restriction.setAllowBackgroundData(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowSettingsBackgroundData)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowBackgroundData(true);
	TEST_EXPECT(0, error);

	status = restriction.getAllowBackgroundData();
	TEST_EXPECT(true, status);

	error = restriction.setAllowBackgroundData(false);
	TEST_EXPECT(0, error);

	status = restriction.getAllowBackgroundData();
	TEST_EXPECT(false, status);
}

TESTCASE(SetAllowUsbDebuggingTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowUsbDebugging(true);
	TEST_EXPECT(0, error);

	error = restriction.setAllowUsbDebugging(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowUsbDebuggingTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowUsbDebugging(true);
	TEST_EXPECT(0, error);

	status = restriction.getAllowUsbDebugging();
	TEST_EXPECT(true, status);

	error = restriction.setAllowUsbDebugging(false);
	TEST_EXPECT(0, error);

	status = restriction.getAllowUsbDebugging();
	TEST_EXPECT(false, status);
}

TESTCASE(SetAllowFactoryResetTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowFactoryReset(true);
	TEST_EXPECT(0, error);

	error = restriction.setAllowFactoryReset(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowFactoryResetTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowFactoryReset(true);
	TEST_EXPECT(0, error);

	status = restriction.getAllowFactoryReset();
	TEST_EXPECT(true, status);

	error = restriction.setAllowFactoryReset(false);
	TEST_EXPECT(0, error);

	status = restriction.getAllowFactoryReset();
	TEST_EXPECT(false, status);
}

TESTCASE(SetAllowBluetoothTethering)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowBluetoothTethering(true);
	TEST_EXPECT(0, error);

	error = restriction.setAllowBluetoothTethering(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowBluetoothTethering)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowBluetoothTethering(true);
	TEST_EXPECT(0, error);

	status = restriction.getAllowBluetoothTethering();
	TEST_EXPECT(true, status);

	error = restriction.setAllowBluetoothTethering(false);
	TEST_EXPECT(0, error);

	status = restriction.getAllowBluetoothTethering();
	TEST_EXPECT(false, status);
}

TESTCASE(SetAllowMockLocation)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowMockLocation(true);
	TEST_EXPECT(0, error);

	error = restriction.setAllowMockLocation(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowMockLocation)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setAllowMockLocation(true);
	TEST_EXPECT(0, error);

	status = restriction.getAllowMockLocation();
	TEST_EXPECT(true, status);

	error = restriction.setAllowMockLocation(false);
	TEST_EXPECT(0, error);

	status = restriction.getAllowMockLocation();
	TEST_EXPECT(false, status);
}

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

	error = restriction.setClipboardRestriction(true);
	TEST_EXPECT(0, error);

	status = restriction.isClipboardRestricted();
	TEST_EXPECT(true, status);

	error = restriction.setClipboardRestriction(false);
	TEST_EXPECT(0, error);

	status = restriction.isClipboardRestricted();
	TEST_EXPECT(false, status);
}

TESTCASE(SettingsChangesTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setSettingsChangesRestriction(true);
	TEST_EXPECT(0, error);

	status = restriction.isSettingsChangesRestricted();
	TEST_EXPECT(true, status);

	error = restriction.setSettingsChangesRestriction(false);
	TEST_EXPECT(0, error);

	status = restriction.isSettingsChangesRestricted();
	TEST_EXPECT(false, status);
}

TESTCASE(UsbDebuggingTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.setUsbDebuggingRestriction(true);
	TEST_EXPECT(0, error);

	status = restriction.isUsbDebuggingRestricted();
	TEST_EXPECT(true, status);

	error = restriction.setUsbDebuggingRestriction(false);
	TEST_EXPECT(0, error);

	status = restriction.isUsbDebuggingRestricted();
	TEST_EXPECT(false, status);
}
/*
TESTCASE(WipeData)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Restriction restriction = client.createPolicyInterface<DevicePolicyManager::Restriction>();

	error = restriction.wipeData(true);
	TEST_EXPECT(0, error);
}
*/
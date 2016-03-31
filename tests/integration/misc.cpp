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
#include "testbench/testbench.h"
#include "audit/logger.h"
#include "misc.hxx"

TESTCASE(SetAllowCameraTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowCamera(true);
	TEST_EXPECT(0, error);

	error = misc.setAllowCamera(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowCameraTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowCamera(true);
	TEST_EXPECT(0, error);

	status = misc.getAllowCamera();
	TEST_EXPECT(true, status);
}

TESTCASE(SetAllowMicrophoneTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowMicrophone(true);
	TEST_EXPECT(0, error);

	error = misc.setAllowMicrophone(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowMicrophoneTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowMicrophone(true);
	TEST_EXPECT(0, error);

	status = misc.getAllowMicrophone();
	TEST_EXPECT(true, status);
}

TESTCASE(SetAllowLocationTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowLocation(true);
	TEST_EXPECT(0, error);

	error = misc.setAllowLocation(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowLocationTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowLocation(true);
	TEST_EXPECT(0, error);

	status = misc.getAllowLocation();
	TEST_EXPECT(true, status);
}

TESTCASE(SetAllowSDCardTest)
{
	DevicePolicyClient client;
	int error = -1;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowSDCard(true);
	TEST_EXPECT(0, error);

	error = misc.setAllowSDCard(false);
	TEST_EXPECT(0, error);
}

TESTCASE(GetAllowSDCardTest)
{
	DevicePolicyClient client;
	int error = -1;
	bool status = false;

	TEST_EXPECT(0, client.connect());

	DevicePolicyManager::Misc misc = client.createPolicyInterface<DevicePolicyManager::Misc>();

	error = misc.setAllowSDCard(true);
	TEST_EXPECT(0, error);

	status = misc.getAllowSDCard();
	TEST_EXPECT(true, status);
}

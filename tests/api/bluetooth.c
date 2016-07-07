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

#include <stdio.h>

#include <dpm/bluetooth.h>

#include "testbench.h"

static int bluetooth_add_device_to_blacklist(struct testcase* tc)
{
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	const char* dummy_mac = "e8:11:32:30:78:9c";
	if (dpm_bluetooth_add_device_to_blacklist(handle, dummy_mac) != 0) {
		printf("Failed to add device from blacklist\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);

	return TEST_SUCCESSED;
}

static int bluetooth_remove_device_from_blacklist(struct testcase* tc)
{
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	const char* dummy_mac = "e8:11:32:30:78:9c";
	if (dpm_bluetooth_remove_device_from_blacklist(handle, dummy_mac) != 0) {
		printf("Failed to remove device from blacklist\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

static int bluetooth_device_restriction(struct testcase* tc)
{
	int allow = false;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_bluetooth_set_device_restriction(handle, true) != 0) {
		printf("Failed to set device restriction\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	if (dpm_bluetooth_is_device_restricted(handle, &allow) != DPM_ERROR_NONE) {
		printf("Failed to check device restriction\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

static int bluetooth_add_uuid_to_blacklist(struct testcase* tc)
{
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	const char* dummy_uuid = "ff8ca1f3-0221-40c9-91fd-25ebbbfa68c3";
	if (dpm_bluetooth_add_uuid_to_blacklist(handle, dummy_uuid) != 0) {
		printf("Failed to add uuid to blacklist\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

static int bluetooth_remove_uuid_from_blacklist(struct testcase* tc)
{
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	const char* dummy_uuid = "ff8ca1f3-0221-40c9-91fd-25ebbbfa68c3";
	if (dpm_bluetooth_remove_uuid_from_blacklist(handle, dummy_uuid) != 0) {
		printf("Failed to remove uuid from blacklist\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

static int bluetooth_uuid_restriction(struct testcase* tc)
{
	int allow = false;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_bluetooth_set_uuid_restriction(handle, true) != 0) {
		printf("Failed to set uuid restriction\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	if (dpm_bluetooth_is_uuid_restricted(handle, &allow) != DPM_ERROR_NONE) {
		printf("Failed to check uuid restriction\n");
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

struct testcase bluetooth_testcase_add_device_to_blacklist = {
	.description = "dpm_add_device_to_blacklist",
	.handler = bluetooth_add_device_to_blacklist
};

struct testcase bluetooth_testcase_remove_device_from_blacklist = {
	.description = "dpm_remove_device_from_blacklist",
	.handler = bluetooth_remove_device_from_blacklist
};

struct testcase bluetooth_testcase_device_restriction = {
	.description = "dpm_device_restriction",
	.handler = bluetooth_device_restriction
};

struct testcase bluetooth_testcase_add_uuid_to_blacklist = {
	.description = "dpm_add_uuid_to_blacklist",
	.handler = bluetooth_add_uuid_to_blacklist
};

struct testcase bluetooth_testcase_remove_uuid_from_blacklist = {
	.description = "dpm_remove_uuid_from_blacklist",
	.handler = bluetooth_remove_uuid_from_blacklist
};

struct testcase bluetooth_testcase_uuid_restriction = {
	.description = "dpm_uuid_restriction",
	.handler = bluetooth_uuid_restriction
};

void TESTCASE_CONSTRUCTOR bluetooth_policy_build_testcase(void)
{
	testbench_populate_testcase(&bluetooth_testcase_add_device_to_blacklist);
	testbench_populate_testcase(&bluetooth_testcase_remove_device_from_blacklist);
	testbench_populate_testcase(&bluetooth_testcase_device_restriction);
	testbench_populate_testcase(&bluetooth_testcase_add_uuid_to_blacklist);
	testbench_populate_testcase(&bluetooth_testcase_remove_uuid_from_blacklist);
	testbench_populate_testcase(&bluetooth_testcase_uuid_restriction);
}

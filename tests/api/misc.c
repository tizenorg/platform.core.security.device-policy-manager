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
#include <stdbool.h>
#include <dpm/misc.h>
#include "testbench.h"

static int misc_camera(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_camera_restriction(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_is_camera_restricted(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_microphone(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_microphone_restriction(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_is_microphone_restricted(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_location(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_location_restriction(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_is_location_restricted(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_sd_card(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_sd_card_restriction(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_is_sd_card_restricted(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

struct testcase misc_testcase_camera = {
	.description = "dpm_camera",
	.handler = misc_camera
};

struct testcase misc_testcase_microphone = {
	.description = "dpm_microphone",
	.handler = misc_microphone
};

struct testcase misc_testcase_location = {
	.description = "dpm_location",
	.handler = misc_location
};

struct testcase misc_testcase_sd_card = {
	.description = "dpm_sd_card",
	.handler = misc_sd_card
};

void TESTCASE_CONSTRUCTOR misc_policy_build_testcase(void)
{
	testbench_populate_testcase(&misc_testcase_camera);
	testbench_populate_testcase(&misc_testcase_microphone);
	testbench_populate_testcase(&misc_testcase_location);
	testbench_populate_testcase(&misc_testcase_sd_card);
}

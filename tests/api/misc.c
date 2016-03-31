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

static int misc_set_allow_camera(struct testcase *tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_camera(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	if (dpm_set_allow_camera(handle, false) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_get_allow_camera(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_camera(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_camera(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_set_allow_microphone(struct testcase *tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_microphone(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	if (dpm_set_allow_microphone(handle, false) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_get_allow_microphone(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_microphone(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_microphone(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_set_allow_location(struct testcase *tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_location(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	if (dpm_set_allow_location(handle, false) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_get_allow_location(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_location(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_location(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_set_allow_sd_card(struct testcase *tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_sd_card(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	if (dpm_set_allow_sd_card(handle, false) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int misc_get_allow_sd_card(struct testcase *tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_sd_card(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_sd_card(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

struct testcase misc_testcase_set_allow_camera = {
	.description = "dpm_set_allow_camera",
	.handler = misc_set_allow_camera
};

struct testcase misc_testcase_get_allow_camera = {
	.description = "dpm_get_allow_camera",
	.handler = misc_get_allow_camera
};

struct testcase misc_testcase_set_allow_microphone = {
	.description = "dpm_set_allow_microphone",
	.handler = misc_set_allow_microphone
};

struct testcase misc_testcase_get_allow_microphone = {
	.description = "dpm_get_allow_microphone",
	.handler = misc_get_allow_microphone
};

struct testcase misc_testcase_set_allow_location = {
	.description = "dpm_set_allow_location",
	.handler = misc_set_allow_location
};

struct testcase misc_testcase_get_allow_location = {
	.description = "dpm_get_allow_location",
	.handler = misc_get_allow_location
};

struct testcase misc_testcase_set_allow_sd_card = {
	.description = "dpm_set_allow_sd_card",
	.handler = misc_set_allow_sd_card
};

struct testcase misc_testcase_get_allow_sd_card = {
	.description = "dpm_get_allow_sd_card",
	.handler = misc_get_allow_sd_card
};

void TESTCASE_CONSTRUCTOR misc_policy_build_testcase(void)
{
	testbench_populate_testcase(&misc_testcase_set_allow_camera);
	testbench_populate_testcase(&misc_testcase_get_allow_camera);

	testbench_populate_testcase(&misc_testcase_set_allow_microphone);
	testbench_populate_testcase(&misc_testcase_get_allow_microphone);

	testbench_populate_testcase(&misc_testcase_set_allow_location);
	testbench_populate_testcase(&misc_testcase_get_allow_location);

	testbench_populate_testcase(&misc_testcase_set_allow_sd_card);
	testbench_populate_testcase(&misc_testcase_get_allow_sd_card);
}

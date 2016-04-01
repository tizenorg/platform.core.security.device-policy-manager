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
#include <dpm/restriction.h>
#include "testbench.h"

static int test_dpm_clipboard(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_clipboard_set_state(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_clipboard_get_state(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}


static int test_dpm_settings_changes(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_settings_set_restriction(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_settings_is_restricted(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int test_dpm_usb_debugging(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_developer_set_usb_debugging_state(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_developer_get_usb_debugging_state(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int test_dpm_usb_mass_storage(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_storage_set_usb_mass_storage_state(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_storage_get_usb_mass_storage_state(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int test_dpm_wipe(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_device_wipe_data(handle) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

struct testcase restriction_testcase_clipboard = {
	.description = "dpm_clipboard",
	.handler = test_dpm_clipboard
};

struct testcase restriction_testcase_settings_changes = {
	.description = "dpm_settings_changes",
	.handler = test_dpm_settings_changes
};

struct testcase restriction_testcase_usb_debugging = {
	.description = "dpm_usb_debugging",
	.handler = test_dpm_usb_debugging
};

struct testcase restriction_testcase_usb_mass_storage = {
	.description = "dpm_usb_mass_storage",
	.handler = test_dpm_usb_mass_storage
};

struct testcase restriction_testcase_wipe = {
	.description = "dpm_wipe",
	.handler = test_dpm_wipe
};

void TESTCASE_CONSTRUCTOR restriction_policy_build_testcase(void)
{
	testbench_populate_testcase(&restriction_testcase_clipboard);
	testbench_populate_testcase(&restriction_testcase_settings_changes);
	testbench_populate_testcase(&restriction_testcase_usb_debugging);
	testbench_populate_testcase(&restriction_testcase_usb_mass_storage);
	//testbench_populate_testcase(&restriction_testcase_wipe);
}

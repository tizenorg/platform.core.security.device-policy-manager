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

static int restriction_clipboard(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_clipboard(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_clipboard_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_settings_changes(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_settings_changes(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_settings_changes_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_background_data(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_background_data(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_background_data_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_usb_debugging(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_usb_debugging(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_usb_debugging_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_usb_mass_storage(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_usb_mass_storage(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_usb_mass_storage_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_factory_reset(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_factory_reset(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_factory_reset_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_bluetooth_tethering(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_bluetooth_tethering(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_bluetooth_tethering_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_mock_location(struct testcase* tc)
{
	dpm_client_h handle;
	bool allow = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_allow_mock_location(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	allow = dpm_is_mock_location_allowed(handle);
	if (allow == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

struct testcase restriction_testcase_clipboard = {
	.description = "dpm_clipboard",
	.handler = restriction_clipboard
};

struct testcase restriction_testcase_settings_changes = {
	.description = "dpm_settings_changes",
	.handler = restriction_settings_changes
};

struct testcase restriction_testcase_background_data = {
	.description = "dpm_background_data",
	.handler = restriction_background_data
};

struct testcase restriction_testcase_usb_debugging = {
	.description = "dpm_usb_debugging",
	.handler = restriction_usb_debugging
};

struct testcase restriction_testcase_usb_mass_storage = {
	.description = "dpm_usb_mass_storage",
	.handler = restriction_usb_mass_storage
};

struct testcase restriction_testcase_factory_reset = {
	.description = "dpm_factory_reset",
	.handler = restriction_factory_reset
};

struct testcase restriction_testcase_bluetooth_tethering = {
	.description = "dpm_bluetooth_tethering",
	.handler = restriction_bluetooth_tethering
};

struct testcase restriction_testcase_mock_location = {
	.description = "dpm_mock_location",
	.handler = restriction_mock_location
};

void TESTCASE_CONSTRUCTOR restriction_policy_build_testcase(void)
{
	testbench_populate_testcase(&restriction_testcase_clipboard);
	testbench_populate_testcase(&restriction_testcase_settings_changes);
	testbench_populate_testcase(&restriction_testcase_background_data);
	testbench_populate_testcase(&restriction_testcase_usb_debugging);
	testbench_populate_testcase(&restriction_testcase_usb_mass_storage);
	testbench_populate_testcase(&restriction_testcase_factory_reset);
	testbench_populate_testcase(&restriction_testcase_bluetooth_tethering);
	testbench_populate_testcase(&restriction_testcase_mock_location);
}

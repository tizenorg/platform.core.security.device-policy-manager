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

static int restriction_set_allow_clipboard(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_clipboard(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_clipboard(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_clipboard(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_clipboard(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_set_allow_settings_changes(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_settings_changes(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_settings_changes(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_settings_changes(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_settings_changes(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_set_allow_background_data(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_background_data(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_background_data(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_background_data(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_background_data(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_set_allow_usb_debugging(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_usb_debugging(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_usb_debugging(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_usb_debugging(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_usb_debugging(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_set_allow_usb_mass_storage(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_usb_mass_storage(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_usb_mass_storage(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_usb_mass_storage(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_usb_mass_storage(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_set_allow_factory_reset(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_factory_reset(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_factory_reset(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_factory_reset(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_factory_reset(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_set_allow_bluetooth_tethering(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_bluetooth_tethering(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_bluetooth_tethering(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_bluetooth_tethering(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_bluetooth_tethering(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_set_allow_mock_location(struct testcase* tc)
{
	dpm_client_h handle;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_mock_location(handle, true) == 0) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

static int restriction_get_allow_mock_location(struct testcase* tc)
{
	dpm_client_h handle;
	bool enable = false;

	handle = dpm_create_client();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_set_allow_mock_location(handle, true) != 0) {
		dpm_destroy_client(handle);
		return TEST_FAILED;
	}

	enable = dpm_get_allow_mock_location(handle);
	if (enable == true) {
		dpm_destroy_client(handle);
		return TEST_SUCCESSED;
	}

	dpm_destroy_client(handle);
	return TEST_FAILED;
}

struct testcase restriction_testcase_set_allow_clipboard = {
	.description = "dpm_set_allow_clipboard",
	.handler = restriction_set_allow_clipboard
};

struct testcase restriction_testcase_get_allow_clipboard = {
	.description = "dpm_get_allow_clipboard",
	.handler = restriction_get_allow_clipboard
};

struct testcase restriction_testcase_set_allow_settings_changes = {
	.description = "dpm_set_allow_settings_changes",
	.handler = restriction_set_allow_settings_changes
};

struct testcase restriction_testcase_get_allow_settings_changes = {
	.description = "dpm_get_allow_settings_changes",
	.handler = restriction_get_allow_settings_changes
};

struct testcase restriction_testcase_set_allow_background_data = {
	.description = "dpm_set_allow_background_data",
	.handler = restriction_set_allow_background_data
};

struct testcase restriction_testcase_get_allow_background_data = {
	.description = "dpm_get_allow_background_data",
	.handler = restriction_get_allow_background_data
};

struct testcase restriction_testcase_set_allow_usb_debugging = {
	.description = "dpm_set_allow_usb_debugging",
	.handler = restriction_set_allow_usb_debugging
};

struct testcase restriction_testcase_get_allow_usb_debugging = {
	.description = "dpm_get_allow_usb_debugging",
	.handler = restriction_get_allow_usb_debugging
};

struct testcase restriction_testcase_set_allow_usb_mass_storage = {
	.description = "dpm_set_allow_usb_mass_storage",
	.handler = restriction_set_allow_usb_mass_storage
};

struct testcase restriction_testcase_get_allow_usb_mass_storage = {
	.description = "dpm_get_allow_usb_mass_storage",
	.handler = restriction_get_allow_usb_mass_storage
};

struct testcase restriction_testcase_set_allow_factory_reset = {
	.description = "dpm_set_allow_factory_reset",
	.handler = restriction_set_allow_factory_reset
};

struct testcase restriction_testcase_get_allow_factory_reset = {
	.description = "dpm_get_allow_factory_reset",
	.handler = restriction_get_allow_factory_reset
};

struct testcase restriction_testcase_set_allow_bluetooth_tethering = {
	.description = "dpm_set_allow_bluetooth_tethering",
	.handler = restriction_set_allow_bluetooth_tethering
};

struct testcase restriction_testcase_get_allow_bluetooth_tethering = {
	.description = "dpm_get_allow_bluetooth_tethering",
	.handler = restriction_get_allow_bluetooth_tethering
};

struct testcase restriction_testcase_set_allow_mock_location = {
	.description = "dpm_set_allow_mock_location",
	.handler = restriction_set_allow_mock_location
};

struct testcase restriction_testcase_get_allow_mock_location = {
	.description = "dpm_get_allow_mock_location",
	.handler = restriction_get_allow_mock_location
};

void TESTCASE_CONSTRUCTOR restriction_policy_build_testcase(void)
{
	testbench_populate_testcase(&restriction_testcase_set_allow_clipboard);
	testbench_populate_testcase(&restriction_testcase_get_allow_clipboard);

	testbench_populate_testcase(&restriction_testcase_set_allow_settings_changes);
	testbench_populate_testcase(&restriction_testcase_get_allow_settings_changes);

	testbench_populate_testcase(&restriction_testcase_set_allow_background_data);
	testbench_populate_testcase(&restriction_testcase_get_allow_background_data);

	testbench_populate_testcase(&restriction_testcase_set_allow_usb_debugging);
	testbench_populate_testcase(&restriction_testcase_get_allow_usb_debugging);

	testbench_populate_testcase(&restriction_testcase_set_allow_usb_mass_storage);
	testbench_populate_testcase(&restriction_testcase_get_allow_usb_mass_storage);

	testbench_populate_testcase(&restriction_testcase_set_allow_factory_reset);
	testbench_populate_testcase(&restriction_testcase_get_allow_factory_reset);

	testbench_populate_testcase(&restriction_testcase_set_allow_bluetooth_tethering);
	testbench_populate_testcase(&restriction_testcase_get_allow_bluetooth_tethering);

	testbench_populate_testcase(&restriction_testcase_set_allow_mock_location);
	testbench_populate_testcase(&restriction_testcase_get_allow_mock_location);
}

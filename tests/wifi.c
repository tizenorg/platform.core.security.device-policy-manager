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

#include <dpm/wifi.h>

#include "testbench.h"

static int wifi_profile_change_restriction(struct testcase* tc)
{
	int allow = false;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_wifi_set_profile_change_restriction(handle, true) != 0) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	if (dpm_wifi_is_profile_change_restricted(handle, &allow) != DPM_ERROR_NONE) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

static int wifi_network_access_restriction(struct testcase* tc)
{
	int allow = false;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_wifi_set_network_access_restriction(handle, true) != 0) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	if (dpm_wifi_is_network_access_restricted(handle, &allow) != DPM_ERROR_NONE) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

static int wifi_blacklist(struct testcase* tc)
{
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_wifi_add_ssid_to_blocklist(handle, "testssid") != 0) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	if (dpm_wifi_remove_ssid_from_blocklist(handle, "testssid") != DPM_ERROR_NONE) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

struct testcase wifi_testcase_profile_change_restriction = {
	.description = "wifi_testcase_profile_change_restriction",
	.handler = wifi_profile_change_restriction
};

struct testcase wifi_testcase_network_access_restriction = {
	.description = "wifi_testcase_network_access_restriction",
	.handler = wifi_network_access_restriction
};

struct testcase wifi_testcase_blacklist = {
	.description = "wifi_testcase_blacklist",
	.handler = wifi_blacklist
};

void TESTCASE_CONSTRUCTOR wifi_policy_build_testcase(void)
{
	testbench_populate_testcase(&wifi_testcase_blacklist);
	testbench_populate_testcase(&wifi_testcase_network_access_restriction);
	testbench_populate_testcase(&wifi_testcase_profile_change_restriction);
}

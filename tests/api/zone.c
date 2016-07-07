/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>

#include <dpm/zone.h>

#include "testbench.h"

#define TEST_ZONE_ID "zone1"
#define TEST_SETUP_WIZARD_PKG_ID "org.tizen.krate-setup-wizard"

#define OWNER_ZONE_ID "owner"

static int zone_create(struct testcase* tc)
{
	int ret = TEST_SUCCESSED;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

	if (dpm_zone_create(handle, TEST_ZONE_ID, TEST_SETUP_WIZARD_PKG_ID) != DPM_ERROR_NONE) {
		ret = TEST_FAILED;
		goto out;
	}

out:
	dpm_manager_destroy(handle);

	return ret;
}

static int zone_get_state(struct testcase* tc)
{
	int ret = TEST_SUCCESSED;
	device_policy_manager_h handle;
	dpm_zone_state_e state;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

	if (dpm_zone_get_state(handle, OWNER_ZONE_ID, &state) != DPM_ERROR_NONE) {
		ret = TEST_FAILED;
		goto out;
	}

	if (state == 0) {
		ret = TEST_FAILED;
	}

out:
	dpm_manager_destroy(handle);

	return ret;
}

static bool get_list_cb(const char* name, void* result)
{
	*((int*)result) = TEST_SUCCESSED;
	return true;
}

static int zone_get_list(struct testcase* tc)
{
	int ret = TEST_SUCCESSED;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

	if (dpm_zone_foreach_name(handle, DPM_ZONE_STATE_ALL, get_list_cb, &ret) != DPM_ERROR_NONE) {
		ret = TEST_FAILED;
		goto out;
	}

out:
	dpm_manager_destroy(handle);

	return ret;
}

struct testcase dpm_zone_testcase_lifecycle = {
	.description = "dpm_zone_lifecycle",
	.handler = zone_create
};

struct testcase dpm_zone_testcase_state = {
	.description = "dpm_zone_state",
	.handler = zone_get_state
};

struct testcase dpm_zone_testcase_list = {
	.description = "dpm_zone_list",
	.handler = zone_get_list
};

void TESTCASE_CONSTRUCTOR zone_policy_build_testcase(void)
{
	testbench_populate_testcase(&dpm_zone_testcase_lifecycle);
	testbench_populate_testcase(&dpm_zone_testcase_state);
	testbench_populate_testcase(&dpm_zone_testcase_list);
}

// Copyright (c) 2015 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <stdio.h>

#include <dpm/security.h>

#include "testbench.h"

static int security_lock_screen(struct testcase* tc)
{
    device_policy_manager_h handle;
    int ret;

    handle = dpm_manager_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_security_lockout_screen(handle) != 0) {
        printf("Failed to lockout screen\n");
        ret = TEST_FAILED;
    }

    dpm_manager_destroy(handle);

    return ret;
}

static int security_wipe_data(struct testcase* tc)
{
	int ret;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	ret = TEST_SUCCESSED;
	if (dpm_security_wipe_data(handle, DPM_SECURITY_WIPE_INTERNAL_STORAGE | DPM_SECURITY_WIPE_EXTERNAL_STORAGE) != 0) {
		ret = TEST_FAILED;
	}

	dpm_manager_destroy(handle);

	return ret;
}

static int security_internal_storage_encryption(struct testcase* tc)
{
	int ret, state;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

    ret = TEST_SUCCESSED;
    if (dpm_security_is_internal_storage_encrypted(handle, &state) != DPM_ERROR_NONE) {
        ret = TEST_FAILED;
    }

	dpm_manager_destroy(handle);

	return ret;
}

static int security_external_storage_encryption(struct testcase* tc)
{
	int ret, state;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

    ret = TEST_SUCCESSED;
    if (dpm_security_is_external_storage_encrypted(handle, &state) != DPM_ERROR_NONE) {
        ret = TEST_FAILED;
    }

	dpm_manager_destroy(handle);

	return ret;
}

struct testcase security_testcase_lock_screen = {
	.description = "security_testcase_lock_screen",
	.handler = security_lock_screen
};

struct testcase security_testcase_wipe_data = {
	.description = "security_testcase_wipe_data",
	.handler = security_wipe_data
};

struct testcase security_testcase_internal_storage_encryption = {
	.description = "security_testcase_internal_storage_encryption",
	.handler = security_internal_storage_encryption
};

struct testcase security_testcase_external_storage_encryption = {
	.description = "security_testcase_external_storage_encryption",
	.handler = security_external_storage_encryption
};

void TESTCASE_CONSTRUCTOR security_policy_build_testcase(void)
{
    testbench_populate_testcase(&security_testcase_lock_screen);
    testbench_populate_testcase(&security_testcase_wipe_data);
    testbench_populate_testcase(&security_testcase_internal_storage_encryption);
    testbench_populate_testcase(&security_testcase_external_storage_encryption);

}

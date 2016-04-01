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
#include <dpm/storage.h>
#include "testbench.h"


static int test_dpm_usb_mass_storage(struct testcase* tc)
{
	dpm_context_h context;
	bool enable = false;

	context = dpm_context_create();
	if (context == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

    dpm_storage_policy_h policy = dpm_context_acquire_storage_policy(context);
    if (policy == NULL) {
        printf("Failed to get storage policy interface\n");
        dpm_context_destroy(context);
        return TEST_FAILED;
    }

	if (dpm_storage_set_external_storage_state(context, true) != 0) {
		dpm_context_destroy(context);
		return TEST_FAILED;
	}

	enable = dpm_storage_get_external_storage_state(context);
	if (enable == true) {
		dpm_context_destroy(context);
		return TEST_SUCCESSED;
	}

	dpm_context_release_storage_policy(context);
	dpm_context_destroy(context);
	return TEST_FAILED;
}

static int test_dpm_wipe(struct testcase* tc)
{
	dpm_context_h handle;

	handle = dpm_context_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	if (dpm_device_wipe_data(handle) != 0) {
		dpm_context_destroy(handle);
		return TEST_FAILED;
	}

	dpm_context_destroy(handle);
	return TEST_FAILED;
}


struct testcase storage_testcase_usb_mass_storage = {
	.description = "dpm_external_storage",
	.handler = test_dpm_usb_mass_storage
};

struct testcase storagetestcase_wipe = {
	.description = "dpm_wipe",
	.handler = test_dpm_wipe
};

void TESTCASE_CONSTRUCTOR storage_policy_build_testcase(void)
{

	testbench_populate_testcase(&storage_testcase_usb_mass_storage);
	//testbench_populate_testcase(&storage_testcase_wipe);
}

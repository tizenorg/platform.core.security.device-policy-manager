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
#include <dpm/datashare.h>
#include "testbench.h"

static int test_dpm_clipboard(struct testcase* tc)
{
	int ret;
	dpm_context_h context;
    dpm_restriction_policy_h policy;
	bool enable = false;

	context = dpm_context_create();
	if (context == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

    policy = dpm_context_acquire_datashare_policy(context);
    if (policy == NULL) {
        printf("Failed to get datashare policy interface\n");
        dpm_context_destroy(context);
        return TEST_FAILED;
    }

	ret = TEST_SUCCESSED;
	if (dpm_restriction_set_clipboard_state(policy, true) != 0) {
		ret = TEST_FAILED;
		goto out;
	}

	enable = dpm_restriction_get_clipboard_state(policy);
	if (enable != true) {
		ret = TEST_FAILED;
		goto out;
	}

out:
    dpm_context_release_datashare_policy(policy);
	dpm_context_destroy(context);
	
	return ret;
}

struct testcase datashare_testcase_clipboard = {
	.description = "dpm_clipboard",
	.handler = test_dpm_clipboard
};

void TESTCASE_CONSTRUCTOR datashare_policy_build_testcase(void)
{
	testbench_populate_testcase(&datashare_testcase_clipboard);
}

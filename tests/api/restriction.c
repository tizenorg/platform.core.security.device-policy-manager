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
	dpm_context_h context;
    dpm_restriction_policy_h policy;
	int ret, enable = false;

	context = dpm_context_create();
	if (context == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

    policy = dpm_context_acquire_restriction_policy(context);
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

	if (dpm_restriction_get_clipboard_state(policy, &enable) < 0) {
		ret = TEST_FAILED;
		goto out;
	}

out:
    dpm_context_release_restriction_policy(context, policy);
	dpm_context_destroy(context);

	return ret;
}

static int test_dpm_usb_debugging(struct testcase* tc)
{
	dpm_context_h context;
    dpm_restriction_policy_h policy;
	int ret, enable = false;

	context = dpm_context_create();
	if (context == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to get developer policy interface\n");
        dpm_context_destroy(context);
        return TEST_FAILED;
    }

	ret = TEST_SUCCESSED;
	if (dpm_restriction_set_usb_debugging_state(policy, true) != 0) {
		ret = TEST_FAILED;
		goto out;
	}

	if (dpm_restriction_get_usb_debugging_state(policy, &enable) < 0) {
		ret = TEST_FAILED;
		goto out;
	}

out:
	dpm_context_release_restriction_policy(context, policy);
	dpm_context_destroy(context);

	return ret;
}

static int test_dpm_settings_changes(struct testcase* tc)
{
	dpm_context_h context;
    dpm_restriction_policy_h policy;
	int ret, enable = false;

	context = dpm_context_create();
	if (context == NULL) {
		printf("Failed to create client context\n");
		return TEST_FAILED;
	}

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to get settings policy interface\n");
        dpm_context_destroy(context);
        return TEST_FAILED;
    }

	ret = TEST_SUCCESSED;
	if (dpm_restriction_set_settings_changes_state(policy, true) != 0) {
		ret = TEST_FAILED;
		goto out;
	}

	if (dpm_restriction_get_settings_changes_state(policy, &enable) < 0) {
		ret = TEST_SUCCESSED;
		goto out;
	}

out:
	dpm_context_release_restriction_policy(context, policy);
	dpm_context_destroy(context);

	return ret;
}

struct testcase settings_testcase_settings_changes = {
	.description = "dpm_settings_changes",
	.handler = test_dpm_settings_changes
};

struct testcase developer_testcase_usb_debugging = {
	.description = "dpm_usb_debugging",
	.handler = test_dpm_usb_debugging
};

struct testcase datashare_testcase_clipboard = {
	.description = "dpm_clipboard",
	.handler = test_dpm_clipboard
};

void TESTCASE_CONSTRUCTOR datashare_policy_build_testcase(void)
{
	testbench_populate_testcase(&datashare_testcase_clipboard);
	testbench_populate_testcase(&developer_testcase_usb_debugging);
	testbench_populate_testcase(&settings_testcase_settings_changes);
}

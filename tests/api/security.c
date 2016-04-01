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
    dpm_context_h context;
    dpm_security_policy_h policy;
    int ret;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return TEST_FAILED;
    }

    policy = dpm_context_acquire_security_policy(context);
    if (policy == NULL) {
        printf("Failed to acquire security policy handle\n");
        dpm_context_destroy(context);
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_security_lockout_screen(policy) != 0) {
        printf("Failed to lockout screen\n");
        ret = TEST_FAILED;
    }

    dpm_context_release_security_policy(context, policy);
    dpm_context_destroy(context);

    return ret;
}

struct testcase security_testcase_lock_screen = {
    .description = "dpm_lock_screen",
    .handler = security_lock_screen
};

void TESTCASE_CONSTRUCTOR security_policy_build_testcase(void)
{
    testbench_populate_testcase(&security_testcase_lock_screen);
}

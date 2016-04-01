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
#include <string.h>

#include <dpm/zone.h>

#include "testbench.h"

#define TEST_ZONE_ID "zone1"
#define TEST_SETUP_WIZARD_PKG_ID "org.tizen.zone-setup-wizard"

static int zone_create(struct testcase* tc)
{
    int ret;
    dpm_context_h context;
    dpm_zone_policy_h policy;
    dpm_zone_state_e state;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return TEST_FAILED;
    }

    policy = dpm_context_acquire_zone_policy(context);
    if (policy == NULL) {
        printf("Failed to get zone policy");
        dpm_context_destroy(context);
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_zone_create(policy, TEST_ZONE_ID, TEST_SETUP_WIZARD_PKG_ID) != DPM_ERROR_NONE) {
        ret = TEST_FAILED;
        goto out;
    }

    if (dpm_zone_get_state(context, TEST_ZONE_ID, &state) != DPM_ERROR_NONE) {
        ret = TEST_FAILED;
        goto remove;
    }

remove:
    if (dpm_zone_destroy(policy, TEST_ZONE_ID) == DPM_ERROR_NONE) {
        ret = TEST_FAILED;
        goto out;
    }

out:
    dpm_context_release_zone_policy(policy);
    dpm_context_destroy(context);

    return ret;
}

struct testcase zone_testcase_lifecycle = {
    .description = "dpm_zone",
    .handler = zone_create
};

void TESTCASE_CONSTRUCTOR zone_policy_build_testcase(void)
{
    testbench_populate_testcase(&zone_testcase_lifecycle);
}

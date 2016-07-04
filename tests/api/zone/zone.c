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

#include <zone/zone.h>

#include "../testbench.h"

#define TEST_ZONE_ID "zone1"
#define TEST_ZONE_METADATA  "<?xml version=\"1.0\" encoding=\"utf-8\"?> \
                            <manifest version=\"0.1.0\"> \
                            <capability></capability> \
                            <filesystem></filesystem> \
                            <network></network> \
                            </manifest>"

#define OWNER_ZONE_ID "owner"

static int zone_create_remove(struct testcase* tc)
{
    int ret = TEST_SUCCESSED;
    int result;
    zone_manager_h handle;

    result = zone_manager_create(&handle);
    if (result != ZONE_ERROR_NONE) {
        printf("Failed to create client context\n");
        return TEST_FAILED;
    }

    if (zone_manager_create_zone(handle, TEST_ZONE_ID, TEST_ZONE_METADATA) != ZONE_ERROR_NONE) {
        ret = TEST_FAILED;
        goto out;
    }

out:
    zone_manager_destroy(handle);

    return ret;
}

static int zone_get_state(struct testcase* tc)
{
    int ret = TEST_SUCCESSED;
    int result;
    zone_manager_h handle;
    zone_state_e state;

    result = zone_manager_create(&handle);
    if (result != ZONE_ERROR_NONE) {
        printf("Failed to create client context\n");
        return TEST_FAILED;
    }

    if (zone_manager_get_zone_state(handle, OWNER_ZONE_ID, &state) != ZONE_ERROR_NONE) {
        ret = TEST_FAILED;
        goto out;
    }

    if (state == 0) {
        ret = TEST_FAILED;
    }

out:
    zone_manager_destroy(handle);

    return ret;
}

bool get_list_cb(const char* name, void* result)
{
    *((int*)result) = TEST_SUCCESSED;
    return true;
}

static int zone_get_list(struct testcase* tc)
{
    int ret = TEST_SUCCESSED;
    int result;
    zone_manager_h handle;

    result = zone_manager_create(&handle);
    if (result != ZONE_ERROR_NONE) {
        printf("Failed to create client context\n");
        return TEST_FAILED;
    }

    if (zone_manager_foreach_name(handle, ZONE_STATE_ALL, get_list_cb, &ret) != ZONE_ERROR_NONE) {
        ret = TEST_FAILED;
        goto out;
    }

out:
    zone_manager_destroy(handle);

    return ret;
}

struct testcase zone_testcase_lifecycle = {
    .description = "zone_lifecycle",
    .handler = zone_create_remove
};

struct testcase zone_testcase_state = {
    .description = "zone_state",
    .handler = zone_get_state
};

struct testcase zone_testcase_list = {
    .description = "zone_list",
    .handler = zone_get_list
};

struct testcase zone_testcase_list = {
    .description = "zone_password",
    .handler = zone_get_list
};

void TESTCASE_CONSTRUCTOR zone_manager_build_testcase(void)
{
    testbench_populate_testcase(&zone_testcase_lifecycle);
    testbench_populate_testcase(&zone_testcase_state);
    testbench_populate_testcase(&zone_testcase_list);
}

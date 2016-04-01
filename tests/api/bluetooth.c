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
#include <dpm/bluetooth.h>
#include "testbench.h"

static int bluetooth_add_device_to_blacklist(struct testcase* tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    const char* dummy_mac_address = "e8:11:32:30:78:9c";
    if (dpm_add_bluetooth_device_to_blacklist(handle, dummy_mac_address) != 0) {
        dpm_destroy_context(handle);
        return TEST_FAILED;
    }

    dpm_destroy_context(handle);
    return TEST_SUCCESSED;
}

static int bluetooth_remove_device_from_blacklist(struct testcase* tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    const char* dummy_mac_address = "e8:11:32:30:78:9c";
    if (dpm_remove_bluetooth_device_from_blacklist(handle, dummy_mac_address) != 0) {
        dpm_destroy_context(handle);
        return TEST_FAILED;
    }

    dpm_destroy_context(handle);
    return TEST_SUCCESSED;
}

static int bluetooth_device_restriction(struct testcase* tc)
{
    bool allow = false;
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_bluetooth_device_restriction(handle, true) != 0) {
        return TEST_FAILED;
    }

    // the 'allow' will be always 'true' before server/bluetooth.cpp has been implemented.
    allow = dpm_is_bluetooth_device_restricted(handle);
    if (allow == true) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int bluetooth_add_uuid_to_blacklist(struct testcase* tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    const char* dummy_uuid = "ff8ca1f3-0221-40c9-91fd-25ebbbfa68c3";
    if (dpm_add_bluetooth_uuid_to_blacklist(handle, dummy_uuid) != 0) {
        dpm_destroy_context(handle);
        return TEST_FAILED;
    }

    dpm_destroy_context(handle);
    return TEST_SUCCESSED;
}

static int bluetooth_remove_uuid_from_blacklist(struct testcase* tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    const char* dummy_uuid = "ff8ca1f3-0221-40c9-91fd-25ebbbfa68c3";
    if (dpm_remove_bluetooth_uuid_from_blacklist(handle, dummy_uuid) != 0) {
        dpm_destroy_context(handle);
        return TEST_FAILED;
    }

    dpm_destroy_context(handle);
    return TEST_SUCCESSED;
}

static int bluetooth_uuid_restriction(struct testcase* tc)
{
    bool allow = false;
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_bluetooth_uuid_restriction(handle, true) != 0) {
        return TEST_FAILED;
    }

    // the 'allow' will be always 'true' before server/bluetooth.cpp has been implemented.
    allow = dpm_is_bluetooth_uuid_restricted(handle);
    if (allow == true) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

struct testcase bluetooth_testcase_add_device_to_blacklist = {
    .description = "dpm_add_device_to_blacklist",
    .handler = bluetooth_add_device_to_blacklist
};

struct testcase bluetooth_testcase_remove_device_from_blacklist = {
    .description = "dpm_remove_device_from_blacklist",
    .handler = bluetooth_remove_device_from_blacklist
};

struct testcase bluetooth_testcase_device_restriction = {
    .description = "dpm_device_restriction",
    .handler = bluetooth_device_restriction
};

struct testcase bluetooth_testcase_add_uuid_to_blacklist = {
    .description = "dpm_add_uuid_to_blacklist",
    .handler = bluetooth_add_uuid_to_blacklist
};

struct testcase bluetooth_testcase_remove_uuid_from_blacklist = {
    .description = "dpm_remove_uuid_from_blacklist",
    .handler = bluetooth_remove_uuid_from_blacklist
};

struct testcase bluetooth_testcase_uuid_restriction = {
    .description = "dpm_uuid_restriction",
    .handler = bluetooth_uuid_restriction
};

void TESTCASE_CONSTRUCTOR bluetooth_policy_build_testcase(void)
{
    testbench_populate_testcase(&bluetooth_testcase_add_device_to_blacklist);
    testbench_populate_testcase(&bluetooth_testcase_remove_device_from_blacklist);
    testbench_populate_testcase(&bluetooth_testcase_device_restriction);
    testbench_populate_testcase(&bluetooth_testcase_add_uuid_to_blacklist);
    testbench_populate_testcase(&bluetooth_testcase_remove_uuid_from_blacklist);
    testbench_populate_testcase(&bluetooth_testcase_uuid_restriction);
}

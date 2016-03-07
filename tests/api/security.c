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
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_lockout_screen(handle) != 0) {
        ret = TEST_FAILED;
    }

    return ret;
}

static int security_wipe_internal_memory(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_wipe_data(handle, WIPE_INTERNAL_MEMORY) != 0) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

static int security_wipe_external_memory(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_wipe_data(handle, WIPE_EXTERNAL_MEMORY) != 0) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

static int security_reboot(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_reboot(handle) != 0) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

static int security_poweroff_device(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_poweroff_device(handle) != 0) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

static int security_set_internal_storage_encryption(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_set_internal_storage_encryption(handle, 1) != 0) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

static int security_is_internal_storage_encrypted(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_is_internal_storage_encrypted(handle) != TRUE) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

static int security_set_external_storage_encryption(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_set_external_storage_encryption(handle, 1) != 0) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

static int security_is_external_storage_encrypted(struct testcase* tc)
{
    int ret;
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    ret = TEST_SUCCESSED;
    if (dpm_is_external_storage_encrypted(handle) != TRUE) {
        ret = TEST_FAILED;
    }

    dpm_destroy_client(handle);
    return ret;
}

struct testcase security_testcase_lock_screen = {
    .description = "dpm_lock_screen",
    .handler = security_lock_screen
};

struct testcase security_testcase_wipe_internal_memory = {
    .description = "dpm_wipe_data(internal memory)",
    .handler = security_wipe_internal_memory
};

struct testcase security_testcase_wipe_external_memory = {
    .description = "dpm_wipe_data(external memory)",
    .handler = security_wipe_external_memory
};

struct testcase security_testcase_reboot = {
    .description = "dpm_reboot",
    .handler = security_reboot
};

struct testcase security_testcase_poweroff = {
    .description = "dpm_poweroff_device",
    .handler = security_poweroff_device
};

struct testcase security_testcase_set_internal_storage_encryption = {
    .description = "dpm_set_internal_storage_encryption",
    .handler = security_set_internal_storage_encryption
};

struct testcase security_testcase_is_internal_storage_encrypted = {
    .description = "dpm_is_internal_storage_encrypted",
    .handler = security_is_internal_storage_encrypted
};

struct testcase security_testcase_set_external_storage_encryption = {
    .description = "dpm_set_external_storage_encryption",
    .handler = security_set_external_storage_encryption
};

struct testcase security_testcase_is_external_storage_encrypted = {
    .description = "dpm_is_external_storage_encrypted",
    .handler = security_is_external_storage_encrypted
};

void TESTCASE_CONSTRUCTOR security_policy_build_testcase(void)
{
    testbench_populate_testcase(&security_testcase_lock_screen);
    testbench_populate_testcase(&security_testcase_wipe_internal_memory);
    testbench_populate_testcase(&security_testcase_wipe_external_memory);
    //testbench_populate_testcase(&security_testcase_reboot);
    //testbench_populate_testcase(&security_testcase_poweroff);
    testbench_populate_testcase(&security_testcase_set_internal_storage_encryption);
    testbench_populate_testcase(&security_testcase_is_internal_storage_encrypted);
    testbench_populate_testcase(&security_testcase_set_external_storage_encryption);
    testbench_populate_testcase(&security_testcase_is_external_storage_encrypted);
}


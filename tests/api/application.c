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

#include <dpm/application.h>

#include "testbench.h"

static int application_mode_restriction(struct testcase* tc)
{
    int allow = false;
    device_policy_manager_h handle;

    handle = dpm_manager_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_application_set_mode_restriction(handle, DPM_PACKAGE_RESTRICTION_MODE_ALL) != 0) {
        dpm_manager_destroy(handle);
        return TEST_FAILED;
    }

    if (dpm_application_get_mode_restriction(handle, &allow) != DPM_ERROR_NONE) {
        dpm_manager_destroy(handle);
        return TEST_FAILED;
    }

    if (dpm_application_unset_mode_restriction(handle, DPM_PACKAGE_RESTRICTION_MODE_ALL) != DPM_ERROR_NONE) {
        dpm_manager_destroy(handle);
        return TEST_FAILED;
    }

    dpm_manager_destroy(handle);
    return TEST_SUCCESSED;
}

static int application_privilege_restriction(struct testcase* tc)
{
    int allow = false;
    device_policy_manager_h handle;

    handle = dpm_manager_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_application_add_privilege_to_blacklist(handle, 0, "http://tizen.org/privilege/internet") != 0) {
        dpm_manager_destroy(handle);
        return TEST_FAILED;
    }

    if (dpm_application_check_privilege_is_blacklisted(handle, 0, "http://tizen.org/privilege/internet", &allow) != DPM_ERROR_NONE) {
        dpm_manager_destroy(handle);
        return TEST_FAILED;
    }

    if (dpm_application_remove_privilege_from_blacklist(handle, 0, "http://tizen.org/privilege/internet") != DPM_ERROR_NONE) {
        dpm_manager_destroy(handle);
        return TEST_FAILED;
    }

    dpm_manager_destroy(handle);
    return TEST_SUCCESSED;
}

struct testcase application_testcase_privilege_restriction = {
    .description = "application_testcase_privilege_restriction",
    .handler = application_privilege_restriction
};

struct testcase application_testcase_mode_restriction = {
    .description = "application_testcase_mode_restriction",
    .handler = application_mode_restriction
};

void TESTCASE_CONSTRUCTOR application_policy_build_testcase(void)
{
    testbench_populate_testcase(&application_testcase_mode_restriction);
    testbench_populate_testcase(&application_testcase_privilege_restriction);
}

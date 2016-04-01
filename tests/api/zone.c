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
    dpm_context_h handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_create_zone(handle, TEST_ZONE_ID, TEST_SETUP_WIZARD_PKG_ID) == 0) {
        dpm_context_destroy(handle);
        return TEST_SUCCESSED;
    }

    dpm_context_destroy(handle);
    return TEST_FAILED;
}

static int zone_remove(struct testcase* tc)
{
    dpm_context_h handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_remove_zone(handle, TEST_ZONE_ID) == 0) {
        dpm_context_destroy(handle);
        return TEST_SUCCESSED;
    }

    dpm_context_destroy(handle);
    return TEST_FAILED;
}

static int zone_get_state(struct testcase* tc)
{
    dpm_context_h handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_get_zone_state(handle, TEST_ZONE_ID) != DPM_ERROR_INVALID_PARAMETER) {
        dpm_context_destroy(handle);
        return TEST_SUCCESSED;
    }

    dpm_context_destroy(handle);
    return TEST_FAILED;
}

static int zone_get_list(struct testcase* tc)
{
    dpm_context_h handle;
    dpm_zone_iterator_h it;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    it = dpm_get_zone_iterator(handle);

    dpm_context_destroy(handle);

    if (it != NULL) {
        dpm_free_zone_iterator(it);
        return TEST_SUCCESSED;
    }

    return TEST_FAILED;
}

static int zone_traverse_list(struct testcase* tc)
{
    dpm_context_h handle;
    dpm_zone_iterator_h it;
    const char* zone;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    it = dpm_get_zone_iterator(handle);

    dpm_context_destroy(handle);

    if (it != NULL) {
        printf("Traversing zone list started\n");
        while ((zone = dpm_zone_iterator_next(it)) != NULL) {
            printf("%s\n", zone);
            if (!strcmp(zone, TEST_ZONE_ID)) {
                printf("%s is found!\n", TEST_ZONE_ID);
                break;
            }
        }
        printf("Traversing zone list was completed\n");

        dpm_free_zone_iterator(it);

        if (zone != NULL) {
            return TEST_SUCCESSED;
        } else {
            return TEST_FAILED;
        }
    }

    return TEST_FAILED;
}

static int zone_signal(struct testcase* tc)
{
    dpm_context_h handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    dpm_context_destroy(handle);

    return TEST_FAILED;
}

struct testcase zone_testcase_create = {
    .description = "dpm_zone_create",
    .handler = zone_create
};

struct testcase zone_testcase_remove = {
    .description = "dpm_zone_remove",
    .handler = zone_remove
};

struct testcase zone_testcase_get_state = {
    .description = "dpm_get_zone_state",
    .handler = zone_get_state
};

struct testcase zone_testcase_get_list = {
    .description = "dpm_get_zone_iterator, dpm_free_zone_iterator",
    .handler = zone_get_list
};

struct testcase zone_testcase_traverse_list = {
    .description = "dpm_zone_iterator_next",
    .handler = zone_traverse_list
};

struct testcase zone_testcase_signal = {
    .description = "dpm_subscribe_zone_signal, dpm_unsubscribe_zone_signal",
    .handler = zone_signal
};


void TESTCASE_CONSTRUCTOR zone_policy_build_testcase(void)
{
    testbench_populate_testcase(&zone_testcase_signal);
    testbench_populate_testcase(&zone_testcase_remove);
    testbench_populate_testcase(&zone_testcase_traverse_list);
    testbench_populate_testcase(&zone_testcase_get_list);
    testbench_populate_testcase(&zone_testcase_get_state);
    testbench_populate_testcase(&zone_testcase_create);
}


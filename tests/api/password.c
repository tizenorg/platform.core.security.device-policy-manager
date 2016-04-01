/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dpm/password.h>

#include "testbench.h"

static int password_set_password_quality(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_quality(handle, "owner", DPM_PASSWORD_QUALITY_ALPHANUMERIC) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_password_minimum_length(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_minimum_length(handle, "owner", 27) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_min_password_complex_chars(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_min_password_complex_chars(handle, "owner", 10) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_maximum_failed_password_for_wipe(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_maximum_failed_password_for_wipe(handle, "owner", 11) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_password_expires(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_expires(handle, "owner", 30) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_password_history(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_history(handle, "owner", 12) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_password_pattern(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_pattern(handle, "owner", "[a-zA-Z]{4}[0-9]{4}") == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_reset_password(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_reset_password(handle, "owner", "tizen") == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_enforce_password_change(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_enforce_password_change(handle, "owner") == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_max_inactivity_time_device_lock(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_max_inactivity_time_device_lock(handle, "owner", 30) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_get_max_inactivity_time_device_lock(struct testcase *tc)
{
    dpm_context_h handle;
    int max_inactivity_time = 0;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_get_max_inactivity_time_device_lock(handle, "owner", &max_inactivity_time) == 0) {
        dpm_destroy_context(handle);
        printf("[DPM] maxInactivityTime: %d\n", max_inactivity_time);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_password_status(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_status(handle, "owner", 10) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_delete_password_pattern(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_delete_password_pattern(handle, "owner") == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_get_password_pattern(struct testcase *tc)
{
    dpm_context_h handle;
    char *passwd_pattern;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    passwd_pattern = dpm_get_password_pattern(handle, "owner");
    if (passwd_pattern != NULL) {
        printf("[DPM] Password Pattern: %s\n", passwd_pattern);

		free(passwd_pattern);

        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_maximum_character_occurrences(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_maximum_character_occurrences(handle, "owner", 15) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_get_maximum_character_occurrences(struct testcase *tc)
{
    dpm_context_h handle;
    int maximum_char_occurences = 0;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_get_maximum_character_occurrences(handle, "owner", &maximum_char_occurences) == 0) {
        dpm_destroy_context(handle);
        printf("[DPM] maxCharOccurences: %d\n", maximum_char_occurences);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_set_maximum_numeric_sequence_length(struct testcase *tc)
{
    dpm_context_h handle;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_maximum_numeric_sequence_length(handle, "owner", 7) == 0) {
        dpm_destroy_context(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

static int password_get_maximum_numeric_sequence_length(struct testcase *tc)
{
    dpm_context_h handle;
    int maximum_num_seq_length = 0;

    handle = dpm_create_context();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_get_maximum_numeric_sequence_length(handle, "owner", &maximum_num_seq_length) == 0) {
        dpm_destroy_context(handle);
        printf("[DPM] maxNumSeqLength: %d\n", maximum_num_seq_length);
        return TEST_SUCCESSED;
    }

    dpm_destroy_context(handle);
    return TEST_FAILED;
}

struct testcase security_testcase_set_password_quality = {
    .description = "dpm_set_password_quality",
    .handler = password_set_password_quality
};

struct testcase security_testcase_set_password_minimum_length = {
    .description = "dpm_set_password_minimum_length",
    .handler = password_set_password_minimum_length
};

struct testcase security_testcase_set_min_password_complex_chars = {
    .description = "dpm_set_min_password_complex_chars",
    .handler = password_set_min_password_complex_chars
};

struct testcase security_testcase_set_maximum_failed_password_for_wipe = {
    .description = "dpm_set_maximum_failed_password_for_wipe",
    .handler = password_set_maximum_failed_password_for_wipe
};

struct testcase security_testcase_set_password_expires = {
    .description = "dpm_set_password_expires",
    .handler = password_set_password_expires
};

struct testcase security_testcase_set_password_history = {
    .description = "dpm_set_password_history",
    .handler = password_set_password_history
};

struct testcase security_testcase_set_password_pattern = {
    .description = "dpm_set_password_pattern",
    .handler = password_set_password_pattern
};

struct testcase security_testcase_reset_password = {
    .description = "dpm_reset_password",
    .handler = password_reset_password
};

struct testcase security_testcase_enforce_password_change = {
    .description = "dpm_enforce_password_change",
    .handler = password_enforce_password_change
};

struct testcase security_testcase_set_max_inactivity_time_device_lock = {
    .description = "dpm_set_max_inactivity_time_device_lock",
    .handler = password_set_max_inactivity_time_device_lock
};

struct testcase security_testcase_get_max_inactivity_time_device_lock = {
    .description = "dpm_get_max_inactivity_time_device_lock",
    .handler = password_get_max_inactivity_time_device_lock
};

struct testcase security_testcase_set_password_status = {
    .description = "dpm_set_password_status",
    .handler = password_set_password_status
};

struct testcase security_testcase_delete_password_pattern = {
    .description = "dpm_delete_password_pattern",
    .handler = password_delete_password_pattern
};

struct testcase security_testcase_get_password_pattern = {
    .description = "dpm_get_password_pattern",
    .handler = password_get_password_pattern
};

struct testcase security_testcase_set_maximum_character_occurrences = {
    .description = "dpm_set_maximum_character_occurrences",
    .handler = password_set_maximum_character_occurrences
};

struct testcase security_testcase_get_maximum_character_occurrences = {
    .description = "dpm_get_maximum_character_occurrences",
    .handler = password_get_maximum_character_occurrences
};

struct testcase security_testcase_set_maximum_numeric_sequence_length = {
    .description = "dpm_set_maximum_numeric_sequence_length",
    .handler = password_set_maximum_numeric_sequence_length
};

struct testcase security_testcase_get_maximum_numeric_sequence_length = {
    .description = "dpm_get_maximum_numeric_sequence_length",
    .handler = password_get_maximum_numeric_sequence_length
};

void TESTCASE_CONSTRUCTOR password_policy_build_testcase(void)
{
    testbench_populate_testcase(&security_testcase_set_password_quality);
    testbench_populate_testcase(&security_testcase_set_password_minimum_length);
    testbench_populate_testcase(&security_testcase_set_min_password_complex_chars);
    testbench_populate_testcase(&security_testcase_set_maximum_failed_password_for_wipe);
    testbench_populate_testcase(&security_testcase_set_password_expires);
    testbench_populate_testcase(&security_testcase_set_password_history);
    testbench_populate_testcase(&security_testcase_enforce_password_change);
    testbench_populate_testcase(&security_testcase_get_max_inactivity_time_device_lock);
    testbench_populate_testcase(&security_testcase_set_max_inactivity_time_device_lock);
    testbench_populate_testcase(&security_testcase_set_password_status);
    testbench_populate_testcase(&security_testcase_delete_password_pattern);
    testbench_populate_testcase(&security_testcase_get_password_pattern);
    testbench_populate_testcase(&security_testcase_set_password_pattern);
    testbench_populate_testcase(&security_testcase_get_maximum_character_occurrences);
    testbench_populate_testcase(&security_testcase_set_maximum_character_occurrences);
    testbench_populate_testcase(&security_testcase_get_maximum_numeric_sequence_length);
    testbench_populate_testcase(&security_testcase_set_maximum_numeric_sequence_length);
    testbench_populate_testcase(&security_testcase_reset_password);
}


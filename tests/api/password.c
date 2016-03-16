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

#include <dpm/password.h>

#include "testbench.h"

static int password_set_password_quality(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_quality(handle, "owner", DPM_PASSWORD_QUALITY_ALPHANUMERIC) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_password_minimum_length(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_minimum_length(handle, "owner", 27) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_min_password_complex_chars(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_min_password_complex_chars(handle, "owner", 10) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_maximum_failed_password_for_wipe(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_maximum_failed_password_for_wipe(handle, "owner", 11) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_password_expires(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_expires(handle, "owner", 30) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_password_history(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_history(handle, "owner", 12) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_password_pattern(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_pattern(handle, "owner", "[a-zA-Z]{4}[0-9]{4}") == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_reset_password(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_reset_password(handle, "owner", "tizen") == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_enforce_password_change(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_enforce_password_change(handle, "owner") == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_max_inactivity_time_device_lock(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_max_inactivity_time_device_lock(handle, "owner", 30) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_get_max_inactivity_time_device_lock(struct testcase* tc)
{
    dpm_client_h handle;
	int maxInactivityTime = 0;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_get_max_inactivity_time_device_lock(handle, "owner", &maxInactivityTime) == 0) {
        dpm_destroy_client(handle);
		printf("[DPM] maxInactivityTime: %d\n",maxInactivityTime);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_get_password_policy(struct testcase* tc)
{
    dpm_client_h handle;
	dpm_data_type* dpm_data;
	dpm_password_policy_type* passwdPolicy;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    dpm_data = dpm_get_password_policy(handle, "owner");
    if (dpm_data != NULL) {
        passwdPolicy = (dpm_password_policy_type*)dpm_data->data;
		printf("[DPM] minLength: %d, maxAttempts: %d, historySize: %d, Pattern: %s\n", passwdPolicy->minLength,
			passwdPolicy->maxAttempts, passwdPolicy->historySize, passwdPolicy->pattern);
		dpm_data->mem_header.free_func(dpm_data);
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_password_status(struct testcase* tc)
{
    dpm_client_h handle;
	
    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_password_status(handle, "owner", 10) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_delete_password_pattern(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_delete_password_pattern(handle, "owner") == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_get_password_pattern(struct testcase* tc)
{
    dpm_client_h handle;
	dpm_data_type* dpm_data;
	char* passwdPattern;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    dpm_data = dpm_get_password_pattern(handle, "owner");
    if (dpm_data != NULL) {
		passwdPattern = (char*)dpm_data->data;
		printf("[DPM] Password Pattern: %s\n", passwdPattern);
		dpm_data->mem_header.free_func(dpm_data);
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_maximum_character_occurrences(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_maximum_character_occurrences(handle, "owner", 15) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_get_maximum_character_occurrences(struct testcase* tc)
{
    dpm_client_h handle;
	int maximumCharOccurences = 0;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_get_maximum_character_occurrences(handle, "owner", &maximumCharOccurences) == 0) {
        dpm_destroy_client(handle);
		printf("[DPM] maxCharOccurences: %d\n",maximumCharOccurences);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_maximum_numeric_sequence_length(struct testcase* tc)
{
    dpm_client_h handle;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_set_maximum_numeric_sequence_length(handle, "owner", 7) == 0) {
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_get_maximum_numeric_sequence_length(struct testcase* tc)
{
    dpm_client_h handle;
	int maximumNumSeqLength = 0;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    if (dpm_get_maximum_numeric_sequence_length(handle, "owner", &maximumNumSeqLength) == 0) {
        dpm_destroy_client(handle);
		printf("[DPM] maxNumSeqLength: %d\n",maximumNumSeqLength);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
    return TEST_FAILED;
}

static int password_set_forbidden_strings(struct testcase* tc)
{
    dpm_client_h handle;
	GList *p_string_list = NULL;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    p_string_list = g_list_append(p_string_list,g_strdup("forbidden"));
	p_string_list = g_list_append(p_string_list,g_strdup("strings"));

	if (dpm_set_forbidden_strings(handle, "owner", p_string_list) == 0) {
        dpm_destroy_client(handle);

		g_list_foreach((GList*)p_string_list,(GFunc)g_free,NULL);
		g_list_free((GList*)p_string_list);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
	g_list_foreach((GList*)p_string_list,(GFunc)g_free,NULL);
	g_list_free((GList*)p_string_list);
    return TEST_FAILED;
}

static int password_get_forbidden_strings(struct testcase* tc)
{
    dpm_client_h handle;
	dpm_data_type* dpm_data;
	GList *p_string_list = NULL;
	int i = 0;

    handle = dpm_create_client();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return TEST_FAILED;
    }

    dpm_data = dpm_get_forbidden_strings(handle, "owner");

	if (dpm_data != NULL) {
		p_string_list = (GList*)dpm_data->data;
		while(p_string_list != NULL)
		{
		    printf("[DPM]Forbidden Strings[%d]: %s\n",i++,(const char*)(p_string_list->data));
			p_string_list = p_string_list->next;
		}
		dpm_data->mem_header.free_func(dpm_data);
        dpm_destroy_client(handle);
        return TEST_SUCCESSED;
    }

    dpm_destroy_client(handle);
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

struct testcase security_testcase_get_password_policy = {
    .description = "dpm_get_password_policy",
    .handler = password_get_password_policy
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

struct testcase security_testcase_set_forbidden_strings = {
    .description = "dpm_set_forbidden_strings",
    .handler = password_set_forbidden_strings
};

struct testcase security_testcase_get_forbidden_strings = {
    .description = "dpm_get_forbidden_strings",
    .handler = password_get_forbidden_strings
};

void TESTCASE_CONSTRUCTOR password_policy_build_testcase(void)
{	
	testbench_populate_testcase(&security_testcase_get_password_policy);
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
	testbench_populate_testcase(&security_testcase_get_forbidden_strings);
	testbench_populate_testcase(&security_testcase_set_forbidden_strings);	
	testbench_populate_testcase(&security_testcase_reset_password);
}


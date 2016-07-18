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
#include <stdlib.h>
#include <unistd.h>	// sysconf
#include <pwd.h>	// struct pwd
#include <dpm/administration.h>

#include "testbench.h"

static int getUID(const char* userName)
{
	struct passwd pwd, *result;
	int bufsize;

	if (userName == NULL)
		return -1;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1)
		bufsize = 16384;

	char *buffer = (char*)malloc(bufsize);
	if (!buffer)
		return -1;

	getpwnam_r(userName, &pwd, buffer, bufsize, &result);
	if (result == NULL) {
		printf("User %s doesn't exist\n", userName);
		return -1;
	}

	if (buffer)
		free(buffer);

	return (int)result->pw_uid;
}

static int admin_register_client(struct testcase* tc)
{
	int ret;
	int uid;
	device_policy_manager_h handle;

	uid = getUID("owner");
	if (uid == -1)
		return TEST_FAILED;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	ret = TEST_SUCCESSED;
	if (dpm_admin_register_client(handle, "org.tizen.dpm-toolkit", uid) != DPM_ERROR_NONE) {
		ret = TEST_FAILED;
	}

	dpm_manager_destroy(handle);

	return ret;
}

static int admin_deregister_client(struct testcase* tc)
{
	int ret;
	int uid;
	device_policy_manager_h handle;

	uid = getUID("owner");
	if (uid == -1)
		return TEST_FAILED;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	ret = TEST_SUCCESSED;
	if (dpm_admin_deregister_client(handle, "org.tizen.dpm-toolkit", uid) != DPM_ERROR_NONE) {
		ret = TEST_FAILED;
	}

	dpm_manager_destroy(handle);

	return ret;
}

struct testcase admin_testcase_register_client = {
	.description = "admin_testcase_register_client",
	.handler = admin_register_client
};

struct testcase admin_testcase_deregister_client = {
	.description = "admin_testcase_deregister_client",
	.handler = admin_deregister_client
};

void TESTCASE_CONSTRUCTOR admin_policy_build_testcase(void)
{
	testbench_populate_testcase(&admin_testcase_deregister_client);
	testbench_populate_testcase(&admin_testcase_register_client);
}

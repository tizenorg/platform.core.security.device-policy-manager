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
#include <unistd.h>
#include <pthread.h>

#include <dpm/restriction.h>

#include "testbench.h"

#define MAX_WORKER_THREADS  5
#define MAX_ITERATIONS      10

static volatile int completed = 0;

static void signal_callback(const char* name, const char* object, void* user_data)
{
}

static int policy_manager_signal(struct testcase* tc)
{
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return TEST_FAILED;
	}

	int id;
	if (dpm_add_signal_cb(handle, "unknown", signal_callback, NULL, &id) == DPM_ERROR_NONE) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	if (dpm_add_signal_cb(handle, "camera", signal_callback, NULL, &id) != DPM_ERROR_NONE) {
		dpm_manager_destroy(handle);
		return TEST_FAILED;
	}

	dpm_remove_signal_cb(handle, id);
	dpm_manager_destroy(handle);
	return TEST_SUCCESSED;
}

static void device_policy_handle_callback(const char* name, const char* state, void* user_data)
{
	int *triggered = user_data;
	printf("*");
	*triggered = 1;
}

static void* getter(void* data)
{
	int i = 0;
	device_policy_manager_h handle;
	volatile int triggered = 0;;

	while (1) {
		handle = dpm_manager_create();
		if (handle == NULL) {
			printf("Failed to create client handle\n");
			return (void *)TEST_FAILED;
		}

		int id;
		dpm_add_policy_changed_cb(handle, "camera", device_policy_handle_callback, (void *)&triggered, &id);

		while (!triggered) {
			if (completed) {
				dpm_remove_policy_changed_cb(handle, id);
				dpm_manager_destroy(handle);
				return (void *)TEST_SUCCESSED;
			}
		}

		triggered = 0;

		if ((i % 10) == 0) {
			printf("\n");
		}

		dpm_remove_policy_changed_cb(handle, id);
		dpm_manager_destroy(handle);

		printf("G");

		i++;
	}

	return (void *)TEST_SUCCESSED;
}

static void* setter(void *data)
{
	int i;
	device_policy_manager_h handle;

	for (i = 0; i < MAX_ITERATIONS; i++) {
		handle = dpm_manager_create();
		if (handle == NULL) {
			printf("Failed to create client handle\n");
			completed = 1;
			return (void *)TEST_FAILED;
		}

		int state = 0;

		dpm_restriction_get_camera_state(handle, &state);
		dpm_restriction_set_camera_state(handle, state ? 0 : 1);

		if ((i % 10) == 0) {
			printf("\n");
		}

		dpm_manager_destroy(handle);

	}

	printf("\n");

	completed = 1;

	return (void *)TEST_SUCCESSED;
}

static int policy_manager_handle(struct testcase* tc)
{
	pthread_t handle[MAX_WORKER_THREADS];
	int i, ret, status, idx[MAX_WORKER_THREADS];

	for (i = 0; i < MAX_WORKER_THREADS; i++) {
		idx[i] = i;

		if (i == 0) {
			pthread_create(&handle[i], NULL, setter, (void *)&idx[i]);
		} else {
			pthread_create(&handle[i], NULL, getter, (void *)&idx[i]);
		}
	}

	ret = TEST_SUCCESSED;
	for (i = 0; i < MAX_WORKER_THREADS; i++) {
		pthread_join(handle[i], (void *)&status);
		if (status == TEST_FAILED) {
			ret = TEST_FAILED;
		}
	}

	return ret;
}

struct testcase policy_manager_handle_testcase = {
	.description = "policy manager handle",
	.handler = policy_manager_handle
};

struct testcase policy_manager_signal_testcase = {
	.description = "policy manager signal",
	.handler = policy_manager_signal
};

void TESTCASE_CONSTRUCTOR device_context_handle_build_testcase(void)
{
	testbench_populate_testcase(&policy_manager_handle_testcase);
	testbench_populate_testcase(&policy_manager_signal_testcase);
}

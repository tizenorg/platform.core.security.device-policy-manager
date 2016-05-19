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
#include <unistd.h>
#include <pthread.h>

#include <dpm/restriction.h>

#include "testbench.h"

#define MAX_WORKER_THREADS  8
#define MAX_ITERATIONS      1000

volatile int completed = 0;

void device_policy_context_callback(const char* name, const char* state, void* user_data)
{
    int *triggered = user_data;
    printf("*");
    *triggered = 1;
}

void* getter(void* data)
{
    int i = 0;
    dpm_context_h context;
    volatile int triggered = 0;;

    printf("Policy receiver %d is ready\n", *((int *)data));

    while(1) {
        context = dpm_context_create();
        if (context == NULL) {
            printf("Failed to create client context\n");
            return (void *)TEST_FAILED;
        }

        int id;
        dpm_context_add_policy_changed_cb(context, "camera", device_policy_context_callback, (void *)&triggered, &id);

        while (!triggered) {
            if (completed) {
                dpm_context_remove_policy_changed_cb(context, id);
                dpm_context_destroy(context);
                return (void *)TEST_SUCCESSED;
            }
        }

        triggered = 0;

        if ((i % 10) == 0) {
            printf("\n");
        }

        dpm_context_remove_policy_changed_cb(context, id);
        dpm_context_destroy(context);

        printf("G");

        i++;
    }

    return (void *)TEST_SUCCESSED;
}

void* setter(void *data)
{
    int i;
    dpm_context_h context;

    printf("Thread setter %d is ready\n", *((int *)data));

    for (i = 0; i < MAX_ITERATIONS; i++) {
        context = dpm_context_create();
        if (context == NULL) {
            printf("Failed to create client context\n");
            completed = 1;
            return (void *)TEST_FAILED;
        }

        dpm_restriction_policy_h policy = dpm_context_acquire_restriction_policy(context);

        int state = 0;

        dpm_restriction_get_camera_state(policy, &state);
        dpm_restriction_set_camera_state(policy, state ? 0 : 1);

        dpm_context_release_restriction_policy(context, policy);

        if ((i % 10) == 0) {
            printf("\n");
        }

        printf("S");

        dpm_context_destroy(context);

    }
    printf("\n");

    completed = 1;

    return (void *)TEST_SUCCESSED;
}

static int device_policy_context(struct testcase* tc)
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

struct testcase device_policy_context_testcase = {
    .description = "device policy context",
    .handler = device_policy_context
};

void TESTCASE_CONSTRUCTOR device_policy_context_build_testcase(void)
{
    testbench_populate_testcase(&device_policy_context_testcase);
}

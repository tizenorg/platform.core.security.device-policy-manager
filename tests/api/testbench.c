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

#include "testbench.h"

static struct testcase *testbench_testcase_head = NULL;

void testbench_populate_testcase(struct testcase* tc)
{
    tc->next = testbench_testcase_head;
    testbench_testcase_head = tc;
}

void testbench_execute_testcases(void)
{
    int passed, failed;;
    struct testcase *tc = testbench_testcase_head;

    passed = failed = 0;

    while (tc != NULL) {
        int ret;
        ret = tc->handler(tc);
        if (ret == TEST_SUCCESSED) {
            passed++;
        } else {
            failed++;
        }

        printf("TESTCASE(%s) = %s\n", tc->description,
               ret == TEST_SUCCESSED ? "SUCCESSED" : "FAILED");

        tc = tc->next;
    }

    printf("Test finished: passed(%d), failed(%d)\n", passed, failed);
}

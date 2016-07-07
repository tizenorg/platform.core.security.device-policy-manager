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

#ifndef __DPM_API_TESTBENCH_H__
#define __DPM_API_TESTBENCH_H__

#include <stdio.h>

#define TEST_SUCCESSED  1
#define TEST_FAILED     0

#define TESTCASE_CONSTRUCTOR __attribute__((constructor))

struct testcase {
	char* description;
	int (*handler)(struct testcase* tc);
	struct testcase* next;
};

void testbench_populate_testcase(struct testcase* tc);
void testbench_execute_testcases(void);

#endif /* !__DPM_API_TESTBENCH_H__ */

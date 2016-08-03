/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <string>
#include <vector>

#include "eventfd.h"
#include "error.h"
#include "exception.h"
#include "audit/logger.h"

#include "testbench/testbench.h"

TESTCASE(EventFdHandleNegative)
{
	try {
		runtime::EventFD evtfd(0, -1);
	} catch (runtime::Exception& e) {
	}
}

TESTCASE(EventFdSendPositive)
{
	try {
		runtime::EventFD evtfd;
		evtfd.send();
		evtfd.receive();
	} catch (runtime::Exception& e) {
		TEST_FAIL(e.what());
	}
}

TESTCASE(EventFdSendNegative)
{
	try {
		runtime::EventFD evtfd;
		evtfd.close();
		evtfd.send();
	} catch (runtime::Exception& e) {
	}
}

TESTCASE(EventFdReceiveNegative)
{
	try {
		runtime::EventFD evtfd;
		evtfd.close();
		evtfd.receive();
	} catch (runtime::Exception& e) {
	}
}



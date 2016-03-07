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

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "user.hxx"
#include "group.hxx"

#include "exception.hxx"
#include "testbench/testbench.hxx"

TESTCASE(GetGroupTest)
{
    try {
        Runtime::Group group("users");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(CreateGroupTest)
{
    try {
        Runtime::Group group = Runtime::Group::create("testgroup");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }

    try {
        Runtime::Group group("testgroup");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(RemoveGroupTest)
{
    try {
        Runtime::Group group("testgroup");
        group.remove();
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }

    try {
        Runtime::Group group("testgroup");
    } catch (Runtime::Exception& e) {}

    TEST_FAIL("Failed to remove group");
}

TESTCASE(GetUserTest)
{
    try {
        Runtime::User user("root");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(CreateUserTest)
{
    try {
        Runtime::User user = Runtime::User::create("testuser", "testgroup");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
    try {
        Runtime::User user("testuser");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(RemoveUserTest)
{
    try {
        Runtime::User user("testuser");
        user.remove();
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }

    try {
        Runtime::Group group("testgroup");
        group.remove();
    } catch (Runtime::Exception& e) {}

    try {
        Runtime::Group group("testuser");
    } catch (Runtime::Exception& e) {}

    TEST_FAIL("Failed to remove group");
}

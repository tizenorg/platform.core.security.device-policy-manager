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

#include "shadow.hxx"

#include "exception.hxx"
#include "testbench/testbench.hxx"

TESTCASE(GetGroupTest)
{
    try {
        Shadow::Group group("users");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(CreateGroupTest)
{
    try {
        Shadow::Group group = Shadow::Group::create("testgroup");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(RemoveGroupTest)
{
    try {
        Shadow::Group group("testgroup");
        group.remove();
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(GetUserTest)
{
    try {
        Shadow::User user("root");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(CreateUserTest)
{
    try {
        Shadow::User user = Shadow::User::create("testuser", "testgroup");
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(RemoveUserTest)
{
    try {
        Shadow::User user("testuser");
        user.remove();
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }

    try {
        Shadow::Group group("testgroup");
        group.remove();
    } catch (Runtime::Exception& e) {}
}


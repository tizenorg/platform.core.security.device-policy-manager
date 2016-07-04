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

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <stdexcept>

#include "exception.h"
#include "filesystem.h"

#include "testbench/testbench.h"

TESTCASE(DirectoryIteration)
{
    runtime::DirectoryIterator iter(runtime::Path("/dev"));
    runtime::DirectoryIterator end;

    TEST_EXPECT(false, iter == end);

    while (iter != end) {
        ++iter;
    }
}

TESTCASE(FileIO)
{
    char readbuf[100];
    char testbuf[100] = "Test Data";

    runtime::File tmp("/tmp/test-file");
    try {
        tmp.create(O_RDWR);
        tmp.write(testbuf, ::strlen(testbuf));
        tmp.read(readbuf, ::strlen(testbuf));
        tmp.close();
    } catch (runtime::Exception& e) {
        TEST_FAIL(e.what());
    }

    try {
        runtime::File tmpFile("/tmp/test-file", O_RDWR);
        tmpFile.close();
        tmpFile.remove();
    } catch (runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(FileAttribute)
{
    runtime::File tmp("/tmp");
    TEST_EXPECT(true, tmp.exists());
    TEST_EXPECT(true, tmp.canRead());
    TEST_EXPECT(true, tmp.canWrite());
    TEST_EXPECT(true, tmp.canExecute());
    TEST_EXPECT(false, tmp.isLink());
    TEST_EXPECT(false, tmp.isFile());
    TEST_EXPECT(true, tmp.isDirectory());
    TEST_EXPECT(false, tmp.isDevice());

    runtime::File testDir("/tmp/dpm-unit-test/dir");
    try {
        testDir.makeDirectory(true, ::getuid(), ::getgid());
        testDir.chown(::getuid(), ::getgid(), false);
    } catch (runtime::Exception& e) {
        TEST_FAIL(e.what());
    }

    runtime::File dir("/tmp/dpm-unit-test");
    try {
        dir.remove(true);
    } catch (runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(Path)
{
    runtime::Path tmp("/tmp");
    TEST_EXPECT(true, tmp.isAbsolute());
    TEST_EXPECT(false, tmp.isRelative());

    TEST_EXPECT("/tmp", tmp.getPathname());
    TEST_EXPECT("tmp", tmp.getFilename());

    runtime::Path copy = tmp;
    TEST_EXPECT(true, copy == tmp);

    TEST_EXPECT(true, copy.isAbsolute());
    TEST_EXPECT(false, copy.isRelative());

    TEST_EXPECT("/tmp", copy.getPathname());
    TEST_EXPECT("tmp", copy.getFilename());
}

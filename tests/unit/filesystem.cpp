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

    while (iter != end) {
        runtime::File &file = *iter;
        std::cout << "File Name: " << file.getPath()
                  << "   Is Dev File: " << file.isDevice()
                  << "   Is Reg FIle: " << file.isFile()
                  << "   Is Directory: " << file.isDirectory()
                  << "   Is Link: " << file.isLink()
                  << std::endl;
        ++iter;
    }
}


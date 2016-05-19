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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <execinfo.h>

#include "backtrace.h"
#include "error.h"
#include "exception.h"

#define BACKTRACING_ENTRIES    128

namespace runtime {

void backtrace(const std::string& filename)
{
    int fd, ret;
    void *buffer[BACKTRACING_ENTRIES];

    do {
        fd = ::open(filename.c_str(), O_WRONLY | O_CREAT, 0777);
    } while ((fd == -1) && (errno == EINTR));

    if (fd == -1) {
        throw runtime::Exception(runtime::GetSystemErrorMessage());
    }

    int nptrs = ::backtrace(buffer, BACKTRACING_ENTRIES);
    ::backtrace_symbols_fd(buffer, nptrs, fd);

    do {
        ret = ::close(fd);
    } while ((ret == -1) && (errno == EINTR));
}

} // namespace runtime

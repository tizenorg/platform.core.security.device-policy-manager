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


#include <regex>

#include <shadow.h>
#include <unistd.h>
#include <sys/types.h>

#include "shadow/lock.hxx"
#include "exception.hxx"

namespace Shadow {

std::mutex PwdFileLock::mutex;

void PwdFileLock::lock()
{
    mutex.lock();

    if (::seteuid(0) != 0) {
        throw Runtime::Exception("failed to change euid");
    }

    if (::lckpwdf() != 0) {
        throw Runtime::Exception("Pwd file lock error");
    }

    isLocked = true;
}

void PwdFileLock::unlock()
{
    if (::seteuid(getuid()) != 0) {
        throw Runtime::Exception("failed to change euid");
    }

    if (::ulckpwdf() != 0) {
        throw Runtime::Exception("Pwd file unlock error");
    }

    isLocked = false;

    mutex.unlock();
}

PwdFileLock::~PwdFileLock()
{
    if (isLocked) {
        unlock();
    }
}

} // namespace Shadow

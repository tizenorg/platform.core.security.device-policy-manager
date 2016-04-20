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
#include <mutex>
#include <memory>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "smack.h"
#include "shadow.h"
#include "exception.h"
#include "filesystem.h"
#include "audit/logger.h"

namespace runtime {

//PwdFileLock
class PwdFileLock final {
public:
    PwdFileLock();
    ~PwdFileLock();

private:
    std::unique_lock<std::mutex> lock;
    static std::mutex mutex;
};

std::mutex PwdFileLock::mutex;

PwdFileLock::PwdFileLock() :
    lock(mutex)
{
    if (::seteuid(0) != 0) {
        throw runtime::Exception("failed to change euid");
    }

    if (::lckpwdf() != 0) {
        throw runtime::Exception("shadow file lock error");
    }
}

PwdFileLock::~PwdFileLock()
{
    if (::ulckpwdf() != 0) {
        throw runtime::Exception("shadow file unlock error");
    }

    if (::seteuid(getuid()) != 0) {
        throw runtime::Exception("failed to change euid");
    }
}

//Shadow
template<typename pwdStruct>
void Shadow::put(const std::string& filename, const pwdStruct& pwd,
                 std::function<int(const pwdStruct*, FILE*)> put)
{
    PwdFileLock pwdLock;

    std::unique_ptr<FILE, decltype(&::fclose)> fp_pwd
    (::fopen(filename.c_str(), "a"), &::fclose);

    if (fp_pwd.get() == NULL) {
        throw runtime::Exception("shadow file open error");
    }

    if (put(&pwd, fp_pwd.get())) {
        throw runtime::Exception("shadow file write error");
    }

    fp_pwd.reset();
}

template<typename pwdStruct>
void Shadow::foreach(const std::string& filename,
                    std::function<int(const pwdStruct*, FILE*)> put,
                    std::function<pwdStruct *(FILE*)> get,
                    std::function<bool(pwdStruct&)> check)
{
    std::string tmpfilename = filename + ".tmp";
    pwdStruct* ppwd;
    mode_t old_mask;
    struct stat st;

    old_mask = ::umask(0777);

    std::unique_ptr<FILE, void(*)(FILE*)> fp_tmp_pwd(
        ::fopen(tmpfilename.c_str(), "w"),
    [](FILE * fp) {
        if (fp == NULL) {
            return;
        }
        ::fclose(fp);
    });

    ::umask(old_mask);

    if (fp_tmp_pwd.get() == NULL) {
        throw runtime::Exception("Tmp file for shadow create error");
    }

    if (::stat(filename.c_str(), &st) != 0) {
        throw runtime::Exception("shadow file information get error");
    }

    PwdFileLock pwdLock;

    std::unique_ptr<FILE, void(*)(FILE*)> fp_pwd(
        ::fopen(filename.c_str(), "r"),
    [](FILE * fp) {
        if (fp == NULL) {
            return;
        }
        ::fclose(fp);
    });

    if (fp_pwd.get() == NULL) {
        throw runtime::Exception("shadow file open error");
    }

    for (ppwd = get(fp_pwd.get()); ppwd != NULL; ppwd = get(fp_pwd.get()))
        if (check(*ppwd))
            if (put(ppwd, fp_tmp_pwd.get()) != 0) {
                throw runtime::Exception("Tmp file for shadow write error");
            }

    fp_pwd.reset();

    ::fflush(fp_tmp_pwd.get());

    if (::chown(tmpfilename.c_str(), 0, 0) != 0) {
        throw runtime::Exception("Shadow file chown error");
    }

    if (::chmod(tmpfilename.c_str(), st.st_mode) != 0) {
        throw runtime::Exception("Shadow file chmod error");
    }

    runtime::File tmpfile(tmpfilename);
    runtime::Smack::setAccess(tmpfile, "_");

    if (::rename(tmpfilename.c_str(), filename.c_str()) != 0) {
        throw runtime::Exception("shadow file update error");
    }
}


void Shadow::putPasswd(const std::string& filename, const struct passwd& ent)
{
    put<struct passwd>(filename, ent, putpwent);
}

void Shadow::putShadow(const std::string& filename, const struct spwd& ent)
{
    put<struct spwd>(filename, ent, putspent);
}

void Shadow::putGroup(const std::string& filename, const struct group& ent)
{
    put<struct group>(filename, ent, putgrent);
}

void Shadow::putGshadow(const std::string& filename, const struct sgrp& ent)
{
    put<struct sgrp>(filename, ent, putsgent);
}


void Shadow::foreachPasswd(const std::string& filename, std::function<bool(passwd&)> check)
{
    foreach<struct passwd>(filename, putpwent, fgetpwent, check);
}

void Shadow::foreachShadow(const std::string& filename, std::function<bool(spwd&)> check)
{
    foreach<struct spwd>(filename, putspent, fgetspent, check);
}

void Shadow::foreachGroup(const std::string& filename, std::function<bool(group&)> check)
{
    foreach<struct group>(filename, putgrent, fgetgrent, check);
}

void Shadow::foreachGshadow(const std::string& filename, std::function<bool(sgrp&)> check)
{
    foreach<struct sgrp>(filename, putsgent, fgetsgent, check);
}

} // namespace runtime

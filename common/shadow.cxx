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

#include "shadow.hxx"
#include "exception.hxx"
#include "filesystem.hxx"
#include "audit/logger.hxx"

#define TEMPORARY_UMASK(mode)   \
        std::unique_ptr<mode_t, void(*)(mode_t *)> umask_##mode(new mode_t, \
        [](mode_t *prev) {umask(*prev);}); \
        *umask_##mode = mode;

namespace Runtime {

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

PwdFileLock::PwdFileLock()
   : lock(mutex)
{
    if (::seteuid(0) != 0) {
        throw Runtime::Exception("failed to change euid");
    }

    if (::lckpwdf() != 0) {
        throw Runtime::Exception("shadow file lock error");
    }
}

PwdFileLock::~PwdFileLock()
{
    if (::ulckpwdf() != 0) {
        throw Runtime::Exception("shadow file unlock error");
    }

    if (::seteuid(getuid()) != 0) {
        throw Runtime::Exception("failed to change euid");
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
        throw Runtime::Exception("shadow file open error");
    }

    if (put(&pwd, fp_pwd.get())) {
        throw Runtime::Exception("shadow file write error");
    }

    fp_pwd.reset();
}

template<typename pwdStruct, typename element>
void Shadow::remove(const std::string& filename, const element& value,
                    std::function<int(const pwdStruct*, FILE*)> put,
                    std::function<pwdStruct *(FILE*)> get,
                    std::function<bool(const pwdStruct&, const element&)> compare)
{
    std::string tmpfilename = filename + ".tmp";
    pwdStruct* ppwd;
    struct stat st;

    TEMPORARY_UMASK(0777);

    std::unique_ptr<FILE, void(*)(FILE*)> fp_tmp_pwd(
        ::fopen(tmpfilename.c_str(), "w"),
    [](FILE * fp) {
        if (fp == NULL) {
            return;
        }
        ::fclose(fp);
    });

    if (fp_tmp_pwd.get() == NULL) {
        throw Runtime::Exception("Tmp file for shadow create error");
    }

    if (::stat(filename.c_str(), &st) != 0) {
        throw Runtime::Exception("shadow file information get error");
    }

    PwdFileLock pwdLock;

    std::unique_ptr<FILE, decltype(&::fclose)> fp_pwd
    (::fopen(filename.c_str(), "r"), &::fclose);

    if (fp_pwd.get() == NULL) {
        throw Runtime::Exception("shadow file open error");
    }

    for (ppwd = get(fp_pwd.get()); ppwd != NULL; ppwd = get(fp_pwd.get()))
        if (!compare(*ppwd, value))
            if (put(ppwd, fp_tmp_pwd.get()) != 0) {
                throw Runtime::Exception("Tmp file for shadow write error");
            }

    fp_pwd.reset();

    ::fflush(fp_tmp_pwd.get());

    if (::chown(tmpfilename.c_str(), 0, 0) != 0) {
        throw Runtime::Exception("Shadow file chown error");
    }

    if (::chmod(tmpfilename.c_str(), st.st_mode) != 0) {
        throw Runtime::Exception("Shadow file chmod error");
    }

    if (::rename(tmpfilename.c_str(), filename.c_str()) != 0) {
        throw Runtime::Exception("shadow file update error");
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


void Shadow::removePasswd(const std::string& filename, uid_t uid)
{
    remove<struct passwd, uid_t>(filename, uid, putpwent, fgetpwent,
    [](const struct passwd & pwd, const uid_t & uid) -> bool {
        return pwd.pw_uid == uid;
    });
}

void Shadow::removeShadow(const std::string& filename, const std::string& user)
{
    remove<struct spwd, std::string>(filename, user, putspent, fgetspent,
    [](const struct spwd & spwd, const std::string & user) -> bool {
        return spwd.sp_namp == user;
    });
}

void Shadow::removeGroup(const std::string& filename, gid_t gid)
{
    remove<struct group, gid_t>(filename, gid, putgrent, fgetgrent,
    [](const struct group & grp, const gid_t & gid) -> bool {
        return grp.gr_gid == gid;
    });
}

void Shadow::removeGshadow(const std::string& filename, const std::string& group)
{
    remove<struct sgrp, std::string>(filename, group, putsgent, fgetsgent,
    [](const struct sgrp & sgrp, const std::string & group) -> bool {
        return sgrp.sg_namp == group;
    });
}

} // namespace Shadow

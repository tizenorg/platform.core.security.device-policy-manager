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
#include <memory>

#include <pwd.h>
#include <shadow.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "user.h"
#include "group.h"
#include "shadow.h"
#include "filesystem.h"
#include "exception.h"

#define PASSWD_DIR_PATH     "/etc/"
#define PASSWD_FILE_NAME    "passwd"
#define SHADOW_FILE_NAME    "shadow"

#define HOMEDIR_PATH        "/home"
#define SHELL_PATH      "/bin/bash"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

namespace runtime {

User::User(const User& user) :
    name(user.name), uid(user.uid), gid(user.gid)
{
}

User::User(const std::string& user)
{
    struct passwd pwd, *result;
    int bufsize;

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    std::unique_ptr<char[]> buf(new char[bufsize]);

    ::getpwnam_r(user.c_str(), &pwd, buf.get(), bufsize, &result);
    if (result == NULL) {
        throw runtime::Exception("User " + user + " doesn't exist");
    }

    name = result->pw_name;
    uid = result->pw_uid;
    gid = result->pw_gid;
}

User::User(const uid_t user)
{
    struct passwd pwd, *result;
    int bufsize;

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    std::unique_ptr<char[]> buf(new char[bufsize]);
    ::getpwuid_r(user, &pwd, buf.get(), bufsize, &result);
    if (result == NULL) {
        throw runtime::Exception("User " + std::to_string(user) + "doesn't exist");
    }

    name = result->pw_name;
    uid = result->pw_uid;
    gid = result->pw_gid;
}

User::User() :
    User(::getuid())
{
}


static std::regex userNamePattern(NAME_PATTERN);

User User::create(const std::string& name, const std::string& group_name,
                  const uid_t min, const uid_t max)
{
    runtime::File home(HOMEDIR_PATH "/" + name);
    struct passwd pwd, tmppwd, *result;
    struct spwd spwd;
    int bufsize;

    if (!std::regex_match(name, userNamePattern)) {
        throw runtime::Exception("Invalid user name : " + name);
    }

    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    std::unique_ptr<char[]> buf(new char[bufsize]);

    ::getpwnam_r(name.c_str(), &tmppwd, buf.get(), bufsize, &result);
    if (result != NULL) {
        return User(name);
    }

    //prepare passwd structure
    std::unique_ptr<char, decltype(&::free)>
    pw_name(::strdup(name.c_str()), ::free),
            pw_passwd(::strdup("x"), ::free),
            pw_gecos(::strdup(""), ::free),
            pw_dir(::strdup(home.getPath().c_str()), ::free),
            pw_shell(::strdup(SHELL_PATH), ::free);

    pwd.pw_name = pw_name.get();
    pwd.pw_passwd = pw_passwd.get();
    pwd.pw_gecos = pw_gecos.get();
    pwd.pw_dir = pw_dir.get();
    pwd.pw_shell = pw_shell.get();

    //prepare passwd structure
    std::unique_ptr<char, decltype(&::free)>
    sp_namp(::strdup(name.c_str()), ::free),
            sp_pwdp(::strdup("!"), ::free);
    spwd.sp_namp = sp_namp.get();
    spwd.sp_pwdp = sp_pwdp.get();
    spwd.sp_lstchg = (long) time((time_t*) 0) / (24 * 60 * 60);
    spwd.sp_min = 0;
    spwd.sp_max = 99999;
    spwd.sp_warn = 7;
    spwd.sp_inact = -1;
    spwd.sp_expire = -1;
    spwd.sp_flag = -1;


    //get free uid
    for (pwd.pw_uid = min; pwd.pw_uid <= max; pwd.pw_uid++) {
        ::getpwuid_r(pwd.pw_uid, &tmppwd, buf.get(), bufsize, &result);
        if (result == NULL) {
            break;
        }
    }

    if (pwd.pw_uid > max) {
        throw runtime::Exception("Too many users");
    }

    pwd.pw_gid = Group::create(group_name).getGid();

    Shadow::putPasswd(PASSWD_DIR_PATH PASSWD_FILE_NAME, pwd);
    Shadow::putShadow(PASSWD_DIR_PATH SHADOW_FILE_NAME, spwd);

    try {
        home.remove(true);
    } catch (runtime::Exception& e) {}

    return User(name);
}

void User::remove()
{
    runtime::File home(HOMEDIR_PATH "/" + name);

    if (uid == INVALID_UID) {
        throw runtime::Exception("User is already removed");
    }

    Shadow::foreachPasswd(PASSWD_DIR_PATH PASSWD_FILE_NAME,
    [this](const struct passwd & pwd) -> bool {
        return pwd.pw_uid != uid;
    });
    Shadow::foreachShadow(PASSWD_DIR_PATH SHADOW_FILE_NAME,
    [this](const struct spwd & spwd) -> bool {
        return spwd.sp_namp != name;
    });

    try {
        home.remove(true);
    } catch (runtime::Exception& e) {}

    int ngroups = 0;
    getgrouplist(name.c_str(), gid, NULL, &ngroups);

    std::unique_ptr<gid_t[]> groups(new gid_t[ngroups]);
    getgrouplist(name.c_str(), gid, groups.get(), &ngroups);

    for (int i = 0; i < ngroups; i++) {
        Group grp(groups.get()[i]);
        grp.removeMember(name);
    }

    name = "";
    uid = INVALID_UID;
}

} // namespace runtime

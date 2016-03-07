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

#include "user.hxx"
#include "group.hxx"
#include "shadow.hxx"
#include "filesystem.hxx"
#include "exception.hxx"

#define PASSWD_DIR_PATH     "/etc/"
#define PASSWD_FILE_NAME    "passwd"
#define SHADOW_FILE_NAME    "shadow"

#define HOMEDIR_PATH        "/home"
#define SHELL_PATH      "/bin/bash"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

namespace Runtime {

User::User(const User& user)
    : name(user.name), uid(user.uid), gid(user.gid)
{
}

User::User(const std::string& user)
{
    struct passwd* pwd;

    pwd = ::getpwnam(user.c_str());
    if (pwd == NULL) {
        throw Runtime::Exception("User " + user + " doesn't exist");
    }

    name = pwd->pw_name;
    uid = pwd->pw_uid;
    gid = pwd->pw_gid;
}

User::User(const uid_t user)
{
    struct passwd* pwd;

    pwd = ::getpwuid(user);
    if (pwd == NULL) {
        throw Runtime::Exception("User " + std::to_string(user) + "doesn't exist");
    }

    name = pwd->pw_name;
    uid = pwd->pw_uid;
    gid = pwd->pw_gid;
}


static std::regex userNamePattern(NAME_PATTERN);

User User::create(const std::string& name, const std::string& group_name,
                  const uid_t min, const uid_t max)
{
    Runtime::File home(HOMEDIR_PATH "/" + name);
    struct passwd pwd;
    struct spwd spwd;
    uid_t uid;

    if (!std::regex_match(name, userNamePattern)) {
        throw Runtime::Exception("Invalid user name : " + name);
    }

    if (::getpwnam(name.c_str()) != NULL) {
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
    for (uid = min; uid <= max; uid++)
        if (::getpwuid(uid) == NULL) {
            break;
        }

    if (uid > max) {
        throw Runtime::Exception("Too many containers");
    }

    pwd.pw_uid = uid;

    pwd.pw_gid = Group::create(group_name).getGid();

    Shadow::putPasswd(PASSWD_DIR_PATH PASSWD_FILE_NAME, pwd);
    Shadow::putShadow(PASSWD_DIR_PATH SHADOW_FILE_NAME, spwd);

    try {
        home.remove(true);
    } catch (Runtime::Exception& e) {}

    return User(name);
}

void User::remove()
{
    Runtime::File home(HOMEDIR_PATH "/" + name);

    if (uid == INVALID_UID) {
        throw Runtime::Exception("User is already removed");
    }

    Shadow::removePasswd(PASSWD_DIR_PATH PASSWD_FILE_NAME, uid);
    Shadow::removeShadow(PASSWD_DIR_PATH SHADOW_FILE_NAME, name);

    try {
        home.remove(true);
    } catch (Runtime::Exception& e) {}

    name = "";
    uid = INVALID_UID;
}

} // namespace Runtime

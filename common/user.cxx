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
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "user.hxx"
#include "exception.hxx"

namespace Runtime {

User::User(const User& user)
    : name(user.name), uid(user.uid)
{
    group = user.group;
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

    group = Group(pwd->pw_gid);
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

    group = Group(pwd->pw_gid);
}

} // namespace Runtime

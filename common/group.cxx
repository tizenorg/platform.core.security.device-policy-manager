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

#include <grp.h>
#include <gshadow.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "group.hxx"
#include "shadow.hxx"
#include "exception.hxx"

#define GROUP_DIR_PATH      "/etc/"
#define GROUP_FILE_NAME     "group"
#define GSHADOW_FILE_NAME   "gshadow"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

namespace Runtime {

Group::Group(const Group& group) :
    name(group.name), gid(group.gid)
{
}

Group::Group(const std::string& group)
{
    struct group* grp;

    grp = ::getgrnam(group.c_str());
    if (grp == NULL) {
        throw Runtime::Exception("Group doesn't exist");
    }

    name = grp->gr_name;
    gid = grp->gr_gid;
}

Group::Group(const gid_t group)
{
    struct group* grp;

    grp = ::getgrgid(group);
    if (grp == NULL) {
        throw Runtime::Exception("Group doesn't exist");
    }

    name = grp->gr_name;
    gid = grp->gr_gid;
}


static std::regex groupNamePattern(NAME_PATTERN);

Group Group::create(const std::string& name, const gid_t min, const gid_t max)
{
    struct group group;
    struct sgrp sgrp;

    if (::getgrnam(name.c_str()) != NULL) {
        return Group(name);
    }

    if (!std::regex_match(name, groupNamePattern)) {
        throw Runtime::Exception("Invalid group name : " + name);
    }

    //prepare group structure
    std::unique_ptr<char, decltype(&::free)>
    gr_name(::strdup(name.c_str()), ::free),
            gr_passwd(::strdup("x"), ::free);

    group.gr_name = gr_name.get();
    group.gr_passwd = gr_passwd.get();
    group.gr_mem = NULL;

    //prepare gshadow structure
    std::unique_ptr<char, decltype(&::free)>
    sg_namp(::strdup(name.c_str()), ::free),
            sg_passwd(::strdup("!"), ::free);

    sgrp.sg_namp = sg_namp.get();
    sgrp.sg_passwd = sg_passwd.get();
    sgrp.sg_adm = NULL;
    sgrp.sg_mem = NULL;

    //prepare gid - get free gid
    for (group.gr_gid = min; group.gr_gid <= max; group.gr_gid++)
        if (::getgrgid(group.gr_gid) == NULL) {
            break;
        }

    if (group.gr_gid > max) {
        throw Runtime::Exception("Too many groups");
    }

    Shadow::putGroup(GROUP_DIR_PATH GROUP_FILE_NAME, group);
    Shadow::putGshadow(GROUP_DIR_PATH GSHADOW_FILE_NAME, sgrp);

    return Group(name);
}

void Group::remove()
{
    if (gid == INVALID_GID) {
        throw Runtime::Exception("Group is already removed");
    }

    Shadow::removeGroup(GROUP_DIR_PATH GROUP_FILE_NAME, gid);
    Shadow::removeGshadow(GROUP_DIR_PATH GSHADOW_FILE_NAME, name);

    name = "";
    gid = INVALID_GID;
}

} // namespace Shadow

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

#include "group.h"
#include "shadow.h"
#include "exception.h"

#define GROUP_DIR_PATH      "/etc/"
#define GROUP_FILE_NAME     "group"
#define GSHADOW_FILE_NAME   "gshadow"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

namespace runtime {

Group::Group(const Group& group) :
    name(group.name), gid(group.gid)
{
}

Group::Group(const std::string& group)
{
    struct group grp, *result;
    int bufsize;

    bufsize = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    std::unique_ptr<char[]> buf(new char[bufsize]);

    ::getgrnam_r(group.c_str(), &grp, buf.get(), bufsize, &result);
    if (result == NULL) {
        throw runtime::Exception("Group doesn't exist");
    }

    name = result->gr_name;
    gid = result->gr_gid;
}

Group::Group(const gid_t group)
{
    struct group grp, *result;
    int bufsize;

    bufsize = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    std::unique_ptr<char[]> buf(new char[bufsize]);

    ::getgrgid_r(group, &grp, buf.get(), bufsize, &result);
    if (result == NULL) {
        throw runtime::Exception("Group doesn't exist");
    }

    name = result->gr_name;
    gid = result->gr_gid;
}

Group::Group() :
    Group(::getgid())
{
}


static std::regex groupNamePattern(NAME_PATTERN);

Group Group::create(const std::string& name, const gid_t min, const gid_t max)
{
    struct group group, tmpgrp, *result;
    struct sgrp sgrp;
    int bufsize;

    bufsize = sysconf(_SC_GETGR_R_SIZE_MAX);
    if (bufsize == -1)
        bufsize = 16384;

    std::unique_ptr<char[]> buf(new char[bufsize]);
    ::getgrnam_r(name.c_str(), &tmpgrp, buf.get(), bufsize, &result);
    if (result != NULL) {
        return Group(name);
    }

    if (!std::regex_match(name, groupNamePattern)) {
        throw runtime::Exception("Invalid group name : " + name);
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
    for (group.gr_gid = min; group.gr_gid <= max; group.gr_gid++) {
        ::getgrgid_r(group.gr_gid, &tmpgrp, buf.get(), bufsize, &result);
        if (result == NULL) {
            break;
        }
    }

    if (group.gr_gid > max) {
        throw runtime::Exception("Too many groups");
    }

    Shadow::putGroup(GROUP_DIR_PATH GROUP_FILE_NAME, group);
    Shadow::putGshadow(GROUP_DIR_PATH GSHADOW_FILE_NAME, sgrp);

    return Group(name);
}

void Group::remove()
{
    if (gid == INVALID_GID) {
        throw runtime::Exception("Group is already removed");
    }

    Shadow::foreachGroup(GROUP_DIR_PATH GROUP_FILE_NAME,
    [this](const struct group & grp) -> bool {
        return grp.gr_gid != gid;
    });
    Shadow::foreachGshadow(GROUP_DIR_PATH GSHADOW_FILE_NAME,
    [this](const struct sgrp & sgrp) -> bool {
        return sgrp.sg_namp != name;
    });

    name = "";
    gid = INVALID_GID;
}

void Group::addMember(const std::string& name)
{
    char ** members = NULL;
    std::unique_ptr<char, decltype(&::free)>
        member(::strdup(name.c_str()), ::free);

    Shadow::foreachGroup(GROUP_DIR_PATH GROUP_FILE_NAME,
    [&member, &members, this](struct group & grp) -> bool {
        if (grp.gr_gid == gid) {
            int len = 0;

            if (grp.gr_mem)
                for (len = 0; grp.gr_mem[len]; len++);

            if (members)
                delete [] members;

            members = new char * [len + 22];

            for (int i = 0; i < len; i++)
                members[i] = grp.gr_mem[i];
            members[len] = member.get();
            members[len + 1] = NULL;

            grp.gr_mem = members; 
        }
        return true;
    });

    if (members)
        delete [] members;
}

void Group::removeMember(const std::string& name)
{
    char ** members = NULL;

    Shadow::foreachGroup(GROUP_DIR_PATH GROUP_FILE_NAME,
    [&name, &members, this](struct group & grp) -> bool {
        if (grp.gr_gid == gid) {
            int len = 0;

            if (grp.gr_mem)
                for (; grp.gr_mem[len]; len++);             

            if (members)
                delete [] members;

            members = new char * [len + 1];

            for (int i = 0; i <= len; i++)
                members[i] = NULL;

            for (int i = 0, j = 0; i < len; i++) {
                if (grp.gr_mem[i] != name)
                    members[j++] = grp.gr_mem[i];
            }

            grp.gr_mem = members; 
        }
        return true;
    });

    if (members)
        delete [] members;
}

} // namespace Shadow

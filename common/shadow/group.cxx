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
#include <grp.h>
#include <unistd.h>
#include <shadow.h>
#include <gshadow.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "shadow/group.hxx"
#include "shadow/lock.hxx"

#include "exception.hxx"
#include "audit/logger.hxx"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

#define GROUP_DIR_PATH      "/etc"
#define GROUP_FILE_NAME     "group"
#define GSHADOW_FILE_NAME   "gshadow"

#define TMPGRP_FILE_NAME    "container-tmp-grp"

#define TEMPORARY_UMASK(mode)   \
    std::unique_ptr<mode_t, void(*)(mode_t *)> umask_##mode(new mode_t, \
        [](mode_t *prev) {umask(*prev);}); \
        *umask_##mode = mode;

namespace Shadow {

static std::regex groupNamePattern(NAME_PATTERN);

Group Group::create(const std::string& name, const gid_t min, const gid_t max)
{
    struct group group;
    struct sgrp sgrp;
    PwdFileLock pwdlock;

    if (::getgrnam(name.c_str()) != NULL) {
        return Group(name);
    }

    if (!std::regex_match(name, groupNamePattern)) {
        throw Runtime::Exception("Invalid group name");
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

    pwdlock.lock();

    //prepare gid - get free gid
    for (group.gr_gid = min; group.gr_gid <= max; group.gr_gid++)
        if (::getgrgid(group.gr_gid) == NULL) {
            break;
        }

    if (group.gr_gid > max) {
        throw Runtime::Exception("Too many groups");
    }

    //write group file
    std::unique_ptr<FILE, decltype(&::fclose)> fp_grp
    (::fopen(GROUP_DIR_PATH "/" GROUP_FILE_NAME, "a"), &::fclose);

    if (fp_grp.get() == NULL) {
        throw Runtime::Exception("Group file open error");
    }

    if (::putgrent(&group, fp_grp.get())) {
        throw Runtime::Exception("Group file write error");
    }

    fp_grp.reset();

    //write gshadow file
    std::unique_ptr<FILE, decltype(&::fclose)> fp_sgrp
    (::fopen(GROUP_DIR_PATH "/" GSHADOW_FILE_NAME, "a"), &::fclose);

    if (fp_sgrp.get() == NULL) {
        throw Runtime::Exception("Group file open error");
    }

    if (::putsgent(&sgrp, fp_sgrp.get())) {
        throw Runtime::Exception("Group file write error");
    }

    fp_sgrp.reset();

    pwdlock.unlock();

    return Group(name);
}

void Group::remove()
{
    PwdFileLock pwdlock;
    struct group* pgrp;
    struct sgrp* psgrp;

    if (gid == INVALID_GID) {
        throw Runtime::Exception("Group is already removed");
    }

    TEMPORARY_UMASK(0033);

    std::unique_ptr<FILE, void(*)(FILE*)> fp_tmp_grp(
        ::fopen(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-1", "w"),
    [](FILE * fp) {
        if (fp == NULL) {
            return;
        }
        ::fclose(fp);
        ::unlink(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-1");
    });

    if (fp_tmp_grp.get() == NULL) {
        throw Runtime::Exception("Tmp file for group create error");
    }

    for (::setgrent(), pgrp = ::getgrent(); pgrp != NULL; pgrp = ::getgrent())
        if (pgrp->gr_gid != gid)
            if (::putgrent(pgrp, fp_tmp_grp.get()) != 0) {
                throw ("Tmp file for group write error");
            }

    ::fflush(fp_tmp_grp.get());

    TEMPORARY_UMASK(0777);

    std::unique_ptr<FILE, void(*)(FILE*)> fp_tmp_sgrp(
        ::fopen(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-2", "w"),
    [](FILE * fp) {
        if (fp == NULL) {
            return;
        }
        ::fclose(fp);
        ::unlink(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-2");
    });

    if (fp_tmp_sgrp.get() == NULL) {
        throw Runtime::Exception("Tmp file for gshadow create error");
    }

    for (::setsgent(), psgrp = ::getsgent(); psgrp != NULL;
            psgrp = ::getsgent())
        if (name != psgrp->sg_namp)
            if (::putsgent(psgrp, fp_tmp_sgrp.get()) != 0) {
                throw ("Tmp file for gshadow write error");
            }

    ::fflush(fp_tmp_sgrp.get());

    pwdlock.lock();

    if (::chown(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-1", 0, 0) != 0) {
        throw Runtime::Exception("Group file chown error");
    }

    if (::chown(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-2", 0, 0) != 0) {
        throw Runtime::Exception("Gshadow file chown error");
    }

    if (::rename(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-1",
                 GROUP_DIR_PATH "/" GROUP_FILE_NAME)) {
        throw Runtime::Exception("Group file update error");
    }

    /* there is bug on getsgent that can't get sgroup entry */
    /*
        if (rename(GROUP_DIR_PATH "/" TMPGRP_FILE_NAME "-2",
                   GROUP_DIR_PATH "/" GSHADOW_FILE_NAME))
            throw Runtime::Exception("Gshadow file update error");
    pwdlock.unlock();
    */

    pwdlock.unlock();

    name = "";
    gid = INVALID_GID;
}

Group::Group(const Group& group)
    : name(group.name), gid(group.gid)
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

} // namespace Shadow

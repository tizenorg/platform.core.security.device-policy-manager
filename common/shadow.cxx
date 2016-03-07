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

#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <gshadow.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tzplatform_config.h>

#include "shadow.hxx"

#include "exception.hxx"
#include "filesystem.hxx"
#include "audit/logger.hxx"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

#define PASSWD_DIR_PATH     "/etc"
#define PASSWD_FILE_NAME    "passwd"
#define SHADOW_FILE_NAME    "shadow"

#define GROUP_DIR_PATH      "/etc"
#define GROUP_FILE_NAME     "group"
#define GSHADOW_FILE_NAME   "gshadow"

#define HOMEDIR_PATH        "/home"
#define SHELL_PATH      "/bin/bash"

#define TMPGRP_FILE_NAME    "container-tmp-grp"
#define TMPPWD_FILE_NAME    "container-tmp-usr"

#define TEMPORARY_UMASK(mode)   \
        std::unique_ptr<mode_t, void(*)(mode_t *)> umask_##mode(new mode_t, \
        [](mode_t *prev) {umask(*prev);}); \
        *umask_##mode = mode;

namespace Shadow {

//PwdFileLock

class PwdFileLock final {
public:
    void lock(void);
    void unlock(void);

    ~PwdFileLock();

private:
    static std::mutex mutex;
    bool isLocked = false;
};

std::mutex PwdFileLock::mutex;

void PwdFileLock::lock()
{
    mutex.lock();

    isLocked = true;

    if (::seteuid(0) != 0) {
        throw Runtime::Exception("failed to change euid");
    }

    if (::lckpwdf() != 0) {
        throw Runtime::Exception("Pwd file lock error");
    }
}

void PwdFileLock::unlock()
{
    isLocked = false;

    if (::ulckpwdf() != 0) {
        throw Runtime::Exception("Pwd file unlock error");
    }

    if (::seteuid(getuid()) != 0) {
        throw Runtime::Exception("failed to change euid");
    }

    mutex.unlock();
}

PwdFileLock::~PwdFileLock()
{
    if (isLocked) {
        unlock();
    }
}

//User

static std::regex NamePattern(NAME_PATTERN);

User User::create(const std::string& name, const std::string& group_name,
                  const uid_t min, const uid_t max)
{
    Runtime::File home(HOMEDIR_PATH "/" + name);
    PwdFileLock pwdlock;
    struct passwd pwd;
    struct spwd spwd;
    uid_t uid;

    if (!std::regex_match(name, NamePattern)) {
        throw Runtime::Exception("Invalid user name");
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

    //prepare gid
    pwd.pw_gid = Group::create(group_name).getGid();

    pwdlock.lock();

    //get free uid
    for (uid = min; uid <= max; uid++)
        if (::getpwuid(uid) == NULL) {
            break;
        }

    if (uid > max) {
        throw Runtime::Exception("Too many containers");
    }

    pwd.pw_uid = uid;

    //write passwd file
    std::unique_ptr<FILE, decltype(&::fclose)> fp_pwd
    (::fopen(PASSWD_DIR_PATH "/" PASSWD_FILE_NAME, "a"), &::fclose);

    if (fp_pwd.get() == NULL) {
        throw Runtime::Exception("Passwd file open error");
    }

    if (::putpwent(&pwd, fp_pwd.get())) {
        throw Runtime::Exception("Passwd file write error");
    }

    fp_pwd.reset();

    //write shadow file
    std::unique_ptr<FILE, decltype(&::fclose)> fp_spwd
    (::fopen(PASSWD_DIR_PATH "/" SHADOW_FILE_NAME, "a"), &::fclose);

    if (fp_spwd.get() == NULL) {
        throw Runtime::Exception("Shadow file open error");
    }

    if (::putspent(&spwd, fp_spwd.get())) {
        throw Runtime::Exception("Shadow file write error");
    }

    fp_spwd.reset();

    pwdlock.unlock();

    try {
        home.remove(true);
    } catch (Runtime::Exception& e) {}

    return User(name);
}

void User::remove()
{
    Runtime::File home(HOMEDIR_PATH "/" + name);
    PwdFileLock pwdlock;
    struct passwd* ppwd;
    struct spwd* pspwd;

    if (uid == INVALID_UID) {
        throw Runtime::Exception("User is already removed");
    }

    TEMPORARY_UMASK(0033);

    std::unique_ptr<FILE, void(*)(FILE*)> fp_tmp_pwd(
        ::fopen(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-1", "w"),
    [](FILE * fp) {
        if (fp == NULL) {
            return;
        }
        ::fclose(fp);
        ::unlink(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-1");
    });

    if (fp_tmp_pwd.get() == NULL) {
        throw Runtime::Exception("Tmp file for pwd create error");
    }

    for (::setpwent(), ppwd = ::getpwent(); ppwd != NULL; ppwd = ::getpwent())
        if (ppwd->pw_uid != uid)
            if (::putpwent(ppwd, fp_tmp_pwd.get()) != 0) {
                throw Runtime::Exception("Tmp file for pwd write error");
            }

    ::fflush(fp_tmp_pwd.get());

    TEMPORARY_UMASK(0777);

    std::unique_ptr<FILE, void(*)(FILE*)> fp_tmp_spwd(
        ::fopen(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-2", "w"),
    [](FILE * fp) {
        if (fp == NULL) {
            return;
        }
        ::fclose(fp);
        ::unlink(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-2");
    });

    if (fp_tmp_spwd.get() == NULL) {
        throw Runtime::Exception("Tmp file for shadow create error");
    }

    for (::setspent(), pspwd = ::getspent(); pspwd != NULL;
            pspwd = ::getspent())
        if (name != pspwd->sp_namp)
            if (::putspent(pspwd, fp_tmp_spwd.get()) != 0) {
                throw Runtime::Exception("Tmp file for shadow write error");
            }

    ::fflush(fp_tmp_spwd.get());

    if (::chown(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-1", 0, 0) != 0) {
        throw Runtime::Exception("Passwd file chown error");
    }

    if (::chown(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-2", 0, 0) != 0) {
        throw Runtime::Exception("Shadow file chown error");
    }

    pwdlock.lock();

    if (::rename(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-1",
                 PASSWD_DIR_PATH "/" PASSWD_FILE_NAME) != 0) {
        throw Runtime::Exception("Passwd file update error");
    }

    if (::rename(PASSWD_DIR_PATH "/" TMPPWD_FILE_NAME "-2",
                 PASSWD_DIR_PATH "/" SHADOW_FILE_NAME) != 0) {
        throw Runtime::Exception("Shadow file update error");
    }

    pwdlock.unlock();

    home.remove(true);

    name = "";
    shell = "";
    uid = INVALID_UID;
}

User::User(const User& user)
    : name(user.name), uid(user.uid), shell(user.shell)
{
    group = user.group;
}

User::User(const std::string& user)
{
    struct passwd* pwd;

    pwd = ::getpwnam(user.c_str());
    if (pwd == NULL) {
        throw Runtime::Exception("User doesn't exist");
    }

    name = pwd->pw_name;
    uid = pwd->pw_uid;
    shell = pwd->pw_shell;

    try {
        group = Group(pwd->pw_gid);
    } catch (Runtime::Exception &e) {
        throw Runtime::Exception("User's group doesn't exist");
    }
}

User::User(const uid_t user)
{
    struct passwd* pwd;

    pwd = ::getpwuid(user);
    if (pwd == NULL) {
        throw Runtime::Exception("User doesn't exist");
    }

    name = pwd->pw_name;
    uid = pwd->pw_uid;
    shell = pwd->pw_shell;

    try {
        group = Group(pwd->pw_gid);
    } catch (Runtime::Exception &e) {
        throw Runtime::Exception("User's group doesn't exist");
    }
}

//Group

Group Group::create(const std::string& name, const gid_t min, const gid_t max)
{
    struct group group;
    struct sgrp sgrp;
    PwdFileLock pwdlock;

    if (::getgrnam(name.c_str()) != NULL) {
        return Group(name);
    }

    if (!std::regex_match(name, NamePattern)) {
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

Group::Group()
    : gid(INVALID_GID)
{
}



} // namespace Shadow

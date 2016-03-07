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
#include <shadow.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <tzplatform_config.h>

#include "shadow/user.hxx"
#include "shadow/group.hxx"
#include "shadow/lock.hxx"

#include "exception.hxx"
#include "filesystem.hxx"
#include "access-control.hxx"
#include "audit/logger.hxx"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

#define PASSWD_DIR_PATH     "/etc"
#define PASSWD_FILE_NAME    "passwd"
#define SHADOW_FILE_NAME    "shadow"

#define HOMEDIR_PATH        "/home"
#define SHELL_PATH      "/bin/bash"

#define HOME_SMACKLABEL     "User::Home"
#define SHARED_SMACKLABEL   "User::App::Shared"

#define TMPPWD_FILE_NAME    "container-tmp-usr"

#define TEMPORARY_UMASK(mode)   \
        std::unique_ptr<mode_t, void(*)(mode_t *)> umask_##mode(new mode_t, \
        [](mode_t *prev) {umask(*prev);}); \
        *umask_##mode = mode;

namespace Shadow {

static std::regex UserNamePattern(NAME_PATTERN);

User User::create(const std::string& name, const std::string& group_name,
                  const uid_t min, const uid_t max)
{
    Runtime::File home(HOMEDIR_PATH "/" + name);
    PwdFileLock pwdlock;
    struct passwd pwd;
    struct spwd spwd;
    uid_t uid;

    if (!std::regex_match(name, UserNamePattern)) {
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

    TEMPORARY_UMASK(0022);
    home.makeDirectory();
    home.chown(pwd.pw_uid, pwd.pw_gid);

    Runtime::SmackFileLabel home_label(home);
    home_label.setAccess(HOME_SMACKLABEL);
    home_label.setTransmute(true);

    struct {
        enum tzplatform_variable dir;
        const char* smack;
    } dirs[] = {
        {TZ_USER_CACHE, SHARED_SMACKLABEL},
        {TZ_USER_APPROOT, HOME_SMACKLABEL},
        {TZ_USER_DB, HOME_SMACKLABEL},
        {TZ_USER_PACKAGES, HOME_SMACKLABEL},
        {TZ_USER_ICONS, HOME_SMACKLABEL},
        {TZ_USER_CONFIG, SHARED_SMACKLABEL},
        {TZ_USER_DATA, HOME_SMACKLABEL},
        {TZ_USER_SHARE, SHARED_SMACKLABEL},
        {TZ_USER_ETC, HOME_SMACKLABEL},
        {TZ_USER_LIVE, HOME_SMACKLABEL},
        {TZ_USER_UG, HOME_SMACKLABEL},
        {TZ_USER_APP, HOME_SMACKLABEL},
        {TZ_USER_CONTENT, SHARED_SMACKLABEL},
        {TZ_USER_CAMERA, SHARED_SMACKLABEL},
        {TZ_USER_VIDEOS, SHARED_SMACKLABEL},
        {TZ_USER_IMAGES, SHARED_SMACKLABEL},
        {TZ_USER_SOUNDS, SHARED_SMACKLABEL},
        {TZ_USER_MUSIC, SHARED_SMACKLABEL},
        {TZ_USER_GAMES, SHARED_SMACKLABEL},
        {TZ_USER_DOCUMENTS, SHARED_SMACKLABEL},
        {TZ_USER_OTHERS, SHARED_SMACKLABEL},
        {TZ_USER_DOWNLOADS, SHARED_SMACKLABEL},
        {TZ_USER_HOME, NULL},
    };

    ::tzplatform_set_user(pwd.pw_uid);
    for (int i = 0; dirs[i].dir != TZ_USER_HOME; i++) {
        Runtime::File dir(std::string(::tzplatform_getenv(dirs[i].dir)));
        dir.makeDirectory();
        dir.chown(pwd.pw_uid, pwd.pw_gid);
        Runtime::SmackFileLabel dir_label(dir);
        dir_label.setAccess(dirs[i].smack);
        dir_label.setTransmute(true);
    }
    ::tzplatform_reset_user();

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
    group = "";
    uid = INVALID_UID;
    gid = INVALID_GID;
}

User::User(const User& user)
    : name(user.name), group(user.group), uid(user.uid), gid(user.gid),
      shell(user.shell)
{
}

User::User(const std::string& user)
{
    struct passwd* pwd;
    struct group* grp;

    pwd = ::getpwnam(user.c_str());
    if (pwd == NULL) {
        throw Runtime::Exception("User doesn't exist");
    }

    grp = ::getgrgid(pwd->pw_gid);
    if (grp == NULL) {
        throw Runtime::Exception("User's group doesn't exist");
    }

    name = pwd->pw_name;
    uid = pwd->pw_uid;
    group = grp->gr_name;
    gid = grp->gr_gid;

    shell = pwd->pw_shell;
}

} // namespace Shadow

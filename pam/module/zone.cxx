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

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <fcntl.h>
#include <unistd.h>
#include <syslog.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <security/pam_ext.h>
#include <security/pam_modules.h>

#include "mount.hxx"
#include "user.hxx"
#include "exception.hxx"
#include "filesystem.hxx"
#include "xml/parser.hxx"
#include "xml/document.hxx"

#define CONTAINER_MANIFEST_DIR CONF_PATH "/container/"

static void attachNamespace(pid_t pid, int flags)
{
    std::string ns[] = {"mnt", "net", "ipc", "uts", "user"};
    int nsflags[] = {CLONE_NEWNS, CLONE_NEWNET, CLONE_NEWIPC, CLONE_NEWUTS, CLONE_NEWUSER};
    std::stringstream nspath;
    int fd;
    unsigned int i;

    for (i = 0; i < sizeof(nsflags) / sizeof(nsflags[0]); i++) {
        if (!(flags & nsflags[i])) {
            continue;
        }

        nspath << "/proc" << pid << "/ns/" << ns[i];

        fd = open(nspath.str().c_str(), O_RDONLY);
        if (fd != -1) {
            if (setns(fd, 0)) {
                throw Runtime::Exception("Failed to setns : " + ns[i]);
            } else {
                close(fd);
            }
        } else {
            throw Runtime::Exception("Unable to open : " + nspath.str());
        }
    }
}

extern "C" {
    PAM_EXTERN  __attribute__((visibility("default"))) int
    pam_sm_open_session(pam_handle_t* pamh, int flags,
                        int argc, const char* argv[])
    {
        std::unique_ptr<Xml::Document> bundle;
        Xml::Node::NodeList nodes;
        int unshare_flags = 0;
        std::string dest, src, type, opts, create;
        std::string user;
        pid_t pid;

        const void* retItem;
        int error = ::pam_get_item(pamh, PAM_USER, &retItem);
        if (error != PAM_SUCCESS) {
            ::pam_syslog(pamh, LOG_ERR, "failed to get user");
            return PAM_SESSION_ERR;
        }
        user = static_cast<const char*>(retItem);

        if (user.size() == 0) {
            ::pam_syslog(pamh, LOG_ERR, "no user is specified");
            return PAM_SESSION_ERR;
        }

        if (user == "root") {
            return PAM_SUCCESS;
        }

        ::umask(077);

        ::mkdir(PID_FILE_PATH, 0700);

        unshare_flags = CLONE_NEWNS | CLONE_NEWNET | CLONE_NEWUTS | CLONE_NEWIPC;

        std::unique_ptr<sem_t, void(*)(sem_t*)> sem(
            ::sem_open("PAMZone", O_CREAT, 0700, 1),
        [](sem_t * sem) {
            if (sem == NULL) {
                return;
            }
            ::sem_post(sem);
            ::sem_close(sem);
            ::sem_unlink("PAMZone");
        });

        if (sem == NULL) {
            ::pam_syslog(pamh, LOG_ERR, "failed to open semaphore");
            return PAM_SESSION_ERR;
        }

        ::sem_wait(sem.get());

        std::fstream pidfile(PID_FILE_PATH "/" + user, std::ios::app);
        if (!pidfile.is_open()) {
            ::pam_syslog(pamh, LOG_ERR, "failed to open PID file");
            return PAM_SESSION_ERR;
        }

        if (!(pidfile >> pid).good()) {
            if (::unshare(unshare_flags) != 0) {
                ::pam_syslog(pamh, LOG_ERR, "failed to unshare namespace");
                return PAM_SESSION_ERR;
            }

            if (unshare_flags & CLONE_NEWNS) {
                if (::mount(NULL, "/", NULL, MS_SLAVE | MS_REC, NULL) < 0) {
                    ::pam_syslog(pamh, LOG_ERR, "failed to remount /");
                    return PAM_SESSION_ERR;
                }

                ::umask(022);
            }
            pidfile.clear();
        } else {
            attachNamespace(pid, unshare_flags);
        }
        pidfile <<::getpid() << std::endl;

        return PAM_SUCCESS;
    }

    PAM_EXTERN  __attribute__((visibility("default"))) int
    pam_sm_close_session(pam_handle_t* pamh, int flags,
                         int argc, const char* argv[])
    {
        std::string user, path, tmppath;
        pid_t pid, current_pid;
        bool is_empty = true;

        const void* retItem;
        int error = ::pam_get_item(pamh, PAM_USER, &retItem);
        if (error != PAM_SUCCESS) {
            ::pam_syslog(pamh, LOG_ERR, "failed to get user");
            return PAM_SESSION_ERR;
        }
        user = static_cast<const char*>(retItem);

        if (user.size() == 0) {
            ::pam_syslog(pamh, LOG_ERR, "no user is specified");
            return PAM_SESSION_ERR;
        }

        current_pid = ::getpid();

        ::umask(077);

        std::unique_ptr<sem_t, void(*)(sem_t*)> sem(
            ::sem_open("PAMZone", O_CREAT, 0700, 1),
        [](sem_t * sem) {
            if (sem == NULL) {
                return;
            }
            ::sem_post(sem);
            ::sem_close(sem);
            ::sem_unlink("PAMZone");
        });

        if (sem == NULL) {
            ::pam_syslog(pamh, LOG_ERR, "failed to open semaphore");
            return PAM_SESSION_ERR;
        }

        ::sem_wait(sem.get());

        path = PID_FILE_PATH "/" + user;
        std::ifstream pidfile(path);
        if (!pidfile.is_open()) {
            ::pam_syslog(pamh, LOG_ERR, "failed to open PID file");
            return PAM_SESSION_ERR;
        }

        tmppath = path + ":";
        std::ofstream tmppidfile(tmppath);
        if (!tmppidfile.is_open()) {
            ::pam_syslog(pamh, LOG_ERR, "failed to open temporary PID file");
            return PAM_SESSION_ERR;
        }

        while (!pidfile.eof()) {
            pidfile >> pid;
            if (pid != current_pid) {
                tmppidfile << pid << std::endl;
                is_empty = 0;
            }
        }
        pidfile.close();
        tmppidfile.close();

        if (::rename(tmppath.c_str(), path.c_str()) != 0) {
            ::unlink(tmppath.c_str());
            ::pam_syslog(pamh, LOG_ERR, "failed to renew PID file");
            return PAM_SESSION_ERR;
        }

        if (is_empty) {
            if (::unlink(path.c_str()) != 0) {
                ::pam_syslog(pamh, LOG_ERR, "failed to remove empty PID file");
                return PAM_SESSION_ERR;
            }
        }

        return PAM_SUCCESS;
    }

#ifdef PAM_MODULE_ENTRY
    PAM_MODULE_ENTRY("pam_zone");
#endif

} // namespace Runtime

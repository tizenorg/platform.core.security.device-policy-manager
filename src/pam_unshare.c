/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Sungbae Yoo <sungbae.yoo@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * @file
 * @author  Sungbae Yoo (sungbae.yoo@samsung.com)
 * @brief   PAM module to create new namespace
 */

#define _GNU_SOURCE
#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <limits.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/mount.h>

#include <security/pam_ext.h>
#include <security/pam_modules.h>

#include "fstab.h"

#ifndef PAM_EXTERN
#define PAM_EXTERN
#endif

#define SEM_NAME "pam_unshare"


static int attach_ns(pam_handle_t* pamh, pid_t pid, int flag)
{
    char* ns[] = {"mnt", "net", "ipc", "uts", "user"};
    int nsflags[] = {CLONE_NEWNS, CLONE_NEWNET, CLONE_NEWIPC, CLONE_NEWUTS, CLONE_NEWUSER};
    char ns_path[PATH_MAX];
    int fd;
    unsigned int i;

    for (i = 0; i < sizeof(nsflags) / sizeof(nsflags[0]); i++) {
        if (!(flag & nsflags[i])) {
            continue;
        }

        snprintf(ns_path, sizeof(ns_path), "/proc/%d/ns/%s", pid, ns[i]);

        fd = open(ns_path, O_RDONLY);
        if (fd != -1) {
            if (setns(fd, 0)) {
                pam_syslog(pamh, LOG_WARNING, "Failed to setns : %s\n", ns[i]);
                return -1;
            } else {
                close(fd);
            }
        } else {
            pam_syslog(pamh, LOG_WARNING, "Unable to open : %s\n", ns_path);
            return -1;

        }
    }

    return 0;
}

PAM_EXTERN int
pam_sm_open_session(pam_handle_t* pamh, int flags __attribute__((unused)),
                    int argc, const char* argv[])
{
    int i, ret = PAM_SUCCESS;
    const char* fstab_name = "/etc/security/unshare_fstab";
    int unshare_flags = CLONE_NEWNET | CLONE_NEWIPC | CLONE_NEWUTS | CLONE_NEWNS;
    char path[PATH_MAX], *user;
    pid_t pid;
    FILE* fp;
    sem_t* sem;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "share-net") == 0) {
            unshare_flags &= ~CLONE_NEWNET;
        } else if (strcmp(argv[i], "share-ipc") == 0) {
            unshare_flags &= ~CLONE_NEWIPC;
        } else if (strcmp(argv[i], "share-uts") == 0) {
            unshare_flags &= ~CLONE_NEWUTS;
        } else if (strstr(argv[i], "fstab=") == argv[i]) {
            fstab_name = argv[i] + 6;
        }
    }

    if (pam_get_item(pamh, PAM_USER, (const void**)&user) != PAM_SUCCESS) {
        pam_syslog(pamh, LOG_ERR, "failed to get user\n");
        return PAM_SESSION_ERR;
    }

    if (strcmp(user, "root") == 0) {
        return PAM_SUCCESS;
    }

    sem = sem_open(SEM_NAME, O_CREAT, 0700, 1);
    if (sem == NULL) {
        pam_syslog(pamh, LOG_ERR, "failed to open semaphore\n");
        return PAM_SESSION_ERR;
    }

    mkdir(PID_FILE_PATH, 0700);
    umask(077);

    snprintf(path, PATH_MAX, PID_FILE_PATH "/%s", user);

    sem_wait(sem);

    fp = fopen(path, "a+");
    if (fp == NULL) {
        pam_syslog(pamh, LOG_ERR, "failed to open PID file\n");
        ret = PAM_SESSION_ERR;
        goto sem_end;
    }

    umask(022);

    if (fscanf(fp, "%d", &pid) < 0) {
        if (unshare(unshare_flags) != 0) {
            pam_syslog(pamh, LOG_ERR, "failed to unshare namespace\n");
            ret = PAM_SESSION_ERR;
            goto pid_file_end;
        }

        if (unshare_flags & CLONE_NEWNS) {
            if (mount(NULL, "/", NULL, MS_SLAVE | MS_REC, NULL) < 0) {
                pam_syslog(pamh, LOG_ERR, "failed to remount /\n");
                ret = PAM_SESSION_ERR;
                goto pid_file_end;
            }

            if (mount_fstab(pamh, fstab_name)) {
                pam_syslog(pamh, LOG_ERR, "failed to mount fstab\n");
                ret = PAM_SESSION_ERR;
                goto pid_file_end;
            }
        }
    } else {
        if (attach_ns(pamh, pid, unshare_flags) != 0) {
            pam_syslog(pamh, LOG_ERR, "failed to attach namespace\n");
            ret = PAM_SESSION_ERR;
            goto pid_file_end;
        }
    }
    fprintf(fp, "%d\n", getpid());

pid_file_end:
    fclose(fp);

sem_end:
    sem_post(sem);
    sem_close(sem);
    sem_unlink(SEM_NAME);

    return ret;
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t* pamh, int flags __attribute__((unused)),
                     int argc __attribute__((unused)),
                     const char* argv[] __attribute__((unused)))
{
    char path[PATH_MAX], tmppath[PATH_MAX];
    char* user;
    FILE* fp, *tfp;
    int is_empty = 1, ret = PAM_SESSION_ERR;
    pid_t pid, current_pid;
    sem_t* sem;

    if (pam_get_item(pamh, PAM_USER, (const void**)&user) != PAM_SUCCESS) {
        pam_syslog(pamh, LOG_ERR, "failed to get user\n");
        return PAM_SESSION_ERR;
    }

    current_pid = getpid();

    sem = sem_open(SEM_NAME, O_CREAT, 0700, 1);
    if (sem == NULL) {
        pam_syslog(pamh, LOG_ERR, "failed to open semaphore\n");
        return PAM_SESSION_ERR;
    }

    snprintf(path, PATH_MAX, PID_FILE_PATH "/%s", user);
    snprintf(tmppath, PATH_MAX, "%s:", path);

    umask(077);

    sem_wait(sem);

    fp = fopen(path, "r");
    if (fp == NULL) {
        pam_syslog(pamh, LOG_ERR, "failed to open PID file\n");
        ret = PAM_SESSION_ERR;
        goto sem_end;
    }

    tfp = fopen(tmppath, "w");
    if (tfp == NULL) {
        pam_syslog(pamh, LOG_ERR, "failed to open temporary PID file\n");
        fclose(fp);
        ret = PAM_SESSION_ERR;
        goto sem_end;
    }

    while (fscanf(fp, "%d", &pid) > 0) {
        if (pid != current_pid) {
            fprintf(tfp, "%d\n", pid);
            is_empty = 0;
        }
    }
    fclose(fp);
    fclose(tfp);

    if (unlink(path) != 0) {
        pam_syslog(pamh, LOG_ERR, "failed to remove PID file\n");
        ret = PAM_SESSION_ERR;
        goto rename_err;
    }

    if (rename(tmppath, path) != 0) {
        pam_syslog(pamh, LOG_ERR, "failed to renew PID file\n");
        ret = PAM_SESSION_ERR;
        goto rename_err;
    }

    if (is_empty && unlink(path) != 0) {
        pam_syslog(pamh, LOG_ERR, "failed to remove empty PID file\n");
        ret = PAM_SESSION_ERR;
    }
    goto sem_end;

rename_err:
    if (unlink(tmppath) != 0) {
        pam_syslog(pamh, LOG_ERR, "failed to remove temporary PID file\n");
    }

sem_end:
    sem_post(sem);
    sem_close(sem);
    sem_unlink(SEM_NAME);

    return ret;
}

#ifdef PAM_MODULE_ENTRY
PAM_MODULE_ENTRY("pam_unshare");
#endif

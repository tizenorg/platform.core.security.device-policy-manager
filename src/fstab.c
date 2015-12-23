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
 * @brief   Implementation to process fstab file for mounts in mnt namespace
 */

#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mntent.h>
#include <limits.h>
#include <libgen.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>

#include <security/pam_ext.h>

static struct mount_opt {
    char* name;
    int clear;
    int flag;
} mount_opt[] = {
    { "defaults",      0, 0                },
    { "ro",            0, MS_RDONLY        },
    { "rw",            1, MS_RDONLY        },
    { "suid",          1, MS_NOSUID        },
    { "nosuid",        0, MS_NOSUID        },
    { "dev",           1, MS_NODEV         },
    { "nodev",         0, MS_NODEV         },
    { "exec",          1, MS_NOEXEC        },
    { "noexec",        0, MS_NOEXEC        },
    { "sync",          0, MS_SYNCHRONOUS   },
    { "async",         1, MS_SYNCHRONOUS   },
    { "dirsync",       0, MS_DIRSYNC       },
    { "remount",       0, MS_REMOUNT       },
    { "mand",          0, MS_MANDLOCK      },
    { "nomand",        1, MS_MANDLOCK      },
    { "atime",         1, MS_NOATIME       },
    { "noatime",       0, MS_NOATIME       },
    { "diratime",      1, MS_NODIRATIME    },
    { "nodiratime",    0, MS_NODIRATIME    },
    { "bind",          0, MS_BIND          },
    { "rbind",         0, MS_BIND | MS_REC },
    { "relatime",      0, MS_RELATIME      },
    { "norelatime",    1, MS_RELATIME      },
    { "strictatime",   0, MS_STRICTATIME   },
    { "nostrictatime", 1, MS_STRICTATIME   },
    { NULL,            0, 0                },
};

static int mkdir_p(const char* dir, mode_t mode)
{
    const char* tmp = dir;
    const char* orig = dir;
    char* makeme;

    do {
        dir = tmp + strspn(tmp, "/");
        tmp = dir + strcspn(dir, "/");
        makeme = strndup(orig, dir - orig);
        if (*makeme) {
            if (mkdir(makeme, mode) && errno != EEXIST) {
                free(makeme);
                return -1;
            }
        }
        free(makeme);
    } while (tmp != dir);

    return 0;
}

static int parse_mntopts(const char* mntopts, unsigned long* mntflags,
                         char** mntdata)
{
    char* s, *data;
    char* p, *saveptr = NULL;
    struct mount_opt* mo;

    *mntdata = NULL;
    *mntflags = 0L;

    if (mntopts == NULL) {
        return 0;
    }

    s = strdup(mntopts);
    if (s == NULL) {
        return -1;
    }

    data = malloc(strlen(s) + 1);
    if (data == NULL) {
        free(s);
        return -1;
    }
    *data = '\0';

    for (p = strtok_r(s, ",", &saveptr); p != NULL;
            p = strtok_r(NULL, ",", &saveptr)) {
        for (mo = &mount_opt[0]; mo->name != NULL; mo++) {
            if (strcmp(p, mo->name) == 0) {
                if (mo->clear) {
                    *mntflags &= ~mo->flag;
                } else {
                    *mntflags |= mo->flag;
                }
                break;
            }
        }
        if (mo->name != NULL) {
            continue;
        }

        if (*data) {
            strcat(data, ",");
        }
        strcat(data, p);
    }

    if (*data) {
        *mntdata = data;
    } else {
        free(data);
    }
    free(s);

    return 0;
}

static int preparse_create_mntopts(const char* target, struct mntent* mntent)
{
    FILE* pathfile = NULL;
    char* pathdirname = NULL;

    if (hasmntopt(mntent, "create=dir")) {
        if (mkdir_p(target, 0755) < 0) {
            return -1;
        }
    }

    if (hasmntopt(mntent, "create=file") && access(target, F_OK)) {
        pathdirname = strdup(target);
        if (pathdirname == NULL) {
            return -1;
        }
        mkdir_p(dirname(pathdirname), 0755);
        free(pathdirname);
        pathfile = fopen(target, "wb");
        if (pathfile == NULL) {
            return -1;
        }
        fclose(pathfile);
    }

    return 0;
}

static void cleanup_preparsed_mntopts(struct mntent* mntent)
{
    int i;
    char* p, *p2;
    char* list[] = {"create=dir",
                    "create=file",
                    "optional",
                    NULL
                   };

    for (i = 0; list[i]; i++) {
        p = strstr(mntent->mnt_opts, list[i]);
        if (p == NULL) {
            continue;
        }
        p2 = strchr(p, ',');
        if (p2 == NULL) {
            *p = '\0';
            continue;
        }
        memmove(p, p2 + 1, strlen(p2 + 1) + 1);
    }
}

static char* convert_param(struct passwd* pwd, const char* param_path)
{
    char path[PATH_MAX], *ptr = path;
    const char* word_ptr, *param_ptr = param_path;
    int size, ret;

    while (1) {
        word_ptr = strchr(param_ptr, '%');
        if (word_ptr == NULL) {
            break;
        }

        size = word_ptr - param_ptr;
        memcpy(ptr, param_ptr, size);
        ptr += size;
        param_ptr = word_ptr + 1;

        *(ptr++) = '%';

        size = sizeof(path) - (param_ptr - param_path);
        switch (param_ptr[0]) {
        case 'U':
            switch (param_ptr[1]) {
            case 'S':
                /* %USER% */
                if (strncmp(param_ptr + 2, "ER%", 3) == 0) {
                    param_ptr += 5;
                    ret = snprintf(ptr - 1, size, "%s", pwd->pw_name);
                    if (ret < 0) {
                        return NULL;
                    }
                    while (*(ptr++));
                    ptr--;
                }
                break;
            case 'I':
                /* %UID% */
                if (strncmp(param_ptr + 2, "D%", 2) == 0) {
                    param_ptr += 4;
                    ret = snprintf(ptr - 1, size, "%d", pwd->pw_uid);
                    if (ret < 0) {
                        return NULL;
                    }
                    while (*(ptr++));
                    ptr--;
                }
                break;
            }
            break;
        case 'H':
            /* %HOME% */
            if (strncmp(param_ptr + 1, "OME%", 4) == 0) {
                param_ptr += 5;
                ret = snprintf(ptr - 1, size, "/home/%s", pwd->pw_name);
                if (ret < 0) {
                    return NULL;
                }
                while (*(ptr++));
                ptr--;
            }
            break;
        case 'R':
            /* %RUN% */
            if (strncmp(param_ptr + 1, "UN%", 3) == 0) {
                param_ptr += 4;
                ret = snprintf(ptr - 1, size, "/run/user/%d", pwd->pw_uid);
                if (ret < 0) {
                    return NULL;
                }
                while (*(ptr++));
                ptr--;
            }
            break;
        case '%':
            /* %% */
            param_ptr++;
        }
    }
    strncpy(ptr, param_ptr, path + sizeof(path) - ptr);

    return strdup(path);
}

static inline int mount_entry(pam_handle_t* pamh, struct mntent* mntent)
{
    char* mntdata, *fsname, *target;
    struct passwd* pwd;
    const char* user;
    unsigned long mntflags;
    int ret = 0;
    int optional = hasmntopt(mntent, "optional") != NULL;


    if (pam_get_item(pamh, PAM_USER, (const void**)&user) != PAM_SUCCESS) {
        pam_syslog(pamh, LOG_ERR, "failed to get user\n");
        return -1;
    }

    pwd = getpwnam(user);
    if (pwd == NULL) {
        pam_syslog(pamh, LOG_ERR, "Failed to get user infomation\n");
        return -1;
    }

    fsname = convert_param(pwd, mntent->mnt_fsname);
    if (fsname == NULL) {
        pam_syslog(pamh, LOG_ERR, "Failed to convert mount path\n");
        return -1;
    }

    target = convert_param(pwd, mntent->mnt_dir);
    if (target == NULL) {
        pam_syslog(pamh, LOG_ERR, "Failed to convert mount target path\n");
        free(fsname);
        return -1;
    }

    if (preparse_create_mntopts(target, mntent) < 0) {
        pam_syslog(pamh, LOG_ERR, "Failed to create mount target '%s'\n", target);
        ret = -1;
        goto free_param;
    }

    cleanup_preparsed_mntopts(mntent);

    if (parse_mntopts(mntent->mnt_opts, &mntflags, &mntdata) < 0) {
        pam_syslog(pamh, LOG_ERR, "Failed to parse mount options\n");
        ret = -1;
        goto free_param;
    }

    if (mount(fsname, target, mntent->mnt_type, mntflags & ~MS_REMOUNT, mntdata)) {
        if (optional) {
            pam_syslog(pamh, LOG_ERR, "failed to mount '%s' on '%s' (optional): %s\n", fsname, target, strerror(errno));
            goto free_mntdata;
        } else {
            pam_syslog(pamh, LOG_ERR, "failed to mount '%s' on '%s'\n", fsname, target);
            ret = -1;
            goto free_mntdata;
        }
    }

    if ((mntflags & MS_REMOUNT) || (mntflags & MS_BIND)) {
        pam_syslog(pamh, LOG_INFO, "remounting %s on %s to respect bind or remount options\n", fsname ? fsname : "(none)", target ? target : "(none)");

        if (mount(fsname, target,  mntent->mnt_type, mntflags | MS_REMOUNT, mntdata)) {
            if (optional) {
                pam_syslog(pamh, LOG_ERR, "failed to mount '%s' on '%s' (optional): %s\n", fsname, target, strerror(errno));
            } else {
                pam_syslog(pamh, LOG_ERR, "failed to mount '%s' on '%s'\n", fsname, target);
                ret = -1;
            }
        }
    }

free_mntdata:
    free(mntdata);

free_param:
    free(fsname);
    free(target);

    return ret;
}

int mount_fstab(pam_handle_t* pamh, const char* fstab)
{
    struct mntent mntent;
    char buf[4096];
    int ret = -1;

    FILE* fp = setmntent(fstab, "r");
    if (fp == NULL) {
        pam_syslog(pamh, LOG_ERR, "failed to read %s\n", fstab);
        return -1;
    }
    while (getmntent_r(fp, &mntent, buf, sizeof(buf))) {
        if (mount_entry(pamh, &mntent)) {
            goto out;
        }
    }

    ret = 0;
out:
    endmntent(fp);
    return ret;
}

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
 * @brief   CLI tool to attach to existing namespace by pam-attach.so
 */

#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/param.h>

#include <security/pam_appl.h>

extern char** environ;

static pam_handle_t* pamh;
static struct pam_conv pamc;

static void usage(char* name)
{
    printf("Usage: %s --user=USER -- COMMAND \n"
           "Run a program in the zone\n"
           "\n"
           "Options :\n"
           "   -u, --user=USER   USER for user name to attach\n"
           "   -l, --list        show all of named namespaces\n"
           "   -h, --help        show this\n"
           "\n", name);
}

static inline int show_list()
{
    DIR* dp;
    struct dirent* d;

    if ((dp = opendir(PID_FILE_PATH)) == NULL) {
        return 0;
    }

    while ((d = readdir(dp)) != NULL) {
        if (d->d_type == DT_REG) {
            printf("%s\n", d->d_name);
        }
    }

    if (closedir(dp) < 0) {
        fprintf(stderr, "Unable to close dir" PID_FILE_PATH);
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    char hostname[MAXHOSTNAMELEN];
    const char* user = NULL, *tty;
    char** args, **pam_envlist, **pam_env;
    struct passwd* pwd;
    int opt = 0, index, pam_err, status;
    pid_t pid;

    struct option options[] = {
        {"user", required_argument, 0, 0},
        {"list", no_argument, 0, 0},
        {"help", no_argument, 0, 0},
        {0, 0, 0, 0}
    };

    if (argc <= 1) {
        usage(argv[0]);
        return 0;
    }

    while (opt != -1) {
        opt = getopt_long(argc, argv, "u:lh", options, &index);
        switch (opt) {
        case 0:
            switch (index) {
            case 0:
                user = optarg;
                break;
            case 1:
                return show_list();
            case 2:
                usage(argv[0]);
                return 0;
            }
        case 'u':
            user = optarg;
            break;
        case 'l':
            return show_list();
        case 'h':
            usage(argv[0]);
            return 0;
        }
    }

    if (user == NULL) {
        fprintf(stderr, "missing user name, use --user option\n");
        return -1;
    }

    pwd = getpwnam(user);

    if (pwd == NULL) {
        fprintf(stderr, "user '%s' doesn't exist\n", user);
        return -1;
    }

    if (optind >= argc) {
        args = malloc(sizeof(char*) * 2);
        if (args == NULL) {
            fprintf(stderr, "failed to allocate memory\n");
            return -1;
        }
        args[0] = pwd->pw_shell;
        args[1] = NULL;
    } else {
        args = argv + optind;
    }

    pam_start("attach", user, &pamc, &pamh);

    gethostname(hostname, sizeof(hostname));
    if ((pam_err = pam_set_item(pamh, PAM_RHOST, hostname)) != PAM_SUCCESS) {
        fprintf(stderr, "failed to set PAM_RHOST\n");
        goto err;
    }

    user = getlogin();
    if ((pam_err = pam_set_item(pamh, PAM_RUSER, user)) != PAM_SUCCESS) {
        fprintf(stderr, "failed to set PAM_RUSER\n");
        goto err;
    }

    tty = ttyname(STDERR_FILENO);
    if ((pam_err = pam_set_item(pamh, PAM_TTY, tty)) != PAM_SUCCESS) {
        fprintf(stderr, "failed to set PAM_TTY\n");
        goto err;
    }

    if ((pam_err = pam_acct_mgmt(pamh, 0)) == PAM_NEW_AUTHTOK_REQD) {
        pam_err = pam_chauthtok(pamh, PAM_CHANGE_EXPIRED_AUTHTOK);
    }

    if (pam_err != PAM_SUCCESS) {
        goto err;
    }

    if ((pam_err = pam_open_session(pamh, 0)) != PAM_SUCCESS) {
        fprintf(stderr, "failed to set open session\n");
        goto err;
    }

    if ((pam_envlist = pam_getenvlist(pamh)) != NULL) {
        for (pam_env = pam_envlist; *pam_env != NULL; ++pam_env) {
            putenv(*pam_env);
            free(*pam_env);
        }
        free(pam_envlist);
    }

    switch ((pid = fork())) {
    case -1:
        fprintf(stderr, "failed to fork()\n");
        goto err;
    case 0:
        if (initgroups(pwd->pw_name, pwd->pw_gid) == -1) {
            fprintf(stderr, "failed to initgroups()\n");
            return EXIT_FAILURE;
        }

        if (setgid(pwd->pw_gid) == -1) {
            fprintf(stderr, "failed to setgid()\n");
            return EXIT_FAILURE;
        }

        if (setuid(pwd->pw_uid) == -1) {
            fprintf(stderr, "failed to setuid()\n");
            return EXIT_FAILURE;
        }

        execve(*args, args, environ);
        fprintf(stderr, "failed to execute %s\n", args[0]);

        return EXIT_FAILURE;
    default:
        waitpid(pid, &status, 0);

        pam_err = pam_close_session(pamh, 0);
        pam_end(pamh, pam_err);

        return  WEXITSTATUS(status);
    }

err:
    pam_end(pamh, pam_err);
    return EXIT_FAILURE;
}

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

#include <string>
#include <vector>
#include <iostream>

#include <grp.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <syslog.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/param.h>

#include "pam.h"
#include "exception.h"
#include "filesystem.h"
#include "auth/user.h"

#define DEFAULT_SHELL "/bin/bash"

extern char** environ;

static inline void usage(const std::string name)
{
    std::cout << "Usage: " << name << " --user=USER -- COMMAND" << std::endl
              << "Run a program in the zone" << std::endl
              << std::endl
              << "Options :" << std::endl
              << "   -u, --user=USER   USER for user name to attach" << std::endl
              << "   -l, --list        show all of named namespaces" << std::endl
              << "   -h, --help        show this" << std::endl
              << std::endl;
}

static inline int show_list()
{
    runtime::DirectoryIterator iter(runtime::Path(PID_FILE_PATH)), end;

    while (iter != end) {
        const std::string& path = (*iter).getPath();
        size_t name = path.rfind('/') + 1;
        std::cout << path.substr(name) << std::endl;
        ++iter;
    }

    return 0;
}

static inline int attach_program(runtime::User& user, char** args)
{
    runtime::PAM pam_app("nsattach", user.getName());

    try {
        char hostname[MAXHOSTNAMELEN];
        gethostname(hostname, sizeof(hostname));
        pam_app.setItem(PAM_RHOST, hostname);
        pam_app.setItem(PAM_RUSER, getlogin());
        pam_app.setItem(PAM_TTY, ttyname(STDERR_FILENO));
    } catch (runtime::Exception& e) {
        std::cerr << "failed to set PAM items" << std::endl;
        return EXIT_FAILURE;
    }

    int ret = pam_app.accountManagement(0);
    if (ret == PAM_NEW_AUTHTOK_REQD) {
        ret = pam_app.changeAuthenticationToken(PAM_CHANGE_EXPIRED_AUTHTOK);
        if (ret != PAM_SUCCESS) {
            std::cerr << "failed to change authentication token" << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        std::cerr << "failed to validate account" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        pam_app.openSession(0);
    } catch (runtime::Exception& e) {
        std::cerr << "failed to set open session" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        std::vector<std::string> envlist = pam_app.getEnvList();
        for (std::vector<std::string>::iterator it = envlist.begin();
                it != envlist.end(); it++) {
            putenv(strdup((*it).c_str()));
        }
    } catch (runtime::Exception& e) {
        std::cerr << "failed to set environment variables" << std::endl;
    }

    int status;
    pid_t pid;

    switch ((pid = fork())) {
    case -1:
        std::cerr << "failed to fork()" << std::endl;
        return EXIT_FAILURE;
    case 0:
        if (initgroups(user.getName().c_str(), user.getGid()) == -1) {
            std::cerr << "failed to initgroups()" << std::endl;
            return EXIT_FAILURE;
        }

        if (setgid(user.getGid()) == -1) {
            std::cerr << "failed to setgid()" << std::endl;
            return EXIT_FAILURE;
        }

        if (setuid(user.getUid()) == -1) {
            std::cerr << "failed to setuid()" << std::endl;
            return EXIT_FAILURE;
        }

        char *default_args[2];
        default_args[0] = strdup(DEFAULT_SHELL);
        default_args[1] = NULL;

        if (args == NULL) {
            args = default_args;
        }

        execve(*args, args, environ);
        std::cerr << "failed to execute " << args[0] << std::endl;

        free(default_args[0]);

        return EXIT_FAILURE;
    }

    waitpid(pid, &status, 0);

    try {
        pam_app.closeSession(0);
    } catch (runtime::Exception& e) {
        std::cerr << "failed to set close session" << std::endl;
        return EXIT_FAILURE;
    }

    return WEXITSTATUS(status);
}

int main(int argc, char* argv[])
{
    std::string username;
    int opt = 0, index;
    char** args;

    struct option options[] = {
        {"user", required_argument, 0, 0},
        {"list", no_argument, 0, 0},
        {"help", no_argument, 0, 0},
        {0, 0, 0, 0}
    };

    if (argc <= 1) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    while (opt != -1) {
        opt = getopt_long(argc, argv, "u:lh", options, &index);
        switch (opt) {
        case 0:
            switch (index) {
            case 0:
                username = optarg;
                break;
            case 1:
                return show_list();
            case 2:
                usage(argv[0]);
                return 0;
            }
            break;
        case 'u':
            username = optarg;
            break;
        case 'l':
            return show_list();
        case 'h':
            usage(argv[0]);
            return EXIT_SUCCESS;
        }
    }

    if (username.size() == 0) {
        std::cerr << "missing user name, use --user option" << std::endl;
        return EXIT_FAILURE;
    }

    if (optind >= argc) {
        args = NULL;
    } else {
        args = argv + optind;
    }

    try {
        runtime::User user(username);
        return attach_program(user, args);
    } catch (runtime::Exception& e) {
        std::cerr << "user '" << username << "' doesn't exist" << std::endl;
    }

    return EXIT_FAILURE;
}

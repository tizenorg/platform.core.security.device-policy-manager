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
#define _XOPEN_SOURCE
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

#include <grp.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>

#include <string>
#include <vector>
#include <iostream>
#include <utility>

#include "../session.h"

#include "error.h"
#include "exception.h"
#include "filesystem.h"
#include "auth/user.h"

#define DEFAULT_SHELL "/bin/bash"

typedef std::pair<std::string, int> NamespacePair;

std::vector<NamespacePair> namespaces = {
    {"mnt",  CLONE_NEWNS},
    {"net",  CLONE_NEWNET},
    {"uts",  CLONE_NEWUTS},
    {"user", CLONE_NEWUSER},
    {"ipc",  CLONE_NEWIPC},
    {"pid",  CLONE_NEWPID}
};

extern char** environ;

static inline void usage(const std::string name)
{
    std::cout << "Usage: " << name << " [Option] -- COMMAND" << std::endl
              << "Run a program in the zone" << std::endl
              << std::endl
              << "Options :" << std::endl
              << "   -a, --attach=zone execute command in the zone" << std::endl
              << "   -l, --list        show all zone instances" << std::endl
              << "   -h, --help        show this" << std::endl
              << std::endl;
}

int showZoneInstances()
{
    runtime::DirectoryIterator iter(runtime::Path("/var/run/zone")), end;

    while (iter != end) {
        const std::string& path = (*iter).getPath();
        size_t name = path.rfind('/') + 1;
        std::cout << path.substr(name) << std::endl;
        ++iter;
    }

    return 0;
}

void attachNamespaces(pid_t pid)
{
    int fd;
    for (const NamespacePair& ns : namespaces) {
        std::string nspath = "/proc/" + std::to_string(pid) + "/ns/" + ns.first;

        do {
            fd = ::open(nspath.c_str(), O_RDONLY);
        } while (fd == -1 && errno == EINTR);

        if (fd == -1) {
            if (errno != ENOENT) {
                throw runtime::Exception("Failed to open namesapce: " + nspath);
            }
        } else {
            if (::setns(fd, ns.second)) {
                ::close(fd);
                throw runtime::Exception("Failed to attach namespace: " + nspath);
            }
            ::close(fd);
        }
    }
}

void launchProcess(runtime::User& user, char** args)
{
    int status;

    pid_t pid = ::fork();
    if (pid == -1) {
        throw runtime::Exception("Failed to create child process");
    }

    if (pid != 0) {
        ::waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != EXIT_SUCCESS) {
            throw runtime::Exception("Child process terminated abnormally");
        }
        return;
    }

    if (::initgroups(user.getName().c_str(), user.getGid()) == -1) {
        std::cerr << "Failed to initgroups()" << std::endl;
        ::exit(EXIT_FAILURE);
    }

    if (::setgid(user.getGid()) == -1) {
        std::cerr << "Failed to setgid()" << std::endl;
        ::exit(EXIT_FAILURE);
    }

    if (::setuid(user.getUid()) == -1) {
        std::cerr << "Failed to setuid()" << std::endl;
        ::exit(EXIT_FAILURE);
    }

    char *default_args[2];
    default_args[0] = ::strdup(DEFAULT_SHELL);
    default_args[1] = NULL;

    if (args == NULL) {
        args = default_args;
    }

    ::execve(*args, args, environ);
    std::cerr << "Failed to execute " << args[0] << std::endl;

    ::free(default_args[0]);

    ::exit(EXIT_FAILURE);
}

int attachToZone(const std::string& name, char* args[])
{
    try {
        runtime::User user(name);

        attachNamespaces(getSessionLeader(user));
        launchProcess(user, args);
    } catch (runtime::Exception& e) {
        std::cerr << e.what()  << std::endl;
        return -1;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    int opt = 0, index, ret = 0;

    struct option options[] = {
        {"attach", required_argument, 0, 'a'},
        {"list", no_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    if (argc <= 1) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    while ((opt = getopt_long(argc, argv, "a:lh", options, &index)) != -1) {
        switch (opt) {
        case 'a':
            ret = attachToZone(optarg, optind >= argc ? NULL : argv + optind);
            break;
        case 'l':
            ret = showZoneInstances();
            break;
        case 'h':
            usage(argv[0]);
            break;
        default:
            usage(argv[0]);
            ret = -1;
        }
    }

    return ret == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

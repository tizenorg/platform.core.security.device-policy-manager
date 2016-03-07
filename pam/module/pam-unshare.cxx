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
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <security/pam_modules.h>

#include "mount.hxx"
#include "pam-impl.hxx"
#include "pam-module.hxx"
#include "exception.hxx"
#include "parameter.hxx"
#include "filesystem.hxx"
#include "xml/parser.hxx"
#include "xml/document.hxx"
#include "shadow/user.hxx"

#define CONTAINER_MANIFEST_DIR  "/etc/dpm/container/"

namespace Runtime {

template class PAM<PAMUnshare>;

void PAMUnshare::attachNamespace(pid_t pid, int flags)
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

void PAMUnshare::_authenticate(int flags, std::vector<std::string> args)
{

}

void PAMUnshare::_setCredential(int flags, std::vector<std::string> args)
{

}

void PAMUnshare::_accountManagement(int flags, std::vector<std::string> args)
{

}

void PAMUnshare::_openSession(int flags, std::vector<std::string> args)
{
    std::unique_ptr<Xml::Document> bundle;
    Xml::Node::NodeList nodes;
    int unshare_flags = 0;
    std::string dest, src, type, opts, create;
    bool optional;
    std::string user;
    pid_t pid;

    try {
        user = static_cast<const char*>(getItem(PAM_USER));
    } catch (Runtime::Exception& e) {
        throw Runtime::Exception("failed to get user", PAM_SESSION_ERR);
    }

    if (user.size() == 0) {
        throw Runtime::Exception("no user is specified", PAM_SESSION_ERR);
    }

    if (user == "root") {
        return;
    }

    ::umask(077);

    ::mkdir(PID_FILE_PATH, 0700);

    try {
        bundle = std::unique_ptr<Xml::Document>(
                     Xml::Parser::parseFile(CONTAINER_MANIFEST_DIR + user + ".xml"));
    } catch (Runtime::Exception& e) {
        bundle = std::unique_ptr<Xml::Document>(
                     Xml::Parser::parseFile(CONTAINER_MANIFEST_DIR "owner.xml"));
    }

    nodes = bundle->evaluate("//bundle-manifest/namespaces");
    for (Xml::Node::NodeList::iterator it = nodes.begin();
            it != nodes.end(); it++) {
        Xml::Node::NodeList children = it->getChildren();
        for (Xml::Node::NodeList::iterator it = children.begin();
                it != children.end(); it++) {
            if (it->getName() == "mount") {
                unshare_flags |= CLONE_NEWNS;
            }
            if (it->getName() == "network") {
                unshare_flags |= CLONE_NEWNET;
            }
            if (it->getName() == "uts") {
                unshare_flags |= CLONE_NEWUTS;
            }
            if (it->getName() == "ipc") {
                unshare_flags |= CLONE_NEWIPC;
            }
#ifdef CLONE_NEWUSER
            if (it->getName() == "user") {
                unshare_flags |= CLONE_NEWUSER;
            }
#endif
#ifdef CLONE_NEWCGROUP
            if (it->getName() == "cgroup") {
                unshare_flags |= CLONE_NEWCGROUP;
            }
#endif
        }
    }

    std::unique_ptr<sem_t, void(*)(sem_t*)> sem(
        ::sem_open("PAMUnshare", O_CREAT, 0700, 1),
    [](sem_t * sem) {
        if (sem == NULL) {
            return;
        }
        ::sem_post(sem);
        ::sem_close(sem);
        ::sem_unlink("PAMUnshare");
    });

    if (sem == NULL) {
        throw Runtime::Exception("failed to open semaphore", PAM_SESSION_ERR);
    }

    ::sem_wait(sem.get());

    std::fstream pidfile(PID_FILE_PATH "/" + user, std::ios::app);
    if (!pidfile.is_open()) {
        throw Runtime::Exception("failed to open PID file", PAM_SESSION_ERR);
    }

    if (!(pidfile >> pid).good()) {
        if (::unshare(unshare_flags) != 0) {
            throw Runtime::Exception("failed to unshare namespace", PAM_SESSION_ERR);
        }

        if (unshare_flags & CLONE_NEWNS) {
            if (::mount(NULL, "/", NULL, MS_SLAVE | MS_REC, NULL) < 0) {
                throw Runtime::Exception("failed to remount /", PAM_SESSION_ERR);
            }

            ::umask(022);

            nodes = bundle->evaluate("//bundle-manifest/mounts/mount");
            for (Xml::Node::NodeList::iterator it = nodes.begin();
                    it != nodes.end(); it++) {
                optional  = false;
                Xml::Node::NodeList children = it->getChildren();
                for (Xml::Node::NodeList::iterator it = children.begin();
                        it != children.end(); it++) {
                    if (it->getName() == "destination") {
                        dest = it->getChildren().begin()->getContent();
                        Runtime::Parameter::parse(dest, user);
                        create = it->getProp("create");
                        if (create == "file") {
                            Runtime::File destdir(dest.substr(0, dest.rfind('/')));
                            destdir.makeDirectory(true);
                            std::ofstream file(dest);
                            file.close();
                        } else if (create == "dir") {
                            Runtime::File destdir(dest);
                            destdir.makeDirectory(true);
                        }
                    } else if (it->getName() == "source") {
                        src = it->getChildren().begin()->getContent();
                        Runtime::Parameter::parse(src, user);
                        create = it->getProp("create");
                        if (create == "file") {
                            Runtime::File destdir(dest.substr(0, dest.rfind('/')));
                            destdir.makeDirectory(true);
                            std::ofstream file(dest);
                            file.close();
                        } else if (create == "dir") {
                            Runtime::File destdir(dest);
                            destdir.makeDirectory(true);
                        }
                    } else if (it->getName() == "type") {
                        type = it->getChildren().begin()->getContent();
                    } else if (it->getName() == "option") {
                        opts = it->getChildren().begin()->getContent();
                    } else if (it->getName() == "optional") {
                        optional = true;
                    }
                }

                Mount::mountEntry(src, dest, type, opts, optional);
            }
        }
        pidfile.clear();
    } else {
        attachNamespace(pid, unshare_flags);
    }
    pidfile <<::getpid() << std::endl;
}

void PAMUnshare::_closeSession(int flags, std::vector<std::string> args)
{
    std::string user, path, tmppath;
    pid_t pid, current_pid;
    bool is_empty = true;

    try {
        user = static_cast<const char*>(getItem(PAM_USER));
    } catch (Runtime::Exception& e) {
        throw Runtime::Exception("failed to get user", PAM_SESSION_ERR);
    }

    if (user.size() == 0) {
        throw Runtime::Exception("no user is specified", PAM_SESSION_ERR);
    }

    current_pid = ::getpid();

    ::umask(077);

    std::unique_ptr<sem_t, void(*)(sem_t*)> sem(
        ::sem_open("PAMUnshare", O_CREAT, 0700, 1),
    [](sem_t * sem) {
        if (sem == NULL) {
            return;
        }
        ::sem_post(sem);
        ::sem_close(sem);
        ::sem_unlink("PAMUnshare");
    });

    if (sem == NULL) {
        throw Runtime::Exception("failed to open semaphore", PAM_SESSION_ERR);
    }

    ::sem_wait(sem.get());

    path = PID_FILE_PATH "/" + user;
    std::ifstream pidfile(path);
    if (!pidfile.is_open()) {
        throw Runtime::Exception("failed to open PID file", PAM_SESSION_ERR);
    }

    tmppath = path + ":";
    std::ofstream tmppidfile(tmppath);
    if (!tmppidfile.is_open())
        throw Runtime::Exception("failed to open temporary PID file",
                                 PAM_SESSION_ERR);

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
        throw Runtime::Exception("failed to renew PID file", PAM_SESSION_ERR);
    }

    if (is_empty) {
        if (::unlink(path.c_str()) != 0)
            throw Runtime::Exception("failed to remove empty PID file",
                                     PAM_SESSION_ERR);
    }
}

void PAMUnshare::_changeAuthenticationToken(int flags, std::vector<std::string> args)
{

}

PAMUnshare::PAMUnshare(pam_handle_t* pamh)
    : PAM(pamh)
{

}

PAMUnshare::~PAMUnshare()
{

}

} // namespace Runtime

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

#ifndef __SOCKET__
#define __SOCKET__

#include "exception.hxx"

namespace Ipc {

class SocketException: public Runtime::Exception {
public:
    SocketException(const std::string& msg) : Runtime::Exception(msg) {}
};

struct Credentials {
    pid_t pid;
    uid_t uid;
    gid_t gid;
};

class Socket {
public:
    explicit Socket(int socketFd = -1);
    Socket(Socket&& socket) noexcept;
    ~Socket() noexcept;

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&) = delete;

    Socket accept();
    int getFd() const;
    Credentials getPeerCredentials() const;

    void write(const void* buffer, const size_t size) const;
    void read(void* buffer, const size_t size) const;

    static Socket create(const std::string& path);
    static Socket connect(const std::string& path);

private:
    static int createRegularSocket(const std::string& path);

#ifdef USE_SYSTEMD_SOCKET_ACTIVATION
    static int createSystemdSocket(const std::string& path);
#endif

private:
    int socketFd;
};

} // namespace Ipc
#endif //__SOCKET__

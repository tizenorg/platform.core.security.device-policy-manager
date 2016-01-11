/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "error.hxx"
#include "socket.hxx"

namespace Ipc {

namespace {

const int MAX_BACKLOG_SIZE = 100;

void setCloseOnExec(int fd)
{
    if (::fcntl(fd, F_SETFD, FD_CLOEXEC) == -1) {
        throw SocketException(Runtime::GetSystemErrorMessage());
    }
}

Credentials getCredentials(int fd)
{
    struct ucred cred;
    socklen_t credsz = sizeof(cred);
    if (::getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &cred, &credsz)) {
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    return {cred.pid, cred.uid, cred.gid};
}

} // namespace

Socket::Socket(int fd)
    : socketFd(fd)
{
}

Socket::Socket(Socket&& socket) noexcept
    : socketFd(socket.socketFd)
{
    socket.socketFd = -1;
}

Socket::~Socket() noexcept
{
    if (socketFd != -1) {
        ::close(socketFd);
    }
}

Socket Socket::accept()
{
    int sockfd = ::accept(socketFd, nullptr, nullptr);
    if (sockfd == -1) {
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    setCloseOnExec(sockfd);

    return Socket(sockfd);
}

int Socket::getFd() const
{
    return socketFd;
}

Credentials Socket::getPeerCredentials() const
{
    return getCredentials(socketFd);
}

void Socket::read(void *buffer, const size_t size) const
{
    size_t total = 0;

    while (total < size) {
        int bytes = ::read(socketFd, reinterpret_cast<char*>(buffer) + total, size - total);
        if (bytes >= 0) {
            total += bytes;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            continue;
        } else {
            throw SocketException(Runtime::GetSystemErrorMessage());
        }
    }
}

void Socket::write(const void *buffer, const size_t size) const
{
    size_t written = 0;

    while (written < size) {
        int bytes = ::write(socketFd, reinterpret_cast<const char*>(buffer) + written, size - written);
        if (bytes >= 0) {
            written += bytes;
        } else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            continue;
        } else {
            throw SocketException(Runtime::GetSystemErrorMessage());
        }
    }
}

#ifdef USE_SYSTEMD_SOCKET_ACTIVATION
int Socket::createSystemdSocket(const std::string& path)
{
    int n = ::sd_listen_fds(-1);
    if (n < 0) {
        throw SocketException("sd_listen_fds faield");
    }

    for (int fd = SD_LISTEN_FDS_START; fd < SD_LISTEN_FDS_START + n; ++fd) {
        if (::sd_is_socket_unix(fd, SOCK_STREAM, 1, path.c_str(), 0) > 0) {
            setCloseOnExec(fd);
            return fd;
        }
    }

    return -1;
}
#endif

int Socket::createRegularSocket(const std::string& path)
{
    if (path.size() >= sizeof(sockaddr_un::sun_path)) {
        throw SocketException(Runtime::GetSystemErrorMessage(ENAMETOOLONG));
    }

    int sockfd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    setCloseOnExec(sockfd);

    ::sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    ::strncpy(addr.sun_path, path.c_str(), sizeof(sockaddr_un::sun_path));

    if (addr.sun_path[0] == '@') {
        addr.sun_path[0] = '\0';
    } else {
        unlink(path.c_str());
    }

    if (::bind(sockfd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(struct sockaddr_un)) == -1) {
        ::close(sockfd);
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    int optval = 1;
    if (::setsockopt(sockfd, SOL_SOCKET, SO_PASSCRED, &optval, sizeof(optval)) == -1) {
        ::close(sockfd);
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    if (::listen(sockfd, MAX_BACKLOG_SIZE) == -1) {
        ::close(sockfd);
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    return sockfd;
}

Socket Socket::create(const std::string& path)
{
    int fd;

#ifdef USE_SYSTEMD_SOCKET_ACTIVATION
    fd = createSystemdSocket(path);
    if (fd == -1) {
        fd = createRegularSocket(path);
    }
#else
    fd = createRegularSocket(path);
#endif

    return Socket(fd);
}

Socket Socket::connect(const std::string& path)
{
    if (path.size() >= sizeof(sockaddr_un::sun_path)) {
        throw SocketException(Runtime::GetSystemErrorMessage(ENAMETOOLONG));
    }

    int fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) {
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    setCloseOnExec(fd);

    sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    ::strncpy(addr.sun_path, path.c_str(), sizeof(sockaddr_un::sun_path));

    if (addr.sun_path[0] == '@') {
        addr.sun_path[0] = '\0';
    }

    if (::connect(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(struct sockaddr_un)) == -1) {
        ::close(fd);
        throw SocketException(Runtime::GetSystemErrorMessage());
    }

    return Socket(fd);
}

} // namespace Ipc

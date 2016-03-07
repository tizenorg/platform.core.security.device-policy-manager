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

#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>

#include "error.hxx"
#include "exception.hxx"
#include "mainloop.hxx"

#define MAX_EPOLL_EVENTS	16

namespace Ipc {

Mainloop::Mainloop()
    : pollFd(::epoll_create1(EPOLL_CLOEXEC)), stopped(false)
{
    auto wakeupMainloop = [this](int fd, Mainloop::Event event) {
        wakeupSignal.receive();
    };

    if (pollFd == -1) {
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    addEventSource(wakeupSignal.getFd(), EPOLLIN, wakeupMainloop);
}

Mainloop::~Mainloop()
{
    if (!callbacks.empty()) {
        assert(0 && "callback list is not empty");
    }

    ::close(pollFd);
}

void Mainloop::addEventSource(const int fd, const Event events, Callback&& callback)
{
    epoll_event event;
    std::lock_guard<Mutex> lock(mutex);

    if (callbacks.find(fd) != callbacks.end()) {
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    ::memset(&event, 0, sizeof(epoll_event));

    event.events = events;
    event.data.fd = fd;

    if (::epoll_ctl(pollFd, EPOLL_CTL_ADD, fd, &event) == -1) {
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    callbacks.insert({fd, std::make_shared<Callback>(std::move(callback))});
}

void Mainloop::removeEventSource(const int fd)
{
    std::lock_guard<Mutex> lock(mutex);

    auto iter = callbacks.find(fd);
    if (iter == callbacks.end()) {
        return;
    }

    callbacks.erase(iter);

    ::epoll_ctl(pollFd, EPOLL_CTL_DEL, fd, NULL);
}

bool Mainloop::dispatch(const int timeout)
{
    int nfds;
    epoll_event event[MAX_EPOLL_EVENTS];

    do {
        nfds = ::epoll_wait(pollFd, event, MAX_EPOLL_EVENTS, timeout);
    } while ((nfds == -1) && (errno == EINTR));

    if (nfds < 0) {
        throw Runtime::Exception(Runtime::GetSystemErrorMessage());
    }

    for (int i = 0; i < nfds; i++) {
        std::lock_guard<Mutex> lock(mutex);

        auto iter = callbacks.find(event[i].data.fd);
        if (iter == callbacks.end()) {
            continue;
        }

        std::shared_ptr<Callback> callback(iter->second);
        try {
            if ((event[i].events & (EPOLLHUP | EPOLLRDHUP))) {
                event[i].events &= ~EPOLLIN;
            }

            (*callback)(event[i].data.fd, event[i].events);
        } catch (std::exception& e) {
            std::cout << "EXCEPTION ON MAINLOOP" << std::endl;
        }
    }

    return true;
}

void Mainloop::stop()
{
    stopped = true;
    wakeupSignal.send();
}

void Mainloop::run()
{
    while (!stopped) {
        dispatch(-1);
    }
}

} // namespace Ipc

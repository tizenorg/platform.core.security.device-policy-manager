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

#ifndef __MAINLOOP__
#define __MAINLOOP__

#include <sys/epoll.h>

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <atomic>

#include "eventfd.hxx"

namespace Ipc {

class Mainloop {
public:
    typedef unsigned int Event;
    typedef std::function<void(int fd, Event event)> Callback;

    Mainloop();
    ~Mainloop();

    void addEventSource(const int fd, const Event events, Callback&& callback);
    void removeEventSource(const int fd);
    bool dispatch(const int timeout);
    void run();
    void stop();

private:
    typedef std::recursive_mutex Mutex;

    std::unordered_map<int, std::shared_ptr<Callback>> callbacks;
    Mutex mutex;
    int pollFd;
    std::atomic<bool> stopped;
    Runtime::EventFD wakeupSignal;
};
} // namespace Ipc
#endif //__MAINLOOP__

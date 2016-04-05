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

#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

#include "exception.h"
#include "service.h"
#include "message.h"

namespace rmi {

thread_local Service::ProcessingContext Service::processingContext;

Service::Service(const std::string& path) :
    address(path),
    workqueue(5)
{
    setNewConnectionCallback(nullptr);
    setCloseConnectionCallback(nullptr);
}

Service::~Service()
{
}

void Service::start()
{
    Socket socket(Socket::create(address));

    auto accept = [&](int fd, runtime::Mainloop::Event event) {
        onNewConnection(std::make_shared<Connection>(socket.accept()));
    };

    mainloop.addEventSource(socket.getFd(),
                            EPOLLIN | EPOLLHUP | EPOLLRDHUP,
                            accept);
    mainloop.run();
}

void Service::stop()
{
}

Service::ConnectionRegistry::iterator Service::getConnectionIterator(const int id)
{
    std::lock_guard<std::mutex> lock(stateLock);

    return std::find_if(connectionRegistry.begin(), connectionRegistry.end(),
                        [id](const std::shared_ptr<Connection>& connection) {
        return id == connection->getFd();
    });
}

void Service::setNewConnectionCallback(const ConnectionCallback& connectionCallback)
{
    auto callback = [connectionCallback, this](const std::shared_ptr<Connection>& connection) {
        auto handle = [&](int fd, runtime::Mainloop::Event event) {
            auto iter = getConnectionIterator(fd);
            if (iter == connectionRegistry.end()) {
                return;
            }

            if ((event & EPOLLHUP) || (event & EPOLLRDHUP)) {
                onCloseConnection(*iter);
                connectionRegistry.erase(iter);
                return;
            }

            onMessageProcess(*iter);
        };

        if ((connectionCallback == nullptr) ||
            (connectionCallback(*connection) == true)) {
            mainloop.addEventSource(connection->getFd(),
                                    EPOLLIN | EPOLLHUP | EPOLLRDHUP,
                                    handle);
            connectionRegistry.push_back(connection);
        }
    };

    std::lock_guard<std::mutex> lock(stateLock);
    onNewConnection = std::move(callback);
}

void Service::setCloseConnectionCallback(const ConnectionCallback& closeCallback)
{
    auto callback = [closeCallback, this](const std::shared_ptr<Connection>& connection) {
        mainloop.removeEventSource(connection->getFd());
        if (closeCallback) {
            closeCallback(*connection);
        }
    };

    std::lock_guard<std::mutex> lock(stateLock);
    onCloseConnection = std::move(callback);
}

void Service::createNotification(const std::string& name)
{
    std::lock_guard<std::mutex> lock(notificationLock);

    if (notificationRegistry.count(name)) {
        throw runtime::Exception("Notification already registered");
    }

    notificationRegistry.emplace(name, name);
}

int Service::subscribeNotification(const std::string& name)
{
    auto closeHandler = [&, name](int fd, runtime::Mainloop::Event event) {
        if ((event & EPOLLHUP) || (event & EPOLLRDHUP)) {
            unsubscribeNotification(name, fd);
            return;
        }
    };

    notificationLock.lock();
    if (!notificationRegistry.count(name)) {
        notificationLock.unlock();
        return -1;
    }

    Notification& notification = notificationRegistry[name];
    notificationLock.unlock();

    try {
        SubscriptionId slot = notification.createSubscriber();
        mainloop.addEventSource(slot.first, EPOLLHUP | EPOLLRDHUP, closeHandler);
        return slot.second;
    } catch (runtime::Exception& e) {
        return -1;
    }

    return -1;
}

int Service::unsubscribeNotification(const std::string& name, const int id)
{
    notificationLock.lock();

    if (!notificationRegistry.count(name)) {
        notificationLock.unlock();
        return -1;
    }

    Notification& notification = notificationRegistry[name];
    notificationLock.unlock();

    notification.removeSubscriber(id);

    mainloop.removeEventSource(id);

    return 0;
}

void Service::onMessageProcess(const std::shared_ptr<Connection>& connection)
{
    auto process = [&](Message& request) {
        try {
            //stateLock.lock();
            std::shared_ptr<MethodDispatcher> methodDispatcher = methodRegistry.at(request.target());
            //stateLock.unlock();

            // [TBD] Request authentication before dispatching method handler.
            processingContext = ProcessingContext(connection);
            connection->send((*methodDispatcher)(request));
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
            connection->send(request.createErrorMessage());
        }
    };

    try {
        workqueue.submit(std::bind(process, connection->dispatch()));
    } catch (runtime::Exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void Service::addAsyncWork(std::function<void()>&& task)
{
    try {
        workqueue.submit(std::move(task));
    } catch (runtime::Exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

} // namespace rmi

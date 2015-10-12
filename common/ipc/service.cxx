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

#include <algorithm>

#include "exception.hxx"
#include "service.hxx"
#include "message.hxx"

namespace Ipc {

Service::Service(const std::string& path)
    : address(path)
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

    auto accept = [&](int fd, Mainloop::Event event) {
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
    return std::find_if(connectionRegistry.begin(), connectionRegistry.end(), [id](const Client& client) {
        return id == client->getFd();
    });
}

void Service::setNewConnectionCallback(const ConnectionCallback& connectionCallback)
{
    auto callback = [connectionCallback, this](const Client& connection) {
        auto handle = [&](int fd, Mainloop::Event event) {
            auto iter = getConnectionIterator(fd);
            if (iter == connectionRegistry.end()) {
                return;
            }

            if ((event & EPOLLHUP) || (event & EPOLLRDHUP)) {
                onCloseConnection(*iter);
                connectionRegistry.erase(iter);
            } else {
                onMessageProcess(*iter);
            }
        };

        if ((connectionCallback == nullptr) ||
                (connectionCallback(*connection) == true)) {
            mainloop.addEventSource(connection->getFd(),
                                    EPOLLIN | EPOLLHUP | EPOLLRDHUP,
                                    handle);
            connectionRegistry.push_back(connection);
        }
    };

    // [TBD] Lock
    onNewConnection = std::move(callback);
}

void Service::setCloseConnectionCallback(const ConnectionCallback& closeCallback)
{
    auto callback = [closeCallback, this](const Client& connection) {
        mainloop.removeEventSource(connection->getFd());
        if (closeCallback) {
            closeCallback(*connection);
        }
    };

    // [TBD] Lock
    onCloseConnection = std::move(callback);
}

void Service::onMessageProcess(const Client& connection)
{
    try {
        Message request = connection->dispatch();

        const std::string &target = request.target();
        if (methodRegistry.count(target)) {
            std::shared_ptr<MethodDispatcher> methodDispatcher = methodRegistry.at(target);
            // [TBD] Request authentication before dispatching method handler.
            connection->send((*methodDispatcher)(request));
        } else {
            connection->send(request.createErrorMessage());
        }
    } catch (Runtime::Exception& e) {
        Runtime::Exception(e.what());
    }
}

} // namespace Ipc

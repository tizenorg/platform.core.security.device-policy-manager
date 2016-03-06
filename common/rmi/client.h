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

#ifndef __RMI_CLIENT_H__
#define __RMI_CLIENT_H__

#include <thread>
#include <string>
#include <memory>

#include "error.h"
#include "object-latch.h"
#include "message.h"
#include "connection.h"
#include "mainloop.h"
#include "callback-holder.h"

namespace rmi {

class Client {
public:
    Client(const std::string& address);
    ~Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void connect();
    void disconnect();

    int subscribe(const std::string& name);

    template<typename... Args>
    void subscribe(const std::string& name,
                   const typename MethodHandler<void, Args...>::type& handler);

    template<typename Type, typename... Args>
    Type methodCall(const std::string& method, Args&&... args);

private:
    std::string address;
    std::shared_ptr<Connection> connection;
    runtime::Mainloop mainloop;
    std::thread dispatcher;
};

template<typename... Args>
void Client::subscribe(const std::string& name,
                       const typename MethodHandler<void, Args...>::type& handler)
{
    auto callback = [handler, this](int fd, runtime::Mainloop::Event event) {
        if ((event & EPOLLHUP) || (event & EPOLLRDHUP)) {
            mainloop.removeEventSource(fd);
            return;
        }

        try {
            Message msg;
            Socket transport(fd, false);
            msg.decode(transport);

            CallbackHolder<void, Args...> callback(handler);
            callback.dispatch(msg);
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    };

    int fd = subscribe(name);
    if (fd > 0) {
        mainloop.addEventSource(fd, EPOLLIN | EPOLLHUP | EPOLLRDHUP, callback);
    }
}

template<typename Type, typename... Args>
Type Client::methodCall(const std::string& method, Args&&... args)
{
    Message request = connection->createMessage(Message::MethodCall, method);
    request.packParameters(std::forward<Args>(args)...);
    connection->send(request);

    Message reply = connection->dispatch();

    Type response;
    reply.disclose<Type>(response);

    return response;
}

} // namespace rmi
#endif //__RMI_CLIENT_H__

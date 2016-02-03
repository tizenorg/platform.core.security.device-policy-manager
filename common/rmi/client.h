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
#include <mutex>
#include <unordered_map>

#include "error.h"
#include "object-latch.h"
#include "message.h"
#include "connection.h"
#include "mainloop.h"

namespace rmi {

class Client {
public:
    Client(const std::string& address);
    ~Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void connect();
    void disconnect();

    template<typename Type, typename... Args>
    Type methodCall(const std::string& method, const Args&... args);

private:
    typedef std::function<void(const Message&)> ReplyCallback;
    typedef std::unordered_map<unsigned int, ReplyCallback> ReplyCallbackMap;

    std::string address;

    ReplyCallbackMap replyCallbackMap;
    std::mutex replyCallbackLock;

    std::shared_ptr<Connection> connection;
    runtime::Mainloop mainloop;
    std::thread dispatcher;
};

template<typename Type, typename... Args>
Type Client::methodCall(const std::string& method, const Args&... args)
{
    ObjectLatch<Type> latch;

    auto ResultBuilder = [&latch](const Message& reply) {
        Type data;

        reply.unpackParameters<Type>(data);
        latch.set(std::move(data));
    };

    Message request = connection->createMessage(Message::MethodCall, method);
    request.packParameters(args...);

    replyCallbackLock.lock();
    replyCallbackMap[request.id()] = std::move(ResultBuilder);
    replyCallbackLock.unlock();

    connection->send(request);

    latch.wait();

    return latch.get();
}

} // namespace rmi
#endif //__RMI_CLIENT_H__

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

#ifndef __CLIENT__
#define __CLIENT__

#include <string>
#include <memory>

#include "error.hxx"
#include "message.hxx"
#include "connection.hxx"

namespace Ipc {

class Client {
public:
    Client(const std::string& address);
    ~Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void connect();
    void stop();

    template<typename Type, typename... Args>
    Type methodCall(const std::string& method, const Args&... args);

private:
    void packParameters(const Message& message)
    {
    }

    template<typename F>
    void packParameters(const Message& message, const F& arg)
    {
        message.enclose<F>(arg);
    }

    template<typename F, typename...R>
    void packParameters(const Message& message, const F& first, const R&... rest)
    {
        packParameters(message, first);
        packParameters(message, rest...);
    }

    std::string address;
    std::shared_ptr<Connection> connection;
};

template<typename Type, typename... Args>
Type Client::methodCall(const std::string& method, const Args&... args)
{
    Message request = connection->createMessage(Message::MethodCall, method);
    packParameters(request, args...);
    connection->send(request);

    auto handleResponse = [&]() {
        Message reply = connection->dispatch();
        if (reply.isError() || reply.isInvalid()) {
            throw Runtime::Exception(Runtime::GetSystemErrorMessage(EREMOTEIO));
        }

        Type response;
        reply.disclose<Type>(response);

        return response;
    };

    return handleResponse();
}

} // namespace Ipc
#endif //__CLIENT__

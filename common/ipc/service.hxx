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

#ifndef __SERVICE__
#define __SERVICE__

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

#include "preprocessor.hxx"
#include "mainloop.hxx"
#include "connection.hxx"
#include "message.hxx"
#include "callback-holder.hxx"

#define STRIP_(...)
#define STRIP(x)         STRIP_ x

#define TYPEOF____(...)   __VA_ARGS__
#define TYPEOF___(...)   (__VA_ARGS__),
#define TYPEOF__(x, ...) TYPEOF____ x
#define TYPEOF_(...)     TYPEOF__(__VA_ARGS__)
#define TYPEOF(x)        TYPEOF_(TYPEOF___ x,)

#define PLACEHOLDER(n)   std::placeholders::_##n

#define PROTOTYPE_(D, N) SEQUENCE(D, N)
#define PROTOTYPE(...)   PROTOTYPE_(PLACEHOLDER, VAR_ARGS_SIZE(__VA_ARGS__))

#define registerMethod(T, M, ...)                                             \
setMethodHandler<TYPEOF(M), TYPEOF(STRIP(STRIP(M)))>                          \
                (STRINGIFY(TYPEOF(STRIP(M))), std::bind(&TYPEOF(STRIP(M)), T, \
                PROTOTYPE(TYPEOF(STRIP(STRIP(M))))))

namespace Ipc {

typedef std::function<bool(const Connection& connection)> ConnectionCallback;

class Service {
public:
    Service(const std::string& address);
    ~Service();

    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    void start();
    void stop();

    template<typename Type, typename... Args>
    void setMethodHandler(const std::string& method,
                          const typename MethodHandler<Type, Args...>::type& handler);

    void setNewConnectionCallback(const ConnectionCallback& callback);
    void setCloseConnectionCallback(const ConnectionCallback& callback);

private:
    typedef std::shared_ptr<Connection> Client;
    typedef std::vector<Client> ConnectionRegistry;
    typedef std::function<void(const Client& connection)> CallbackDispatcher;

    typedef std::function<Message(const Message& message)> MethodDispatcher;
    typedef std::unordered_map<std::string, std::shared_ptr<MethodDispatcher>> MethodRegistry;

    void onMessageProcess(const Client& client);

    ConnectionRegistry::iterator getConnectionIterator(const int id);

    CallbackDispatcher onNewConnection;
    CallbackDispatcher onCloseConnection;

    MethodRegistry methodRegistry;
    ConnectionRegistry connectionRegistry;

    Mainloop mainloop;
    std::string address;
};

template<typename Type, typename... Args>
void Service::setMethodHandler(const std::string& method,
                               const typename MethodHandler<Type, Args...>::type& handler)
{
    auto dispatchMethod = [handler](const Message& message) {
        CallbackHolder<Type, Args...> callback(handler);

        Type response = callback.dispatch(message);

        Message reply = message.createReplyMessage();
        reply.enclose<Type>(response);

        return reply;
    };

    if (methodRegistry.count(method)) {
        throw Runtime::Exception("Method handler already registered");
    }

    methodRegistry[method] = std::make_shared<MethodDispatcher>(std::move(dispatchMethod));
}

} // namespace Ipc

#endif //__SERVICE__


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

#ifndef __CONNECTION__
#define __CONNECTION__

#include <string>
#include <mutex>

#include "socket.hxx"
#include "serialize.hxx"
#include "message.hxx"

namespace Ipc {

class Connection {
public:
    Connection(Socket&& sock);
    Connection(const Connection&) = delete;
    ~Connection() noexcept;

    Connection& operator=(const Connection&) = delete;
    Connection& operator=(Connection&) = delete;

    Message createMessage(unsigned int type, const std::string& target);

    void send(const Message& message) const;
    Message dispatch() const;

    int getFd() const {
        return socket.getFd();
    }

    Credentials getPeerCredentials() const {
        return socket.getPeerCredentials();
    }

private:
    Socket socket;
    mutable std::mutex receiveMutex;
    mutable std::mutex transmitMutex;
};

} // namespace Ipc
#endif //__CONNECTION__

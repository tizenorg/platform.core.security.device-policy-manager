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

#include "client.hxx"

namespace Ipc {

Client::Client(const std::string& path)
    : address(path)
{
}

Client::~Client()
{
    disconnect();
}

void Client::connect()
{
    connection = std::make_shared<Connection>(Socket::connect(address));

    dispatcher = std::thread([this] { mainloop.run(); });
}

int Client::subscribe(const std::string& name)
{
    Message request = connection->createMessage(Message::MethodCall, "Server::subscribeNotification");
    request.packParameters(name);
    connection->send(request);

    Message reply = connection->dispatch();
    if (reply.isError()) {
        return -1;
    }

    FileDescriptor response;
    reply.disclose(response);

    return response.fileDescriptor;
}

void Client::disconnect()
{
    mainloop.stop();
    dispatcher.join();
}

} // namespace Ipc

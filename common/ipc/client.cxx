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
    auto callback = [&](int fd, Mainloop::Event event) {
        if ((event & EPOLLHUP) || (event & EPOLLRDHUP)) {
            mainloop.removeEventSource(fd);
            return;
        }

        try {
            Message msg = connection->dispatch();

            replyCallbackLock.lock();
            ReplyCallback composer = std::move(replyCallbackMap.at(msg.id()));
            replyCallbackMap.erase(msg.id());
            replyCallbackLock.unlock();

            composer(msg);
        } catch (Runtime::Exception& e) {
            std::cout << e.what() << std::endl;
        }
    };

    connection = std::make_shared<Connection>(Socket::connect(address));
    mainloop.addEventSource(connection->getFd(),
                            EPOLLIN | EPOLLHUP | EPOLLRDHUP,
                            callback);

    dispatcher = std::thread([this] { mainloop.run(); });
}

void Client::disconnect()
{
    mainloop.stop();
    dispatcher.join();
    mainloop.removeEventSource(connection->getFd());
}

} // namespace Ipc


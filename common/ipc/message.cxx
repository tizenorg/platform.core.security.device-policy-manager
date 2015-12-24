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

#include "message.hxx"

namespace Ipc {

Message::Message()
    : signature({0, Invalid, ""})
{
}

Message::Message(unsigned int type, const std::string& target)
    : signature({0, type, target})
{
    enclose(signature);
}

Message::~Message()
{
}

Message::Message(const Message& message)
    : signature(message.signature)
{
    enclose(signature);
}

Message Message::createReplyMessage() const
{
    return Message(Reply, target());
}

Message Message::createErrorMessage() const
{
    return Message(Error, target());
}

} // namespace Ipc

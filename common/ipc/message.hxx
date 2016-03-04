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

#ifndef __MESSAGE__
#define __MESSAGE__

#include <cstring>

#include <string>
#include <memory>

#include "reflection.hxx"
#include "serialize.hxx"

namespace Ipc {

class MessageBuffer {
public:
    MessageBuffer(size_t caps = 1024) : capacity(caps), position(0)
    {
        buffer = new char[capacity];
    }

    ~MessageBuffer() {
        delete buffer;
    }

    void write(const void* ptr, const size_t sz) const
    {
        size_t bytes = sz;
        if ((position + bytes) > capacity) {
            bytes = capacity - position;
        }

        ::memcpy(reinterpret_cast<char *>(buffer + position), ptr, bytes);
        position += bytes;
    }

    void read(void* ptr, const size_t sz) const
    {
        size_t bytes = sz;
        if ((position + bytes) > capacity) {
            bytes = capacity - position;
        }

        ::memcpy(ptr, reinterpret_cast<char *>(buffer) + position, bytes);
        position += bytes;
    }

    void reset() const {
        position = 0;
    }

    char* begin() const {
        return buffer;
    }

    char* end() const {
        return buffer + position;
    }

    size_t size() const {
        return (end() - begin());
    }

private:
    char *buffer;
    size_t capacity;
    mutable int position;
};

class Message {
public:
    enum Type {
        Invalid,
        MethodCall,
        Reply,
        Error,
        Signal
    };

    Message(unsigned int type = Invalid, const std::string& target = "");
    Message(const Message& message);

    ~Message();

    Message& operator=(Message&) = delete;
    Message& operator=(const Message&) = delete;

    // [TBD] Take arguments
    Message createReplyMessage() const;
    Message createErrorMessage() const;

    template<typename DataType>
    void enclose(const DataType& data) const;

    template<typename DataType>
    void disclose(DataType& data) const;

    unsigned int id() const {
        return signature.id;
    }
    unsigned int type() const {
        return signature.type;
    }
    const std::string& target() const {
        return signature.target;
    }
    bool isInvalid() const {
        return type() == Invalid;
    }
    bool isError() const {
        return type() == Error;
    }
    bool isMethodCall() const {
        return type() == MethodCall;
    }
    bool isSignal() const {
        return type() == Signal;
    }
    bool isReply() const {
        return type() == Reply;
    }

    template<typename T>
    void encode(const T& device) const;

    template<typename T>
    void decode(const T& device);

private:
    struct MessageHeader {
        unsigned int id;    // Unique message id
        unsigned int type;  // Mesage type
        size_t length;// Total message length except MessageHeader itself
    };

    struct MessageSignature {
        unsigned int id;
        unsigned int type;
        std::string target;

        REFLECTABLE(
            id,
            type,
            target
        )
    };

    MessageSignature signature;
    MessageBuffer buffer;
};

template<typename DataType>
void Message::enclose(const DataType& data) const
{
    Runtime::Serializer<MessageBuffer> serializer(buffer);
    data.accept(serializer);
}

template<typename DataType>
void Message::disclose(DataType& data) const
{
    Runtime::Deserializer<MessageBuffer> deserializer(buffer);
    data.accept(deserializer);
}

template<typename T>
void Message::encode(const T& device) const
{
    MessageHeader header = {
        signature.id,
        signature.type,
        buffer.size()
    };

    device.write(&header, sizeof(header));
    device.write(buffer.begin(), header.length);
}

template<typename T>
void Message::decode(const T& device)
{
    MessageHeader header;
    device.read(&header, sizeof(header));

    buffer.reset();
    device.read(buffer.begin(), header.length);

    disclose(signature);
}

} // namespae Ipc
#endif //__MESSAGE__

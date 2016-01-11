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

#ifndef __RMI_MESSAGE_H__
#define __RMI_MESSAGE_H__

#include <cstring>
#include <iostream>
#include <string>
#include <memory>

#include "reflection.h"
#include "serialize.h"

namespace rmi {

class MemoryBlock {
public:
    MemoryBlock(size_t caps = 1024) :
        capacity(caps),
        produce(0),
        consume(0),
        buffer(new char[caps])
    {
    }

    MemoryBlock(const MemoryBlock& rhs) :
        capacity(rhs.capacity),
        produce(rhs.produce),
        consume(rhs.consume),
        buffer(new char[rhs.capacity])
    {
        std::copy(rhs.buffer + consume, rhs.buffer + produce, buffer + consume);
    }

    MemoryBlock(MemoryBlock&& rhs)
        : capacity(0),
          produce(0),
          consume(0),
          buffer(nullptr)
    {
        buffer = rhs.buffer;
        capacity = rhs.capacity;
        produce = rhs.produce;
        consume = rhs.consume;

        // Release buffer pointer from the source object so that
        // the destructor does not free the memory multiple times.

        rhs.buffer = nullptr;
        rhs.produce = 0;
        rhs.consume = 0;
    }

    ~MemoryBlock()
    {
        if (buffer != nullptr) {
            delete[] buffer;
        }
    }

    MemoryBlock& operator=(const MemoryBlock& rhs)
    {
        if (this != &rhs) {
            delete[] buffer;

            capacity = rhs.capacity;
            produce = rhs.produce;
            consume = rhs.consume;
            buffer = new char[capacity];
            std::copy(rhs.buffer + consume, rhs.buffer + produce, buffer + consume);
        }

        return *this;
    }

    MemoryBlock& operator=(MemoryBlock&& rhs)
    {
        if (this != &rhs) {
            // Free existing buffer
            delete[] buffer;

            // Copy the buffer pointer and its attributes from the
            // source object.
            buffer = rhs.buffer;
            produce = rhs.produce;
            consume = rhs.consume;
            capacity = rhs.capacity;

            // Release buffer pointer from the source object so that
            // the destructor does not free the memory multiple times.
            rhs.buffer = nullptr;
            rhs.produce = 0;
            rhs.consume = 0;
            rhs.capacity = 0;
        }

        return *this;
    }

    void write(const void* ptr, const size_t sz) const
    {
        size_t bytes = sz;
        if ((produce + bytes) > capacity) {
            bytes = capacity - produce;
        }

        ::memcpy(reinterpret_cast<char *>(buffer + produce), ptr, bytes);
        produce += bytes;
    }

    void read(void* ptr, const size_t sz) const
    {
        size_t bytes = sz;
        if ((consume + bytes) > produce) {
            bytes = produce - consume;
        }

        ::memcpy(ptr, reinterpret_cast<char *>(buffer) + consume, bytes);
        consume += bytes;

        // Reset cursors if there is no data
        if (consume == produce) {
            consume = produce = 0;
        }
    }

    void reserve(size_t size)
    {
        produce = size;
    }

    void reset()
    {
        produce = consume = 0;
    }

    char* begin() const
    {
        return buffer + consume;
    }

    char* end() const
    {
        return buffer + produce;
    }

    size_t size() const
    {
        return (end() - begin());
    }

private:
    size_t capacity;
    mutable size_t produce;
    mutable size_t consume;
    mutable char *buffer;
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

    Message();
    Message(unsigned int type, const std::string&);
    Message(const Message& rhs);
    Message(Message&& rhs);

    ~Message();

    Message& operator=(const Message& rhs);
    Message& operator=(Message&& rhs);

    // [TBD] Take arguments
    Message createReplyMessage() const;
    Message createErrorMessage() const;

    template<typename DataType>
    void enclose(const DataType& data) const;

    template<typename DataType>
    void disclose(DataType& data) const;

    unsigned int id() const
    {
        return signature.id;
    }

    unsigned int type() const
    {
        return signature.type;
    }

    const std::string& target() const
    {
        return signature.target;
    }

    bool isInvalid() const
    {
        return type() == Invalid;
    }

    bool isError() const
    {
        return type() == Error;
    }

    bool isMethodCall() const
    {
        return type() == MethodCall;
    }

    bool isSignal() const
    {
        return type() == Signal;
    }

    bool isReply() const
    {
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
    MemoryBlock buffer;
};

template<typename DataType>
void Message::enclose(const DataType& data) const
{
    runtime::Serializer<MemoryBlock> serializer(buffer);
    runtime::SerializableArgument<DataType> arg(data);
    arg.accept(serializer);
}

template<typename DataType>
void Message::disclose(DataType& data) const
{
    runtime::Deserializer<MemoryBlock> deserializer(buffer);
    runtime::DeserializableArgument<DataType> arg(data);
    arg.accept(deserializer);
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
    device.read(buffer.begin(), header.length);

    buffer.reserve(header.length);

    disclose(signature);
}

} // namespae rmi
#endif //__RMI_MESSAGE_H__

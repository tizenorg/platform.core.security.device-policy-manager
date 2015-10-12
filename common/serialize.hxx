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

#ifndef __SERIALIZER__
#define __SERIALIZER__

#include <vector>
#include <string>
#include <utility>

namespace Runtime {

template<class StorageType>
class Serializer {
public:
    Serializer(const StorageType& source) : storage(source) {}

    template<typename DataType>
    void visit(const std::string&, const DataType& value)
    {
        visitInternal(value);
    }

private:
    void visitInternal(const std::string& value)
    {
        visitInternal(value.size());
        storage.write(value.c_str(), value.size());
    }

    template<typename DataType, typename std::enable_if<std::is_arithmetic<DataType>::value, int>::type = 0>
    void visitInternal(const DataType& value)
    {
        storage.write(&value, sizeof(DataType));
    }

    template<typename DataType>
    void visitInternal(const std::vector<DataType>& values)
    {
        visitInternal(values.size());
        for (const DataType& value : values) {
            visitInternal(value);
        }
    }
private:
    const StorageType& storage;
};

template<class StorageType>
class Deserializer {
public:
    Deserializer(const StorageType& source) : storage(source) {}

    template<typename DataType>
    void visit(const std::string&, DataType& value)
    {
        visitInternal(value);
    }

private:
    void visitInternal(std::string& value)
    {
        size_t size;
        visitInternal(size);
        value.resize(size);
        storage.read(&value.front(), size);
    }

    template<typename DataType, typename std::enable_if<std::is_arithmetic<DataType>::value, int>::type = 0>
    void visitInternal(DataType& value)
    {
        storage.read(&value, sizeof(DataType));
    }

    template<typename DataType>
    void visitInternal(std::vector<DataType>& values)
    {
        size_t size;
        visitInternal(size);
        values.resize(size);

        for (DataType& value : values) {
            visitInternal(value);
        }
    }
private:
    const StorageType& storage;
};

} // namespace Runtime

#endif //__SERIALIZER__

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

#ifndef __OBJECT_LATCH__
#define __OBJECT_LATCH__

#include <utility>
#include <mutex>
#include <condition_variable>

template<typename DataType>
class ObjectLatch {
public:
    ObjectLatch() : state(false) {}

    ObjectLatch(const ObjectLatch&) = delete;
    ObjectLatch& operator=(const ObjectLatch&) = delete;

    void set(const DataType& val) {
        data = val;
        state = true;
        condition.notify_one();
    }

    void set(DataType&& val) {
        data  = std::move(val);
        state = true;
        condition.notify_one();
    }

    DataType get() {
        return DataType(std::move(data));
    }

    bool isValid() const {
        return state == true;
    }

    void wait() {
        std::unique_lock<std::mutex> lock(stateLock);
        condition.wait(lock, [this]{
            return isValid();
        });
    }

private:
    DataType data;
    bool state;
    std::mutex stateLock;
    std::condition_variable condition;
};

#endif //__OBJECT_LATCH__

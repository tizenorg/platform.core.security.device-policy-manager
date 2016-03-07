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

#ifndef __SHADOW_LOCK__
#define __SHADOW_LOCK__

#include <mutex>
#include <string>
#include <limits.h>

namespace Shadow {

class PwdFileLock final {
public:
    void lock(void);
    void unlock(void);

    ~PwdFileLock();

private:
    static std::mutex mutex;
    bool isLocked = false;
};

} // namespace Shadow

#endif //__SHADOW_LOCK__

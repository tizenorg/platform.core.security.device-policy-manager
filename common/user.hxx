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

#ifndef __USER__
#define __USER__

#include <string>
#include <limits.h>
#include <sys/types.h>

#include "group.hxx"

#ifndef INVALID_UID
#define INVALID_UID             UINT_MAX
#endif

namespace Runtime  {

class User final {
public:
    const std::string& getName() const {
        return name;
    }

    uid_t getUid() const {
        return uid;
    }

    const Group& getGroup() {
        return group;
    }

    User(const std::string& name);
    User(const uid_t user);
    User(const User& user);

private:
    std::string name;
    uid_t uid;

    Group group;
};

} // namespace Runtime

#endif //__USER__

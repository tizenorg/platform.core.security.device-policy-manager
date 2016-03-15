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


#include <regex>
#include <memory>

#include <grp.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "group.hxx"
#include "exception.hxx"

namespace Runtime {

Group::Group(const Group& group)
    : name(group.name), gid(group.gid)
{
}

Group::Group(const std::string& group)
{
    struct group* grp;

    grp = ::getgrnam(group.c_str());
    if (grp == NULL) {
        throw Runtime::Exception("Group doesn't exist");
    }

    name = grp->gr_name;
    gid = grp->gr_gid;
}

Group::Group(const gid_t group)
{
    struct group* grp;

    grp = ::getgrgid(group);
    if (grp == NULL) {
        throw Runtime::Exception("Group doesn't exist");
    }

    name = grp->gr_name;
    gid = grp->gr_gid;
}

Group::Group()
    : gid(INVALID_GID)
{
}

} // namespace Shadow

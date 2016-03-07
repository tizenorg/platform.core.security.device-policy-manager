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

#ifndef __SHADOW__
#define __SHADOW__

#include <cstdio>
#include <string>

#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <gshadow.h>
#include <limits.h>
#include <sys/types.h>

namespace Runtime  {

class Shadow final {
public:
    Shadow() = delete;
    Shadow(Shadow&&) = delete;
    Shadow(const Shadow&) = delete;

    Shadow& operator=(const Shadow&) = delete;
    Shadow& operator=(Shadow &&) = delete;

    static void putPasswd(std::string filename, const struct passwd& ent);
    static void putShadow(std::string filename, const struct spwd& ent);
    static void putGroup(std::string filename, const struct group& ent);
    static void putGshadow(std::string filename, const struct sgrp& ent);

    static void removePasswd(std::string filename, uid_t uid);
    static void removeShadow(std::string filename, std::string user);
    static void removeGroup(std::string filename, gid_t gid);
    static void removeGshadow(std::string filename, std::string group);

private:
    template<typename pwdStruct>
    static void put(std::string& filename, const pwdStruct& pwd,
                    std::function<int(const pwdStruct*, FILE*)> put);

    template<typename pwdStruct, typename element>
    static void remove(std::string& filename, const element& value,
                       std::function<int(const pwdStruct*, FILE*)> put,
                       std::function<pwdStruct *(FILE*)> get,
                       std::function<bool(const pwdStruct&, const element&)> compare);
};

} // namespace Shadow

#endif //__SHADOW__

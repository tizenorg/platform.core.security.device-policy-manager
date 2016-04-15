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

#ifndef __RUNTIME_SHADOW_H__
#define __RUNTIME_SHADOW_H__

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <gshadow.h>
#include <limits.h>

#include <cstdio>
#include <string>
#include <functional>

namespace runtime  {

class Shadow final {
public:
    Shadow() = delete;
    Shadow(Shadow&&) = delete;
    Shadow(const Shadow&) = delete;

    Shadow& operator=(const Shadow&) = delete;
    Shadow& operator=(Shadow &&) = delete;

    static void putPasswd(const std::string& filename, const struct passwd& ent);
    static void putShadow(const std::string& filename, const struct spwd& ent);
    static void putGroup(const std::string& filename, const struct group& ent);
    static void putGshadow(const std::string& filename, const struct sgrp& ent);

    static void foreachPasswd(const std::string& filename, std::function<bool(passwd&)> check);
    static void foreachShadow(const std::string& filename, std::function<bool(spwd&)> check);
    static void foreachGroup(const std::string& filename, std::function<bool(group&)> check);
    static void foreachGshadow(const std::string& filename, std::function<bool(sgrp&)> check);

private:
    template<typename pwdStruct>
    static void put(const std::string& filename, const pwdStruct& pwd,
                    std::function<int(const pwdStruct*, FILE*)> put);

    template<typename pwdStruct>
    static void foreach(const std::string& filename,
                        std::function<int(const pwdStruct*, FILE*)> put,
                        std::function<pwdStruct *(FILE*)> get,
                        std::function<bool(pwdStruct&)> check);
};

} // namespace runtime

#endif //__RUNTIME_SHADOW_H__

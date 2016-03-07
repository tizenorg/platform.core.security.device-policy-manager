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

#include "parameter.hxx"
#include "shadow/user.hxx"

namespace Runtime {

void Parameter::replaceAll(std::string& text, const std::string from, const std::string to)
{
    size_t pos = 0;

    pos = text.find(from, pos);
    while (pos != std::string::npos) {
        text.replace(pos, from.length(), to);
        pos = text.find(from, pos);
    }
}

void Parameter::parse(std::string& str, const std::string& username)
{
    Shadow::User user(username);

    replaceAll(str, "%USER%", user.getName());
    replaceAll(str, "%UID%",  std::to_string(user.getUid()));
    replaceAll(str, "%GROUP%", user.getGroup());
    replaceAll(str, "%GID%", std::to_string(user.getGid()));
    replaceAll(str, "%HOME%", "/home/" + user.getName());
    replaceAll(str, "%RUN%", "/run/user/" + std::to_string(user.getUid()));
}

} // namespace Runtime

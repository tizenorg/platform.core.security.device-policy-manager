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

#ifndef __DPM_SMACK__
#define __DPM_SMACK__

#include <string>

#include "filesystem.hxx"

namespace Runtime {

class Smack final {
public:
    Smack() = delete;
    Smack(const Smack&) = delete;
    Smack(Smack&&) = delete;

    Smack& operator=(const Smack&) = delete;
    Smack& operator=(Smack&&) = delete;

    static void setAccess(File& file, const std::string& label);
    static void setExecute(File& file, const std::string& label);
    static void setMmap(File& file, const std::string& label);
    static void setTransmute(File& file, const bool enable);

    static std::string getAccess(File& file);
    static std::string getExecute(File& file);
    static std::string getMmap(File& file);
    static bool getTransmute(File& file);


private:
    static void set(File& file, const char* xattr, const std::string& label);
    static std::string get(File& file, const char* xattr);
};

} // namespace Runtime
#endif //__DPM_SMACK__

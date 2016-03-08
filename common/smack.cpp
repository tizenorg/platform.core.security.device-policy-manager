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

#include <string>
#include <sys/smack.h>
#include <linux/xattr.h>

#include "smack.h"
#include "exception.h"

namespace runtime {

void Smack::setAccess(File& file, const std::string& label)
{
    set(file, XATTR_NAME_SMACK, label);
}

void Smack::setExecute(File& file, const std::string& label)
{
    set(file, XATTR_NAME_SMACKEXEC, label);
}

void Smack::setMmap(File& file, const std::string& label)
{
    set(file, XATTR_NAME_SMACKMMAP, label);
}

void Smack::setTransmute(File& file, const bool enable)
{
    const char* enable_value;

    if (enable) {
        enable_value = "TRUE";
    } else {
        enable_value = "FALSE";
    }

    set(file, XATTR_NAME_SMACKTRANSMUTE, enable_value);
}

std::string Smack::getAccess(File& file)
{
    return get(file, XATTR_NAME_SMACK);
}

std::string Smack::getExecute(File& file)
{
    return get(file, XATTR_NAME_SMACKEXEC);
}

std::string Smack::getMmap(File& file)
{
    return get(file, XATTR_NAME_SMACKMMAP);
}

bool Smack::getTransmute(File& file)
{
    std::string label;
    label = get(file, XATTR_NAME_SMACKTRANSMUTE);

    if (label == "TRUE") {
        return true;
    }
    return false;
}

void Smack::set(File& file, const char* xattr, const std::string& label)
{
    if (::smack_set_label_for_path(file.getPath().c_str(),
                                   xattr, 0, label.c_str()) != 0) {
        throw runtime::Exception("Smack setting error");
    }
}

std::string Smack::get(File& file, const char* xattr)
{
    char* plabel;
    std::string label;
    if (::smack_new_label_from_path(file.getPath().c_str(),
                                    xattr, 0, &plabel) != 0) {
        throw runtime::Exception("Getting smack label error");
    }

    label = plabel;
    free(plabel);

    return label;
}

} // namespace runtime

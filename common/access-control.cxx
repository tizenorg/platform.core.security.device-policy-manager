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

#include "access-control.hxx"
#include "exception.hxx"

namespace Runtime {

void SmackFileLabel::setAccess(File& file, const std::string& label)
{
    set(file, XATTR_NAME_SMACK, label);
}
void SmackFileLabel::setExecute(File& file, const std::string& label)
{
    set(file, XATTR_NAME_SMACKEXEC, label);
}
void SmackFileLabel::setMmap(File& file, const std::string& label)
{
    set(file, XATTR_NAME_SMACKMMAP, label);
}
void SmackFileLabel::setTransmute(File& file, const bool enable)
{
    const char* enable_value;

    if (enable) {
        enable_value = "TRUE";
    } else {
        enable_value = "FALSE";
    }

    set(file, XATTR_NAME_SMACKTRANSMUTE, enable_value);
}

const std::string SmackFileLabel::getAccess(File& file)
{
    return get(file, XATTR_NAME_SMACK);
}
const std::string SmackFileLabel::getExecute(File& file)
{
    return get(file, XATTR_NAME_SMACKEXEC);
}
const std::string SmackFileLabel::getMmap(File& file)
{
    return get(file, XATTR_NAME_SMACKMMAP);
}
bool SmackFileLabel::getTransmute(File& file)
{
    std::string label;
    label = get(file, XATTR_NAME_SMACKTRANSMUTE);

    if (label == "TRUE") {
        return true;
    }
    return false;
}

void SmackFileLabel::set(File& file, const char* xattr, const std::string& label)
{
    if (::smack_set_label_for_path(file.getPath().c_str(),
                                   xattr, 0, label.c_str()) != 0) {
        throw Runtime::Exception("Smack setting error");
    }
}

const std::string SmackFileLabel::get(File& file, const char* xattr)
{
    char* plabel;
    std::string label;
    if (::smack_new_label_from_path(file.getPath().c_str(),
                                    xattr, 0, &plabel) != 0) {
        throw Runtime::Exception("Getting smack label error");
    }

    label = plabel;
    free(plabel);

    return label;
}

} // namespace Runtime

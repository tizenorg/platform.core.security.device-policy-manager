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

void SmackFileLabel::setAccess(const std::string& label)
{
    set(XATTR_NAME_SMACK, label);
}
void SmackFileLabel::setExecute(const std::string& label)
{
    set(XATTR_NAME_SMACKEXEC, label);
}
void SmackFileLabel::setMmap(const std::string& label)
{
    set(XATTR_NAME_SMACKMMAP, label);
}
void SmackFileLabel::setTransmute(const bool enable)
{
    const char* enable_value;

    if (enable) {
        enable_value = "TRUE";
    } else {
        enable_value = "FALSE";
    }

    set(XATTR_NAME_SMACKTRANSMUTE, enable_value);
}

const std::string SmackFileLabel::getAccess() const
{
    return get(XATTR_NAME_SMACK);
}
const std::string SmackFileLabel::getExecute() const
{
    return get(XATTR_NAME_SMACKEXEC);
}
const std::string SmackFileLabel::getMmap() const
{
    return get(XATTR_NAME_SMACKMMAP);
}
const bool SmackFileLabel::getTransmute() const
{
    std::string label;
    label = get(XATTR_NAME_SMACKTRANSMUTE);

    if (label == "TRUE") {
        return true;
    }
    return false;
}

void SmackFileLabel::set(const char* xattr, const std::string& label)
{
    if (::smack_set_label_for_path(file.getPath().c_str(),
                                   xattr, 0, label.c_str()) != 0) {
        throw Runtime::Exception("Smack setting error");
    }
}

const std::string SmackFileLabel::get(const char* xattr) const
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

SmackFileLabel::SmackFileLabel(File& file)
    : file(file)
{
}


void SmackSocketLabel::setIn(const std::string& label)
{
    set(XATTR_NAME_SMACKIPIN, label);
}

void SmackSocketLabel::setOut(const std::string& label)
{
    set(XATTR_NAME_SMACKIPOUT, label);
}

const std::string SmackSocketLabel::getIn() const
{
    return get(XATTR_NAME_SMACKIPIN);
}

const std::string SmackSocketLabel::getOut() const
{
    return get(XATTR_NAME_SMACKIPIN);
}


void SmackSocketLabel::set(const char* xattr, const std::string& label)
{
    if (::smack_set_label_for_file(sockfd, xattr, label.c_str()) != 0) {
        throw Runtime::Exception("Smack setting error");
    }
}
const std::string SmackSocketLabel::get(const char* xattr) const
{
    char* plabel;
    std::string label;
    if (::smack_new_label_from_file(sockfd, xattr, &plabel) != 0) {
        throw Runtime::Exception("Getting smack label error");
    }

    label = plabel;
    free(plabel);

    return label;
}

SmackSocketLabel::SmackSocketLabel(int sockfd)
    : sockfd(sockfd)
{
}

} // namespace Runtime

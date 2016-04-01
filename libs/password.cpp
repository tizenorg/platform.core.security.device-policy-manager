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

#include "password.hxx"
#include "audit/logger.h"

namespace DevicePolicyManager {

Password::Password(PolicyControlContext &ctxt) :
    __context(ctxt)
{
}

Password::~Password()
{
}

int Password::setPasswordQuality(const int quality)
{
    try {
        return __context->methodCall<int>("Password::setPasswordQuality", quality);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordQuality()
{
    try {
        return __context->methodCall<int>("Password::getPasswordQuality");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordMinimumLength(const int value)
{
    try {
        return __context->methodCall<int>("Password::setPasswordMinimumLength", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordMinimumLength()
{
    try {
        return __context->methodCall<int>("Password::getPasswordMinimumLength");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMinPasswordComplexChars(const int value)
{
    try {
        return __context->methodCall<int>("Password::setMinPasswordComplexChars", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMinPasswordComplexChars()
{
    try {
        return __context->methodCall<int>("Password::getMinPasswordComplexChars");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMaximumFailedPasswordForWipe(const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaximumFailedPasswordForWipe", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}
int Password::getMaximumFailedPasswordForWipe()
{
    try {
        return __context->methodCall<int>("Password::getMaximumFailedPasswordForWipe");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordExpires(const int value)
{
    try {
        return __context->methodCall<int>("Password::setPasswordExpires", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordExpires()
{
    try {
        return __context->methodCall<int>("Password::getPasswordExpires");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordHistory(const int value)
{
    try {
        return __context->methodCall<int>("Password::setPasswordHistory", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordHistory()
{
    try {
        return __context->methodCall<int>("Password::getPasswordHistory");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordPattern(const std::string& pattern)
{
    try {
        return __context->methodCall<int>("Password::setPasswordPattern", pattern);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::resetPassword(const std::string& passwd)
{
    try {
        return __context->methodCall<int>("Password::resetPassword", passwd);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::enforcePasswordChange()
{
    try {
        return __context->methodCall<int>("Password::enforcePasswordChange");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMaxInactivityTimeDeviceLock(const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaxInactivityTimeDeviceLock", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMaxInactivityTimeDeviceLock()
{
    try {
        return __context->methodCall<int>("Password::getMaxInactivityTimeDeviceLock");;
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordStatus(const int status)
{
    try {
        return __context->methodCall<int>("Password::setPasswordStatus", status);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::deletePasswordPattern()
{
    try {
        return __context->methodCall<int>("Password::deletePasswordPattern");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::string Password::getPasswordPattern()
{
    std::string error("Error");
    try {
        return __context->methodCall<std::string>("Password::getPasswordPattern");
    } catch (runtime::Exception& e) {
        return error;
    }
}

int Password::setMaximumCharacterOccurrences(const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaximumCharacterOccurrences", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMaximumCharacterOccurrences()
{
    try {
        return __context->methodCall<int>("Password::getMaximumCharacterOccurrences");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMaximumNumericSequenceLength(const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaximumNumericSequenceLength", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMaximumNumericSequenceLength()
{
    try {
        return __context->methodCall<int>("Password::getMaximumNumericSequenceLength");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setForbiddenStrings(const std::vector<std::string>& forbiddenStrings)
{
    try {
        return __context->methodCall<int>("Password::setForbiddenStrings", forbiddenStrings);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> Password::getForbiddenStrings()
{
    try {
        return __context->methodCall<std::vector<std::string>>("Password::getForbiddenStrings");
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}
} /* namespace DevicePolicyManager */

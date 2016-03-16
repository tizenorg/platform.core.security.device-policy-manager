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

int Password::setPasswordQuality(const std::string& username, const int quality)
{
    try {
        return __context->methodCall<int>("Password::setPasswordQuality", username, quality);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordQuality(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getPasswordQuality", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordMinimumLength(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setPasswordMinimumLength", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordMinimumLength(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getPasswordMinimumLength", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMinPasswordComplexChars(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setMinPasswordComplexChars", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMinPasswordComplexChars(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getMinPasswordComplexChars", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMaximumFailedPasswordForWipe(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaximumFailedPasswordForWipe", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}
int Password::getMaximumFailedPasswordForWipe(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getMaximumFailedPasswordForWipe", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordExpires(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setPasswordExpires", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordExpires(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getPasswordExpires", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordHistory(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setPasswordHistory", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getPasswordHistory(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getPasswordHistory", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordPattern(const std::string& username, const std::string& pattern)
{
    try {
        return __context->methodCall<int>("Password::setPasswordPattern", username, pattern);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::resetPassword(const std::string& username, const std::string& passwd)
{
    try {
        return __context->methodCall<int>("Password::resetPassword", username, passwd);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::enforcePasswordChange(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::enforcePasswordChange", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMaxInactivityTimeDeviceLock(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaxInactivityTimeDeviceLock", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMaxInactivityTimeDeviceLock(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getMaxInactivityTimeDeviceLock", username);;
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setPasswordStatus(const std::string& username, const int status)
{
    try {
        return __context->methodCall<int>("Password::setPasswordStatus", username, status);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::deletePasswordPattern(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::deletePasswordPattern", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::string Password::getPasswordPattern(const std::string& username)
{
    std::string error("Error");
    try {
        return __context->methodCall<std::string>("Password::getPasswordPattern", username);
    } catch (runtime::Exception& e) {
        return error;
    }
}

int Password::setMaximumCharacterOccurrences(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaximumCharacterOccurrences", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMaximumCharacterOccurrences(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getMaximumCharacterOccurrences", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setMaximumNumericSequenceLength(const std::string& username, const int value)
{
    try {
        return __context->methodCall<int>("Password::setMaximumNumericSequenceLength", username, value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::getMaximumNumericSequenceLength(const std::string& username)
{
    try {
        return __context->methodCall<int>("Password::getMaximumNumericSequenceLength", username);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Password::setForbiddenStrings(const std::string& username, const std::vector<std::string>& forbiddenStrings)
{
    try {
        return __context->methodCall<int>("Password::setForbiddenStrings", username, forbiddenStrings);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> Password::getForbiddenStrings(const std::string& username)
{
    try {
        return __context->methodCall<std::vector<std::string>>("Password::getForbiddenStrings", username);
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}
} /* namespace DevicePolicyManager */

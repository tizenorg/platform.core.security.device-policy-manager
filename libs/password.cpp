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

PasswordPolicy::PasswordPolicy(PolicyControlContext &ctxt) :
    __context(ctxt)
{
}

PasswordPolicy::~PasswordPolicy()
{
}

int PasswordPolicy::setQuality(int quality)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setQuality", quality);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getQuality()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getQuality");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setMinimumLength(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setMinimumLength", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getMinimumLength()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getMinimumLength");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setMinimumComplexCharacters(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setMinimumComplexCharacters", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getMinimumComplexCharacters()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getMinimumComplexCharacters");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setMaximumFailureCount(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setMaximumFailureCount", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}
int PasswordPolicy::getMaximumFailureCount()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getMaximumFailureCount");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setExpires(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setExpires", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getExpires()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getExpires");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setHistorySize(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setHistorySize", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getHistorySize()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getHistorySize");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setPattern(const std::string& pattern)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setPattern", pattern);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::reset(const std::string& passwd)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::reset", passwd);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::enforceChange()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::enforceChange");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setMaximumInactivityTimeDeviceLock(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setMaximumInactivityTimeDeviceLock", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getMaximumInactivityTimeDeviceLock()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getMaximumInactivityTimeDeviceLock");;
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setStatus(int status)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setStatus", status);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getStatus()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getStatus");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::deletePattern()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::deletePattern");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::string PasswordPolicy::getPattern()
{
    std::string error("Error");
    try {
        return __context->methodCall<std::string>("PasswordPolicy::getPattern");
    } catch (runtime::Exception& e) {
        return error;
    }
}

int PasswordPolicy::setMaximumCharacterOccurrences(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setMaximumCharacterOccurrences", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getMaximumCharacterOccurrences()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getMaximumCharacterOccurrences");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setMaximumNumericSequenceLength(int value)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setMaximumNumericSequenceLength", value);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::getMaximumNumericSequenceLength()
{
    try {
        return __context->methodCall<int>("PasswordPolicy::getMaximumNumericSequenceLength");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int PasswordPolicy::setForbiddenStrings(const std::vector<std::string>& forbiddenStrings)
{
    try {
        return __context->methodCall<int>("PasswordPolicy::setForbiddenStrings", forbiddenStrings);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> PasswordPolicy::getForbiddenStrings()
{
    std::vector<std::string> error;
    try {
        return __context->methodCall<std::vector<std::string>>("PasswordPolicy::getForbiddenStrings");
    } catch (runtime::Exception& e) {
        return error;
    }
}
} /* namespace DevicePolicyManager */

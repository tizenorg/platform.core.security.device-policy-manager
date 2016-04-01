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

#ifndef __PASSWORD_POLICY__
#define __PASSWORD_POLICY__

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager {

class Password {
public:
    typedef enum {
        DPM_PASSWORD_QUALITY_UNSPECIFIED     = 0x00,    /**< No requirements for password. */
        DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD = 0x01,    /**< Eas requirement for simple password */
        DPM_PASSWORD_QUALITY_SOMETHING       = 0x10,    /**< Some kind password is required, but doesn't care what it is */
        DPM_PASSWORD_QUALITY_NUMERIC         = 0x20,    /**< Containing at least numeric characters */
        DPM_PASSWORD_QUALITY_ALPHABETIC      = 0x40,    /**< Containing at least alphabetic (or other symbol) characters */
        DPM_PASSWORD_QUALITY_ALPHANUMERIC    = 0x80,    /**< Containing at least numeric and alphabetic characters */
    } PasswordQuality;

    Password(PolicyControlContext &ctxt);
    ~Password();

    int setPasswordQuality(const int quality);
    int getPasswordQuality();
    int setPasswordMinimumLength(const int value);
    int getPasswordMinimumLength();
    int setMinPasswordComplexChars(const int value);
    int getMinPasswordComplexChars();
    int setMaximumFailedPasswordForWipe(const int value);
    int getMaximumFailedPasswordForWipe();
    int setPasswordExpires(const int value);
    int getPasswordExpires();
    int setPasswordHistory(const int value);
    int getPasswordHistory();
    int setPasswordPattern(const std::string& pattern);
    int resetPassword(const std::string& passwd);
    int enforcePasswordChange();
    int setMaxInactivityTimeDeviceLock(const int value);
    int getMaxInactivityTimeDeviceLock();
    std::string getPasswordPolicy();
    int setPasswordStatus(const int status);
    int deletePasswordPattern();
    std::string getPasswordPattern();
    int setMaximumCharacterOccurrences(const int value);
    int getMaximumCharacterOccurrences();
    int setMaximumNumericSequenceLength(const int value);
    int getMaximumNumericSequenceLength();
    int setForbiddenStrings(const std::vector<std::string>& forbiddenPasswds);
    std::vector<std::string> getForbiddenStrings();

private:
    PolicyControlContext& __context;
};

} /* namespace DevicePolicyManager */
#endif /* __PASSWORD_POLICY__ */

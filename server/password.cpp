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

#include <sys/types.h>

#include <auth-passwd-admin.h>

#include "password.hxx"

#include "auth/user.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

namespace {

Password::PasswordQuality qualityType;
unsigned int minLength;
unsigned int minComplexCharNumber;
unsigned int maxAttempts;
unsigned int validPeriod;
unsigned int historySize;
std::string passwordPattern;
unsigned int maxInactivityTime;
unsigned int passwdStatus;
unsigned int maxCharOccurrences;
unsigned int maxNumSeqLength;
std::vector<std::string> forbiddenPasswds;

int transformValueFromIntToQualityType(const int quality, Password::PasswordQuality &changed_quality)
{
    switch (quality) {
    case Password::DPM_PASSWORD_QUALITY_UNSPECIFIED:
        changed_quality = Password::DPM_PASSWORD_QUALITY_UNSPECIFIED;
        break;
    case Password::DPM_PASSWORD_QUALITY_SOMETHING:
        changed_quality = Password::DPM_PASSWORD_QUALITY_SOMETHING;
        break;
    case Password::DPM_PASSWORD_QUALITY_NUMERIC:
        changed_quality = Password::DPM_PASSWORD_QUALITY_NUMERIC;
        break;
    case Password::DPM_PASSWORD_QUALITY_ALPHABETIC:
        changed_quality = Password::DPM_PASSWORD_QUALITY_ALPHABETIC;
        break;
    case Password::DPM_PASSWORD_QUALITY_ALPHANUMERIC:
        changed_quality = Password::DPM_PASSWORD_QUALITY_ALPHANUMERIC;
        break;
    default:
        std::cerr << "unknown password quality" << std::endl;
        return -1;
        break;
    }

    return 0;
}

int transformQualityFromDPMToAuth(const int dpm_quality, password_quality_type &auth_quality)
{
    switch (dpm_quality) {
    case Password::DPM_PASSWORD_QUALITY_UNSPECIFIED:
        auth_quality = AUTH_PWD_QUALITY_UNSPECIFIED;
        break;
    case Password::DPM_PASSWORD_QUALITY_SOMETHING:
        auth_quality = AUTH_PWD_QUALITY_SOMETHING;
        break;
    case Password::DPM_PASSWORD_QUALITY_NUMERIC:
        auth_quality = AUTH_PWD_QUALITY_NUMERIC;
        break;
    case Password::DPM_PASSWORD_QUALITY_ALPHABETIC:
        auth_quality = AUTH_PWD_QUALITY_ALPHABETIC;
        break;
    case Password::DPM_PASSWORD_QUALITY_ALPHANUMERIC:
        auth_quality = AUTH_PWD_QUALITY_ALPHANUMERIC;
        break;
    default:
        std::cerr << "unknown" << std::endl;
        return -1;
        break;
    }

    return 0;
}

} // namespace

Password::Password(PolicyControlContext &ctxt) :
    __context(ctxt)
{
    ctxt.registerParametricMethod(this, (int)(Password::setPasswordQuality)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getPasswordQuality));
    ctxt.registerParametricMethod(this, (int)(Password::setPasswordMinimumLength)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getPasswordMinimumLength));
    ctxt.registerParametricMethod(this, (int)(Password::setMinPasswordComplexChars)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getMinPasswordComplexChars));
    ctxt.registerParametricMethod(this, (int)(Password::setMaximumFailedPasswordForWipe)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getMaximumFailedPasswordForWipe));
    ctxt.registerParametricMethod(this, (int)(Password::setPasswordExpires)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getPasswordExpires)());
    ctxt.registerParametricMethod(this, (int)(Password::setPasswordHistory)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getPasswordHistory)());
    ctxt.registerParametricMethod(this, (int)(Password::setPasswordPattern)(std::string));
    ctxt.registerParametricMethod(this, (int)(Password::resetPassword)(std::string));
    ctxt.registerNonparametricMethod(this, (int)(Password::enforcePasswordChange));
    ctxt.registerParametricMethod(this, (int)(Password::setMaxInactivityTimeDeviceLock)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getMaxInactivityTimeDeviceLock));
    ctxt.registerParametricMethod(this, (int)(Password::setPasswordStatus)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::deletePasswordPattern));
    ctxt.registerNonparametricMethod(this, (std::string)(Password::getPasswordPattern));
    ctxt.registerParametricMethod(this, (int)(Password::setMaximumCharacterOccurrences)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getMaximumCharacterOccurrences));
    ctxt.registerParametricMethod(this, (int)(Password::setMaximumNumericSequenceLength)(int));
    ctxt.registerNonparametricMethod(this, (int)(Password::getMaximumNumericSequenceLength));
    ctxt.registerParametricMethod(this, (int)(Password::setForbiddenStrings)(std::vector<std::string>));
    ctxt.registerNonparametricMethod(this, (std::vector<std::string>)(Password::getForbiddenStrings));
}

Password::~Password()
{
}

int Password::setPasswordQuality(const int quality)
{
    int ret = 0;
    policy_h *p_policy;
    password_quality_type auth_quality_type = AUTH_PWD_QUALITY_UNSPECIFIED;

    if (transformValueFromIntToQualityType(quality, qualityType) != 0) {
        return -1;
    }

    if (transformQualityFromDPMToAuth(qualityType, auth_quality_type) != 0) {
        return -1;
    }

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_quality(p_policy, auth_quality_type) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    return ret;
}

int Password::getPasswordQuality()
{
    return qualityType;
}

int Password::setPasswordMinimumLength(const int value)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_min_length(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    minLength = value;

    return ret;
}

int Password::getPasswordMinimumLength()
{
    return minLength;
}

int Password::setMinPasswordComplexChars(const int value)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_min_complex_char_num(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    minComplexCharNumber = value;

    return ret;
}

int Password::getMinPasswordComplexChars()
{
    return minComplexCharNumber;
}

int Password::setMaximumFailedPasswordForWipe(const int value)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_max_attempts(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    maxAttempts = value;

    return ret;
}

int Password::getMaximumFailedPasswordForWipe()
{
    return maxAttempts;
}

int Password::setPasswordExpires(const int value)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_validity(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    validPeriod = value;

    return ret;
}

int Password::getPasswordExpires()
{
    return validPeriod;
}

int Password::setPasswordHistory(const int value)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_history_size(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    historySize = value;

    return ret;
}

int Password::getPasswordHistory()
{
    return historySize;
}

int Password::setPasswordPattern(const std::string& pattern)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_pattern(p_policy, pattern.c_str()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    passwordPattern = pattern;

    return ret;
}

int Password::resetPassword(const std::string& passwd)
{
    int ret = 0;

    if (auth_passwd_reset_passwd(AUTH_PWD_NORMAL, __context.getPeerUid(), passwd.c_str()) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    return ret;
}

int Password::enforcePasswordChange()
{
    return 0;
}

int Password::setMaxInactivityTimeDeviceLock(const int value)
{
    maxInactivityTime = value;

    return 0;
}

int Password::getMaxInactivityTimeDeviceLock()
{
    return maxInactivityTime;
}

int Password::setPasswordStatus(const int status)
{
    passwdStatus = status;

    return 0;
}

int Password::deletePasswordPattern()
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_pattern(p_policy, NULL) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    passwordPattern.clear();

    return ret;
}

std::string Password::getPasswordPattern()
{
    return passwordPattern;
}

int Password::setMaximumCharacterOccurrences(const int value)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_max_char_occurrences(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    maxCharOccurrences = value;

    return ret;
}

int Password::getMaximumCharacterOccurrences()
{
    return maxCharOccurrences;
}

int Password::setMaximumNumericSequenceLength(const int value)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_max_num_seq_len(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    maxNumSeqLength = value;

    return ret;
}

int Password::getMaximumNumericSequenceLength()
{
    return maxNumSeqLength;
}

int Password::setForbiddenStrings(const std::vector<std::string>& forbiddenStrings)
{
    int ret = 0;
    policy_h *p_policy;

    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, __context.getPeerUid()) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    for (std::vector<std::string>::const_iterator iter = forbiddenStrings.begin(); iter != forbiddenStrings.end(); ++iter) {
        if (auth_passwd_set_forbidden_passwd(p_policy, (*iter).c_str()) != AUTH_PASSWD_API_SUCCESS) {
            auth_passwd_free_policy(p_policy);
            return -1;
        }
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

    std::copy(forbiddenStrings.begin(), forbiddenStrings.end(), std::back_inserter(forbiddenPasswds));

    return ret;
}

std::vector<std::string> Password::getForbiddenStrings()
{
    return forbiddenPasswds;
}

Password passwordPolicy(Server::instance());

} /* namespace DevicePolicyManager*/

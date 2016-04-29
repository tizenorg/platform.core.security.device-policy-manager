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

#include "policy-helper.h"
#include "auth/user.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

namespace {

PasswordPolicy::PasswordPolicyQuality qualityType;
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

int transformValueFromIntToQualityType(const int quality, PasswordPolicy::PasswordPolicyQuality &changed_quality)
{
    switch (quality) {
    case PasswordPolicy::DPM_PASSWORD_QUALITY_UNSPECIFIED:
        changed_quality = PasswordPolicy::DPM_PASSWORD_QUALITY_UNSPECIFIED;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_SOMETHING:
        changed_quality = PasswordPolicy::DPM_PASSWORD_QUALITY_SOMETHING;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_NUMERIC:
        changed_quality = PasswordPolicy::DPM_PASSWORD_QUALITY_NUMERIC;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_ALPHABETIC:
        changed_quality = PasswordPolicy::DPM_PASSWORD_QUALITY_ALPHABETIC;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_ALPHANUMERIC:
        changed_quality = PasswordPolicy::DPM_PASSWORD_QUALITY_ALPHANUMERIC;
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
    case PasswordPolicy::DPM_PASSWORD_QUALITY_UNSPECIFIED:
        auth_quality = AUTH_PWD_QUALITY_UNSPECIFIED;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_SOMETHING:
        auth_quality = AUTH_PWD_QUALITY_SOMETHING;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_NUMERIC:
        auth_quality = AUTH_PWD_QUALITY_NUMERIC;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_ALPHABETIC:
        auth_quality = AUTH_PWD_QUALITY_ALPHABETIC;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_ALPHANUMERIC:
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

PasswordPolicy::PasswordPolicy(PolicyControlContext &ctxt) :
    __context(ctxt)
{
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setPasswordPolicyQuality)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getPasswordPolicyQuality));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setPasswordPolicyMinimumLength)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getPasswordPolicyMinimumLength));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setMinPasswordPolicyComplexChars)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getMinPasswordPolicyComplexChars));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setMaximumFailedPasswordPolicyForWipe)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getMaximumFailedPasswordPolicyForWipe));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setPasswordPolicyExpires)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getPasswordPolicyExpires)());
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setPasswordPolicyHistory)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getPasswordPolicyHistory)());
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setPasswordPolicyPattern)(std::string));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::resetPasswordPolicy)(std::string));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::enforcePasswordPolicyChange));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setMaxInactivityTimeDeviceLock)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getMaxInactivityTimeDeviceLock));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setPasswordPolicyStatus)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::deletePasswordPolicyPattern));
    ctxt.registerNonparametricMethod(this, (std::string)(PasswordPolicy::getPasswordPolicyPattern));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setMaximumCharacterOccurrences)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getMaximumCharacterOccurrences));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setMaximumNumericSequenceLength)(int));
    ctxt.registerNonparametricMethod(this, (int)(PasswordPolicy::getMaximumNumericSequenceLength));
    ctxt.registerParametricMethod(this, (int)(PasswordPolicy::setForbiddenStrings)(std::vector<std::string>));
    ctxt.registerNonparametricMethod(this, (std::vector<std::string>)(PasswordPolicy::getForbiddenStrings));
}

PasswordPolicy::~PasswordPolicy()
{
}

int PasswordPolicy::setPasswordPolicyQuality(const int quality)
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

int PasswordPolicy::getPasswordPolicyQuality()
{
    return qualityType;
}

int PasswordPolicy::setPasswordPolicyMinimumLength(const int value)
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

int PasswordPolicy::getPasswordPolicyMinimumLength()
{
    return minLength;
}

int PasswordPolicy::setMinPasswordPolicyComplexChars(const int value)
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

int PasswordPolicy::getMinPasswordPolicyComplexChars()
{
    return minComplexCharNumber;
}

int PasswordPolicy::setMaximumFailedPasswordPolicyForWipe(const int value)
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

int PasswordPolicy::getMaximumFailedPasswordPolicyForWipe()
{
    return maxAttempts;
}

int PasswordPolicy::setPasswordPolicyExpires(const int value)
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

int PasswordPolicy::getPasswordPolicyExpires()
{
    return validPeriod;
}

int PasswordPolicy::setPasswordPolicyHistory(const int value)
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

int PasswordPolicy::getPasswordPolicyHistory()
{
    return historySize;
}

int PasswordPolicy::setPasswordPolicyPattern(const std::string& pattern)
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

int PasswordPolicy::resetPasswordPolicy(const std::string& passwd)
{
    int ret = 0;

    if (auth_passwd_reset_passwd(AUTH_PWD_NORMAL, __context.getPeerUid(), passwd.c_str()) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    return ret;
}

int PasswordPolicy::enforcePasswordPolicyChange()
{
    return 0;
}

int PasswordPolicy::setMaxInactivityTimeDeviceLock(const int value)
{
    maxInactivityTime = value;

    return 0;
}

int PasswordPolicy::getMaxInactivityTimeDeviceLock()
{
    return maxInactivityTime;
}

int PasswordPolicy::setPasswordPolicyStatus(const int status)
{
    passwdStatus = status;

    return 0;
}

int PasswordPolicy::deletePasswordPolicyPattern()
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

std::string PasswordPolicy::getPasswordPolicyPattern()
{
    return passwordPattern;
}

int PasswordPolicy::setMaximumCharacterOccurrences(const int value)
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

int PasswordPolicy::getMaximumCharacterOccurrences()
{
    return maxCharOccurrences;
}

int PasswordPolicy::setMaximumNumericSequenceLength(const int value)
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

int PasswordPolicy::getMaximumNumericSequenceLength()
{
    return maxNumSeqLength;
}

int PasswordPolicy::setForbiddenStrings(const std::vector<std::string>& forbiddenStrings)
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

std::vector<std::string> PasswordPolicy::getForbiddenStrings()
{
    return forbiddenPasswds;
}

PasswordPolicy passwordPolicy(Server::instance());

} /* namespace DevicePolicyManager*/

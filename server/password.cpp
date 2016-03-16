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
#include <pwd.h>

#include <auth-passwd-admin.h>

#include "password.hxx"

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
    rmi::Service &manager = __context.getServiceManager();

    manager.registerParametricMethod(this, (int)(Password::setPasswordQuality)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getPasswordQuality)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setPasswordMinimumLength)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getPasswordMinimumLength)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setMinPasswordComplexChars)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getMinPasswordComplexChars)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setMaximumFailedPasswordForWipe)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getMaximumFailedPasswordForWipe)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setPasswordExpires)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getPasswordExpires)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setPasswordHistory)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getPasswordHistory)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setPasswordPattern)(std::string, std::string));
    manager.registerParametricMethod(this, (int)(Password::resetPassword)(std::string, std::string));
    manager.registerParametricMethod(this, (int)(Password::enforcePasswordChange)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setMaxInactivityTimeDeviceLock)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getMaxInactivityTimeDeviceLock)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setPasswordStatus)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::deletePasswordPattern)(std::string));
    manager.registerParametricMethod(this, (std::string)(Password::getPasswordPattern)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setMaximumCharacterOccurrences)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getMaximumCharacterOccurrences)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setMaximumNumericSequenceLength)(std::string, int));
    manager.registerParametricMethod(this, (int)(Password::getMaximumNumericSequenceLength)(std::string));
    manager.registerParametricMethod(this, (int)(Password::setForbiddenStrings)(std::string, std::vector<std::string>));
    manager.registerParametricMethod(this, (std::vector<std::string>)(Password::getForbiddenStrings)(std::string));
}

Password::~Password()
{
}

int Password::setPasswordQuality(const std::string& username, const int quality)
{
    int ret = 0;
    struct passwd *user_info;
    password_quality_type auth_quality_type = AUTH_PWD_QUALITY_UNSPECIFIED;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
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

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getPasswordQuality(const std::string& username)
{
    return qualityType;
}

int Password::setPasswordMinimumLength(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getPasswordMinimumLength(const std::string& username)
{
    return minLength;
}

int Password::setMinPasswordComplexChars(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_minComplexCharNumber(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getMinPasswordComplexChars(const std::string& username)
{
    return minComplexCharNumber;
}

int Password::setMaximumFailedPasswordForWipe(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getMaximumFailedPasswordForWipe(const std::string& username)
{
    return maxAttempts;
}

int Password::setPasswordExpires(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getPasswordExpires(const std::string& username)
{
    return validPeriod;
}

int Password::setPasswordHistory(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getPasswordHistory(const std::string& username)
{
    return historySize;
}

int Password::setPasswordPattern(const std::string& username, const std::string& pattern)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::resetPassword(const std::string& username, const std::string& passwd)
{
    int ret = 0;
    struct passwd *user_info;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_reset_passwd(AUTH_PWD_NORMAL, user_info->pw_uid, passwd.c_str()) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    return ret;
}

int Password::enforcePasswordChange(const std::string& username)
{
    return 0;
}

int Password::setMaxInactivityTimeDeviceLock(const std::string& username, const int value)
{
    maxInactivityTime = value;

    return 0;
}

int Password::getMaxInactivityTimeDeviceLock(const std::string& username)
{
    return maxInactivityTime;
}

int Password::setPasswordStatus(const std::string& username, const int status)
{
    passwdStatus = status;

    return 0;
}

int Password::deletePasswordPattern(const std::string& username)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
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

std::string Password::getPasswordPattern(const std::string& username)
{
    return passwordPattern;
}

int Password::setMaximumCharacterOccurrences(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_maxCharOccurrences(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getMaximumCharacterOccurrences(const std::string& username)
{
    return maxCharOccurrences;
}

int Password::setMaximumNumericSequenceLength(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    if (auth_passwd_set_maxNumSeqLength(p_policy, value) != AUTH_PASSWD_API_SUCCESS) {
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

int Password::getMaximumNumericSequenceLength(const std::string& username)
{
    return maxNumSeqLength;
}

int Password::setForbiddenStrings(const std::string& username, const std::vector<std::string>& forbiddenStrings)
{
    int ret = 0;
    struct passwd *user_info;

    policy_h *p_policy;

    user_info = ::getpwnam(username.c_str());
    if (auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    if (auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    for (std::vector<std::string>::const_iterator iter = forbiddenStrings.begin(); iter != forbiddenStrings.end(); ++iter) {
        if (auth_passwd_set_forbiddenPasswd(p_policy, (*iter).c_str()) != AUTH_PASSWD_API_SUCCESS) {
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

std::vector<std::string> Password::getForbiddenStrings(const std::string& username)
{
    return forbiddenPasswds;
}

Password passwordPolicy(Server::instance());

} /* namespace DevicePolicyManager*/

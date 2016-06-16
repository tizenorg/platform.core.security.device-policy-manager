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
#include <bundle.h>
#include <aul.h>
#include <notification.h>
#include <vconf.h>
#include <system_settings.h>

#include "password.hxx"

#include "policy-helper.h"
#include "auth/user.h"
#include "audit/logger.h"

#define SIMPLE_PASSWORD_LENGTH 7
#define PEER_UID 5001
#define PASSWORD_EMPTY_STRING "\\n"

namespace DevicePolicyManager {

namespace {

PasswordPolicy::PasswordPolicyQuality qualityType;
std::string PasswordPattern;
std::vector<std::string> ForbiddenStrings;

int SetPasswordPolicy(PolicyControlContext &context, const std::string &name, const std::string &value)
{
    return context.updatePolicy(name, value, "password", name);
}

std::string GetPasswordPolicy(PolicyControlContext &context, const std::string &name)
{
    return context.getPolicy(name);
}

int transformValueFromIntToQualityType(const int quality, PasswordPolicy::PasswordPolicyQuality &changed_quality)
{
    switch (quality) {
    case PasswordPolicy::DPM_PASSWORD_QUALITY_UNSPECIFIED:
        changed_quality = PasswordPolicy::DPM_PASSWORD_QUALITY_UNSPECIFIED;
        break;
    case PasswordPolicy::DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD:
        changed_quality = PasswordPolicy::DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD;
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
        ERROR("unknown password quality");
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
    case PasswordPolicy::DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD:
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
        ERROR("unknown");
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

	ctxt.createNotification("password");
	if (std::stoi(GetPasswordPolicy(__context, "password-status")) == PasswordPolicy::DPM_PASSWORD_STATUS_CHANGE_REQUIRED) {
		const char *user_data[4] = {"app-id", "setting-locktype-efl", "caller", "DPM"};
		bundle *b = ::bundle_create();
		::bundle_add_str(b, "id", "password-enforce-change");
		::bundle_add_str_array(b, "user-data", user_data, 4);
		int ret = 0;
		while (1) {
			ret = ::aul_launch_app_for_uid("org.tizen.dpm-syspopup", b, PEER_UID);
			if (ret > 0)
				break;
		}
		::bundle_free(b);
	}
}

PasswordPolicy::~PasswordPolicy()
{
}

int PasswordPolicy::setPasswordPolicyQuality(const int quality)
{
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

    if (qualityType == PasswordPolicy::DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD) {
        if (auth_passwd_set_min_length(p_policy, SIMPLE_PASSWORD_LENGTH) != AUTH_PASSWD_API_SUCCESS) {
            auth_passwd_free_policy(p_policy);
            return -1;
        }
    }

    if (auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS) {
        auth_passwd_free_policy(p_policy);
        return -1;
    }

    auth_passwd_free_policy(p_policy);

	SetPasswordPolicy(__context, "password-quality", std::to_string(quality));
	if (qualityType == PasswordPolicy::DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD)
		SetPasswordPolicy(__context, "password-minimum-length", std::to_string(SIMPLE_PASSWORD_LENGTH));

    return 0;
}

int PasswordPolicy::getPasswordPolicyQuality()
{
    return std::stoi(GetPasswordPolicy(__context, "password-quality"));
}

int PasswordPolicy::setPasswordPolicyMinimumLength(const int value)
{
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

    return SetPasswordPolicy(__context, "password-minimum-length", std::to_string(value));
}

int PasswordPolicy::getPasswordPolicyMinimumLength()
{
    return std::stoi(GetPasswordPolicy(__context, "password-minimum-length"));
}

int PasswordPolicy::setMinPasswordPolicyComplexChars(const int value)
{
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

    return SetPasswordPolicy(__context, "password-minimum-complexity", std::to_string(value));
}

int PasswordPolicy::getMinPasswordPolicyComplexChars()
{
    return std::stoi(GetPasswordPolicy(__context, "password-minimum-complexity"));
}

int PasswordPolicy::setMaximumFailedPasswordPolicyForWipe(const int value)
{
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

    return SetPasswordPolicy(__context, "password-maximum-failure-count", std::to_string(value));
}

int PasswordPolicy::getMaximumFailedPasswordPolicyForWipe()
{
    return std::stoi(GetPasswordPolicy(__context, "password-maximum-failure-count"));
}

int PasswordPolicy::setPasswordPolicyExpires(const int value)
{
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

    return SetPasswordPolicy(__context, "password-expired", std::to_string(value));
}

int PasswordPolicy::getPasswordPolicyExpires()
{
    return std::stoi(GetPasswordPolicy(__context, "password-expired"));
}

int PasswordPolicy::setPasswordPolicyHistory(const int value)
{
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

    return SetPasswordPolicy(__context, "password-history", std::to_string(value));
}

int PasswordPolicy::getPasswordPolicyHistory()
{
    return std::stoi(GetPasswordPolicy(__context, "password-history"));
}

int PasswordPolicy::setPasswordPolicyPattern(const std::string &pattern)
{
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

    PasswordPattern = pattern;
    if (PasswordPattern.compare("") == 0)
        PasswordPattern = PASSWORD_EMPTY_STRING;

    return SetPasswordPolicy(__context, "password-pattern", PasswordPattern.c_str());
}

int PasswordPolicy::resetPasswordPolicy(const std::string &passwd)
{
    int ret = 0;

    if (auth_passwd_reset_passwd(AUTH_PWD_NORMAL, __context.getPeerUid(), passwd.c_str()) != AUTH_PASSWD_API_SUCCESS) {
        return -1;
    }

    return ret;
}

int PasswordPolicy::enforcePasswordPolicyChange()
{
	int ret = 0;
	const char *user_data[4] = {"app-id", "setting-locktype-efl", "caller", "DPM"};

	bundle *b = ::bundle_create();
	ERROR("enforce Password Policy Change");
	::bundle_add_str(b, "id", "password-enforce-change");
	::bundle_add_str_array(b, "user-data", user_data, 4);
	ret = ::aul_launch_app_for_uid("org.tizen.dpm-syspopup", b, __context.getPeerUid());
	::bundle_free(b);

	if (ret < 0) {
		ERROR("Failed to launch Password Application.");
		return -1;
	} else {
		return SetPasswordPolicy(__context, "password-status", std::to_string(PasswordPolicy::DPM_PASSWORD_STATUS_CHANGE_REQUIRED));
	}

}

int PasswordPolicy::setMaxInactivityTimeDeviceLock(const int value)
{
    return SetPasswordPolicy(__context, "password-inactivity-timeout", std::to_string(value));
}

int PasswordPolicy::getMaxInactivityTimeDeviceLock()
{
    return std::stoi(GetPasswordPolicy(__context, "password-inactivity-timeout"));
}

int PasswordPolicy::setPasswordPolicyStatus(const int status)
{
    int current_status = std::stoi(GetPasswordPolicy(__context, "password-status"));

    ERROR("current status: " + std::to_string(current_status) + ", status: " + std::to_string(status));
    if (status >= PasswordPolicy::DPM_PASSWORD_STATUS_MAX) {
        return -1;
    }

    if (status == PasswordPolicy::DPM_PASSWORD_STATUS_MAX_ATTEMPTS_EXCEEDED) {
        ERROR("Max Attempts Exceeded.");
        return -1;
    }

	if (current_status == PasswordPolicy::DPM_PASSWORD_STATUS_CHANGE_REQUIRED) {
		if (status == PasswordPolicy::DPM_PASSWORD_STATUS_CHANGED) {
			return SetPasswordPolicy(__context, "password-status", std::to_string(PasswordPolicy::DPM_PASSWORD_STATUS_NORMAL));
		} else if (status == PasswordPolicy::DPM_PASSWORD_STATUS_NOT_CHANGED) {
			ERROR("Not Changed");
			return 0;
		}
	} else if (current_status ==  PasswordPolicy::DPM_PASSWORD_STATUS_NORMAL) {
		if (status == PasswordPolicy::DPM_PASSWORD_STATUS_CHANGE_REQUIRED) {
			return SetPasswordPolicy(__context, "password-status", std::to_string(status));
		}
	}

    return -1;
}

int PasswordPolicy::deletePasswordPolicyPattern()
{
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

    PasswordPattern.clear();
    return SetPasswordPolicy(__context, "password-pattern", PASSWORD_EMPTY_STRING);
}

std::string PasswordPolicy::getPasswordPolicyPattern()
{
    PasswordPattern = GetPasswordPolicy(__context, "password-pattern");
    if (PasswordPattern.compare(PASSWORD_EMPTY_STRING) == 0)
        PasswordPattern = "";

    return PasswordPattern;
}

int PasswordPolicy::setMaximumCharacterOccurrences(const int value)
{
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

    return SetPasswordPolicy(__context, "password-maximum-character-occurrences", std::to_string(value));
}

int PasswordPolicy::getMaximumCharacterOccurrences()
{
    return std::stoi(GetPasswordPolicy(__context, "password-maximum-character-occurrences"));
}

int PasswordPolicy::setMaximumNumericSequenceLength(const int value)
{
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

    return SetPasswordPolicy(__context, "password-numeric-sequences-length", std::to_string(value));
}

int PasswordPolicy::getMaximumNumericSequenceLength()
{
    return std::stoi(GetPasswordPolicy(__context, "password-numeric-sequences-length"));
}

int PasswordPolicy::setForbiddenStrings(const std::vector<std::string> &forbiddenStrings)
{
    unsigned int count = 0;
    policy_h *p_policy;
    std::string xmlForbiddenStrings;

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

    std::copy(forbiddenStrings.begin(), forbiddenStrings.end(), std::back_inserter(ForbiddenStrings));
    for (std::vector<std::string>::const_iterator iter = forbiddenStrings.begin(); iter != forbiddenStrings.end(); ++iter) {
        xmlForbiddenStrings += (*iter).c_str();
        if (++count < forbiddenStrings.size())
            xmlForbiddenStrings += PASSWORD_EMPTY_STRING;
    }

    if (xmlForbiddenStrings.compare("") == 0) {
        xmlForbiddenStrings = PASSWORD_EMPTY_STRING;
    }

    return SetPasswordPolicy(__context, "password-forbidden-strings", xmlForbiddenStrings.c_str());
}

std::vector<std::string> PasswordPolicy::getForbiddenStrings()
{
    unsigned int nPos;
    std::string xmlForbiddenStrings;

    xmlForbiddenStrings = GetPasswordPolicy(__context, "password-forbidden-strings");
    ForbiddenStrings.clear();
    while ((nPos = xmlForbiddenStrings.find_first_of(PASSWORD_EMPTY_STRING)) != xmlForbiddenStrings.npos) {
        if (nPos > 0) {
            ForbiddenStrings.push_back(xmlForbiddenStrings.substr(0, nPos));
        }
        xmlForbiddenStrings = xmlForbiddenStrings.substr(nPos + 3);
    }
    if (xmlForbiddenStrings.length() > 0)
        ForbiddenStrings.push_back(xmlForbiddenStrings.substr(0, nPos));

    if ((*ForbiddenStrings.begin()).compare(PASSWORD_EMPTY_STRING) == 0)
        *ForbiddenStrings.begin() = "";

    return ForbiddenStrings;
}

PasswordPolicy passwordPolicy(Server::instance());

} /* namespace DevicePolicyManager*/

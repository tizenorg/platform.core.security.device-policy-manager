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
#include "audit/logger.hxx"


namespace DevicePolicyManager {

Password::Password(PolicyControlContext& ctxt)
    : context(ctxt)
{
    Ipc::Service& manager = context.getServiceManager();

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
	manager.registerParametricMethod(this, (std::string)(Password::getPasswordPolicy)(std::string));
	manager.registerParametricMethod(this, (int)(Password::setPasswordStatus)(std::string,int));
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

int Password::transformValueFromIntToQualityType(const int pwd_quality, PasswordQuality& changed_quality)
{
    switch (pwd_quality) {
        case DPM_PASSWORD_QUALITY_UNSPECIFIED:
			changed_quality = DPM_PASSWORD_QUALITY_UNSPECIFIED;
			break;
		case DPM_PASSWORD_QUALITY_SOMETHING:
			changed_quality = DPM_PASSWORD_QUALITY_SOMETHING;
            break;
        case DPM_PASSWORD_QUALITY_NUMERIC:
			changed_quality = DPM_PASSWORD_QUALITY_NUMERIC;
            break;
		case DPM_PASSWORD_QUALITY_ALPHABETIC:
			changed_quality = DPM_PASSWORD_QUALITY_ALPHABETIC;
            break;
		case DPM_PASSWORD_QUALITY_ALPHANUMERIC:
			changed_quality = DPM_PASSWORD_QUALITY_ALPHANUMERIC;
            break;
        default:
            std::cerr << "unknown" << std::endl;
			return -1;
			break;
    }

	return 0;
}

int Password::transformQualityFromDPMToAuth(const int dpm_quality, password_quality_type& auth_quality)
{
    switch (dpm_quality) {
        case DPM_PASSWORD_QUALITY_UNSPECIFIED:
			auth_quality = AUTH_PWD_QUALITY_UNSPECIFIED;
			break;
		case DPM_PASSWORD_QUALITY_SOMETHING:
			auth_quality = AUTH_PWD_QUALITY_SOMETHING;
            break;
        case DPM_PASSWORD_QUALITY_NUMERIC:
			auth_quality = AUTH_PWD_QUALITY_NUMERIC;
            break;
		case DPM_PASSWORD_QUALITY_ALPHABETIC:
			auth_quality = AUTH_PWD_QUALITY_ALPHABETIC;
            break;
		case DPM_PASSWORD_QUALITY_ALPHANUMERIC:
			auth_quality = AUTH_PWD_QUALITY_ALPHANUMERIC;
            break;
        default:
            std::cerr << "unknown" << std::endl;
			return -1;
			break;
    }

	return 0;
}

int Password::setPasswordQuality(const std::string& username, const int pwd_quality)
{
    int ret = 0;
	struct passwd *user_info;
	PasswordQuality changed_quality;
	password_quality_type auth_quality_type = AUTH_PWD_QUALITY_UNSPECIFIED;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	if(transformValueFromIntToQualityType(pwd_quality, changed_quality) != 0)
        return -1;
	
	this->qualityType = changed_quality;

	if(transformQualityFromDPMToAuth(changed_quality,auth_quality_type) != 0)
		return -1;

    if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
    if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
        return -1;
	
	if(auth_passwd_set_quality(p_policy, auth_quality_type) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
	    return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

int Password::getPasswordQuality(const std::string& username)
{
	int ret = 0;
	
	ret = this->qualityType;
		
	return ret;
}

int Password::setPasswordMinimumLength(const std::string& username, const int value)
{
    int ret = 0;
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->minLength = value;

	if(auth_passwd_new_policy(&p_policy)!= AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_min_length(p_policy, value) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;	
		
    ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

int Password::getPasswordMinimumLength(const std::string& username)
{
	int ret = 0;
		
	ret = this->minLength;
				
	return ret;
}

int Password::setMinPasswordComplexChars(const std::string& username, const int value)
{
    int ret = 0;    
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->minComplexCharNumber = value;

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_minComplexCharNumber(p_policy, value) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
	
    return ret;
}

int Password::getMinPasswordComplexChars(const std::string& username)
{
	int ret = 0;
			
	ret = this->minComplexCharNumber;
				
	return ret;
}

int Password::setMaximumFailedPasswordForWipe(const std::string& username, const int value)
{
    int ret = 0;
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->maxAttempts = value;

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
		
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_max_attempts(p_policy, value) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

int Password::getMaximumFailedPasswordForWipe(const std::string& username)
{
    int ret = 0;
			
	ret = this->maxAttempts;
					
	return ret;
}

int Password::setPasswordExpires(const std::string& username, const int value)
{
    int ret = 0;
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->validPeriod = value;

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_validity(p_policy, value) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

int Password::getPasswordExpires(const std::string& username)
{
	int ret = 0;
				
	ret = this->validPeriod;
					
	return ret;
}

int Password::setPasswordHistory(const std::string& username, const int value)
{
    int ret = 0;
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->historySize = value;

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_history_size(p_policy, value) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

int Password::getPasswordHistory(const std::string& username)
{
	int ret = 0;
					
	ret = this->historySize;
							
	return ret;
}

int Password::setPasswordPattern(const std::string& username, const std::string& pattern)
{
    int ret = 0;
    struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->pattern.append(pattern);

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_pattern(p_policy, pattern.c_str()) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
	
    return ret;
}

int Password::resetPassword(const std::string& username, const std::string& new_password)
{
    int ret = 0;
	struct passwd *user_info;

	user_info = ::getpwnam(username.c_str());

    this->password.append(new_password.c_str());
	if(auth_passwd_reset_passwd(AUTH_PWD_NORMAL,user_info->pw_uid,this->password.c_str()) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");
    
    return ret;
}

int Password::enforcePasswordChange(const std::string& username)
{
    int ret = 0;
	//struct passwd *user_info;

    //user_info = ::getpwnam(username.c_str());

	// Password change setting is launched.
    
    return ret;
}

int Password::setMaxInactivityTimeDeviceLock(const std::string& username, const int value)
{
    int ret = 0;
	//struct passwd *user_info;

    //user_info = ::getpwnam(username.c_str());

	this->maxInactivityTime = value;

	// Write the value to Policy DB.
    
    return ret;
}

int Password::getMaxInactivityTimeDeviceLock(const std::string& username)
{
    int ret = 0;

    ret = this->maxInactivityTime;
	
    return ret;
}

std::string Password::getPasswordPolicy(const std::string& username)
{
    std::string passwdPolicy("Policy");;
    
    return passwdPolicy;
}

int Password::setPasswordStatus(const std::string& username, const int status)
{
    int ret = 0;

	this->passwdStatus = status;
    
    return ret;
}

int Password::deletePasswordPattern(const std::string& username)
{
    int ret = 0;
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->pattern.clear();

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_pattern(p_policy, NULL) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

std::string Password::getPasswordPattern(const std::string& username)
{
    return this->pattern.c_str();
}

int Password::setMaximumCharacterOccurrences(const std::string& username, const int value)
{
    int ret = 0;
    struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->maxCharOccurrences = value;

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_maxCharOccurrences(p_policy, value) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
	
    return ret;
}

int Password::getMaximumCharacterOccurrences(const std::string& username)
{
    int ret = 0;	

	ret = this->maxCharOccurrences;
	
    return ret;
}

int Password::setMaximumNumericSequenceLength(const std::string& username, const int value)
{
    int ret = 0;
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	this->maxNumSeqLength = value;

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_maxNumSeqLength(p_policy, value) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

int Password::getMaximumNumericSequenceLength(const std::string& username)
{
    int ret = 0;

	ret = maxNumSeqLength;
	
    return ret;
}

int Password::setForbiddenStrings(const std::string& username, const std::vector<std::string>& forbiddenStrings)
{
    int ret = 0;
	struct passwd *user_info;

	policy_h* p_policy;

    user_info = ::getpwnam(username.c_str());

	std::copy(forbiddenStrings.begin(),forbiddenStrings.end(),std::back_inserter(this->forbiddenPasswds));

	if(auth_passwd_new_policy(&p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;
	
	if(auth_passwd_set_user(p_policy, user_info->pw_uid) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	for(std::vector<std::string>::const_iterator iter = forbiddenStrings.begin();iter!=forbiddenStrings.end();++iter)
	{
		if(auth_passwd_set_forbiddenPasswd(p_policy, (*iter).c_str()) != AUTH_PASSWD_API_SUCCESS)
			return -1;
		std::cout << "ForbiddenStrings: " << (*iter).c_str() << std::endl;
	}

	if(auth_passwd_set_policy(p_policy) != AUTH_PASSWD_API_SUCCESS)
		return -1;

	ERROR("[Password] AUTH_PASSWD_API_SUCCESS");

	auth_passwd_free_policy(p_policy);
    
    return ret;
}

std::vector<std::string> Password::getForbiddenStrings(const std::string& username)
{
    return this->forbiddenPasswds;
}

Password passwordPolicy(DevicePolicyServer::Server::instance());

} // namespace DevicePolicyManager

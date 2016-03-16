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

#include <string>
#include "policy-context.hxx"
#include "reflection.hxx"
#include <auth-passwd-admin.h>

namespace DevicePolicyManager {

class Password {
public:
	typedef enum {
		DPM_PASSWORD_QUALITY_UNSPECIFIED     = 0x00,	/**< No requirements for password. */
		DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD = 0x01,	/**< Eas requirement for simple password */
		DPM_PASSWORD_QUALITY_SOMETHING       = 0x10,	/**< Some kind password is required, but doesn't care what it is */
		DPM_PASSWORD_QUALITY_NUMERIC         = 0x20,	/**< Containing at least numeric characters */
		DPM_PASSWORD_QUALITY_ALPHABETIC      = 0x40,	/**< Containing at least alphabetic (or other symbol) characters */
		DPM_PASSWORD_QUALITY_ALPHANUMERIC    = 0x80,	/**< Containing at least numeric and alphabetic characters */
	} PasswordQuality;
	
    Password(PolicyControlContext& ctxt);
    ~Password();

    int transformValueFromIntToQualityType(const int pwd_quality, PasswordQuality& changed_quality);
	int transformQualityFromDPMToAuth(const int dpm_quality, password_quality_type& auth_quality);
	int setPasswordQuality(const std::string& username, const int pwd_quality);
	int getPasswordQuality(const std::string& username);
	int setPasswordMinimumLength(const std::string& username, const int value);
	int getPasswordMinimumLength(const std::string& username);
	int setMinPasswordComplexChars(const std::string& username, const int value);
	int getMinPasswordComplexChars(const std::string& username);
	int setMaximumFailedPasswordForWipe(const std::string& username, const int value);
	int getMaximumFailedPasswordForWipe(const std::string& username);
	int setPasswordExpires(const std::string& username, const int value);
	int getPasswordExpires(const std::string& username);
	int setPasswordHistory(const std::string& username, const int value);
	int getPasswordHistory(const std::string& username);
	int setPasswordPattern(const std::string& username, const std::string& pattern);
	int resetPassword(const std::string& username, const std::string& new_password);
	int enforcePasswordChange(const std::string& username);
	int setMaxInactivityTimeDeviceLock(const std::string& username, const int value);
	int getMaxInactivityTimeDeviceLock(const std::string& username);
	std::string getPasswordPolicy(const std::string& username);
	int setPasswordStatus(const std::string& username, const int status);
	int deletePasswordPattern(const std::string& username);
	std::string getPasswordPattern(const std::string& username);
	int setMaximumCharacterOccurrences(const std::string& username, const int value);
	int getMaximumCharacterOccurrences(const std::string& username);
	int setMaximumNumericSequenceLength(const std::string& username, const int value);
	int getMaximumNumericSequenceLength(const std::string& username);
	int setForbiddenStrings(const std::string& username, const std::vector<std::string>& forbiddenPasswds);
	std::vector<std::string> getForbiddenStrings(const std::string& username);
    
private:
    PolicyControlContext& context;

	PasswordQuality qualityType;
	unsigned int minLength;
	unsigned int minComplexCharNumber;
	unsigned int maxAttempts;
	unsigned int validPeriod;
	unsigned int historySize;
	std::string pattern;
	std::string password;
	unsigned int maxInactivityTime;
	unsigned int passwdStatus;
	unsigned int maxCharOccurrences;
	unsigned int maxNumSeqLength;
	std::vector<std::string> forbiddenPasswds;	
};

} // namespace DevicePolicyManager
#endif // __PASSWORD_POLICY__

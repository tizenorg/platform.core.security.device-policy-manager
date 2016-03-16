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

#include <dpm/password.h>

#include "dpm.hxx"
#include "password.hxx"

using namespace DevicePolicyManager;

static void dpm_free_password(void* data)
{
    dpm_data_type *dpm_data = (dpm_data_type*)data;

	if(data != NULL)
	{
	    switch(dpm_data->mem_header.id) {
			case DPM_GET_PASSWORD_POLOCY:
                g_free(dpm_data->data);
				break;
			case DPM_GET_PASSWORD_PATTERN:
                g_free(dpm_data->data);
				break;
			case DPM_GET_PASSWORD_FORBIDDEN_STRINGS: {
                GList *current = (GList *)(dpm_data->data);
				if(current) {
					g_list_foreach((GList *)current, (GFunc)g_free, NULL);
					g_list_free((GList *)current);
				}
				}
				break;
			default:
				g_free(dpm_data->data);
	    }
	}	
}

int dpm_set_password_quality(dpm_client_h handle, const char* p_username, dpm_password_quality_e pwd_quality)
{
    int ret = 0;
	std::string username(p_username);	
    
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setPasswordQuality(p_username, pwd_quality) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
    return ret;
}

int dpm_set_password_minimum_length(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
	std::string username(p_username);	
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setPasswordMinimumLength(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_min_password_complex_chars(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
	std::string username(p_username);	
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setMinPasswordComplexChars(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_maximum_failed_password_for_wipe(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
    std::string username(p_username);	
		
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setMaximumFailedPasswordForWipe(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_password_expires(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
    std::string username(p_username);	
		
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setPasswordExpires(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_password_history(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
	std::string username(p_username);	
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setPasswordHistory(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_password_pattern(dpm_client_h handle, const char* p_username, const char* p_pattern)
{
    int ret = 0;
    std::string username(p_username);
	std::string pattern(p_pattern);
		
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setPasswordPattern(username, pattern) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_reset_password(dpm_client_h handle, const char* p_username, const char* p_password)
{
    int ret = 0;
	std::string username(p_username);
	std::string new_password(p_password);
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.resetPassword(username, new_password) == 0)
	    ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_enforce_password_change(dpm_client_h handle, const char* p_username)
{
    int ret = 0;
	std::string username(p_username);	
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.enforcePasswordChange(username) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_max_inactivity_time_device_lock(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
    std::string username(p_username);	
		
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setMaxInactivityTimeDeviceLock(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_get_max_inactivity_time_device_lock(dpm_client_h handle, const char* p_username, int* p_value)
{
    int ret = 0;
	std::string username(p_username);			
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

    *p_value = password.getMaxInactivityTimeDeviceLock(username);

	if(*p_value > 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

dpm_data_type* dpm_get_password_policy(dpm_client_h handle, const char* p_username)
{
    dpm_data_type* dpm_data = NULL;
	dpm_password_policy_type* dpmPasswdPolicy = NULL;
	GList *p_list = NULL;
	std::string username(p_username);			
		
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	dpm_data = (dpm_data_type*)g_malloc0(sizeof(dpm_data_type));
    dpm_data->mem_header.id = DPM_GET_PASSWORD_POLOCY;
	dpm_data->mem_header.free_func = dpm_free_password;

    dpmPasswdPolicy = (dpm_password_policy_type*)g_malloc0(sizeof(dpm_password_policy_type));
	dpmPasswdPolicy->qualityType = password.getPasswordQuality(username);
	dpmPasswdPolicy->minLength = password.getPasswordMinimumLength(username);
	dpmPasswdPolicy->minComplexCharNumber = password.getMinPasswordComplexChars(username);
	dpmPasswdPolicy->maxAttempts = password.getMaximumFailedPasswordForWipe(username);
	dpmPasswdPolicy->validPeriod = password.getPasswordExpires(username);
	dpmPasswdPolicy->historySize = password.getPasswordHistory(username);
	dpmPasswdPolicy->maxInactivityTime = password.getMaxInactivityTimeDeviceLock(username);
	dpmPasswdPolicy->maxCharOccurrences = password.getMaximumCharacterOccurrences(username);
	dpmPasswdPolicy->maxNumSeqLength = password.getMaximumNumericSequenceLength(username);	
    dpmPasswdPolicy->pattern = (char*) g_strdup(password.getPasswordPattern(username).c_str());
	
	std::vector<std::string> forbiddenStrings = password.getForbiddenStrings(username);
	for(std::vector<std::string>::const_iterator iter = forbiddenStrings.begin();iter!=forbiddenStrings.end();++iter)
	{
		p_list = g_list_append(p_list, g_strdup((*iter).c_str()));		
	}
	dpmPasswdPolicy->forbiddenStringList = p_list;

    dpm_data->data = (void*)dpmPasswdPolicy;
	dpm_data->ret = DPM_ERROR_NONE;
		
    return dpm_data;
}

int dpm_set_password_status(dpm_client_h handle, const char* p_username, const int status)
{
    int ret = 0;
    std::string username(p_username);
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setPasswordStatus(username, status) == 0)
	    ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_delete_password_pattern(dpm_client_h handle, const char* p_username)
{
    int ret = 0;
    std::string username(p_username);	
		
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.deletePasswordPattern(username) == 0)
	    ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

dpm_data_type* dpm_get_password_pattern(dpm_client_h handle, const char* p_username)
{
    dpm_data_type* dpm_data = NULL;
	std::string username(p_username);
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	dpm_data = (dpm_data_type*)g_malloc0(sizeof(dpm_data_type));
    dpm_data->mem_header.id = DPM_GET_PASSWORD_PATTERN;
	dpm_data->mem_header.free_func = dpm_free_password;
	if(password.getPasswordPattern(username).size() > 0)
	{
		dpm_data->ret = DPM_ERROR_NONE;
		dpm_data->data = (void*) g_strdup(password.getPasswordPattern(username).c_str());
	}
	else
		dpm_data->ret = DPM_ERROR_NOT_SUPPORTED;
	
    return dpm_data;
}

int dpm_set_maximum_character_occurrences(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
	std::string username(p_username);
    
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setMaximumCharacterOccurrences(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_get_maximum_character_occurrences(dpm_client_h handle, const char* p_username, int* p_value)
{
    int ret = 0;
    std::string username(p_username);			
		
	DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();
	*p_value = password.getMaximumCharacterOccurrences(username);

	if(*p_value > 0)
    	ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_maximum_numeric_sequence_length(dpm_client_h handle, const char* p_username, const int value)
{
    int ret = 0;
    std::string username(p_username);
		
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setMaximumNumericSequenceLength(username, value) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_get_maximum_numeric_sequence_length(dpm_client_h handle, const char* p_username, int* p_value)
{
    int ret = 0;
	std::string username(p_username);			
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();
	*p_value = password.getMaximumNumericSequenceLength(username);

	if(*p_value > 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

int dpm_set_forbidden_strings(dpm_client_h handle, const char* p_username, const GList* p_string_list)
{
    int ret = 0;
	std::string username(p_username);
    std::vector<std::string> forbiddenStrings;

	const GList* p_list = p_string_list;
	
	while(p_list != NULL)
	{
	    forbiddenStrings.push_back((const char*)(p_list->data));
        p_list = p_list->next;
	}		

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

	if(password.setForbiddenStrings(username, forbiddenStrings) == 0)
		ret = DPM_ERROR_NONE;
	else
		ret = DPM_ERROR_NOT_SUPPORTED;
	
	return ret;
}

dpm_data_type* dpm_get_forbidden_strings(dpm_client_h handle, const char* p_username)
{
    dpm_data_type* dpm_data = NULL;
	GList* p_list = NULL;
	std::string username(p_username);	
	
    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Password password = client.createPolicyInterface<Password>();

    std::vector<std::string> forbiddenStrings = password.getForbiddenStrings(username);
	for(std::vector<std::string>::const_iterator iter = forbiddenStrings.begin();iter!=forbiddenStrings.end();++iter)
	{
		p_list = g_list_append(p_list, g_strdup((*iter).c_str()));		
	}
	dpm_data = (dpm_data_type*)g_malloc0(sizeof(dpm_data_type));
    dpm_data->mem_header.id = DPM_GET_PASSWORD_FORBIDDEN_STRINGS;
	dpm_data->mem_header.free_func = dpm_free_password;
	if(p_list != NULL)
	{
		dpm_data->ret = DPM_ERROR_NONE;
		dpm_data->data = (void*)p_list;
	}
	else
		dpm_data->ret = DPM_ERROR_NOT_SUPPORTED;
	
    return dpm_data;
}


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

#include <cstring>
#include <cassert>

#include "password.h"
#include "password.hxx"

#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_password_policy_h dpm_context_acquire_password_policy(dpm_context_h handle, const char* zone)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<PasswordPolicy>();
}

DPM_API int dpm_context_release_password_policy(dpm_password_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<PasswordPolicy>(handle);
    return 0;
}

int dpm_password_set_quality(dpm_password_policy_h handle, dpm_password_quality_e quality)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setPasswordPolicyQuality(quality) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_minimum_length(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setPasswordPolicyMinimumLength(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_min_complex_chars(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setMinPasswordPolicyComplexChars(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_maximum_failed_attempts_for_wipe(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setMaximumFailedPasswordPolicyForWipe(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_expires(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setPasswordPolicyExpires(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_history(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setPasswordPolicyHistory(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_pattern(dpm_password_policy_h handle, const char *pattern)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pattern, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setPasswordPolicyPattern(pattern) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_reset(dpm_password_policy_h handle, const char *passwd)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(passwd, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.resetPasswordPolicy(passwd) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_enforce_change(dpm_password_policy_h handle)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.enforcePasswordPolicyChange() == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_max_inactivity_time_device_lock(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setMaxInactivityTimeDeviceLock(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_max_inactivity_time_device_lock(dpm_password_policy_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    *value = password.getMaxInactivityTimeDeviceLock();

    if (*value > 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_status(dpm_password_policy_h handle, const int status)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setPasswordPolicyStatus(status) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_delete_pattern(dpm_password_policy_h handle)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.deletePasswordPolicyPattern() == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_pattern(dpm_password_policy_h handle, char* pattern)
{
    int ret = 0;
	
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
	RET_ON_FAILURE(pattern, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.getPasswordPolicyPattern().size() > 0) {
        pattern = ::strdup(password.getPasswordPolicyPattern().c_str());
		ret = DPM_ERROR_NONE;
    }
	else
		ret = DPM_ERROR_TIMED_OUT;

    return ret;
}

int dpm_password_set_maximum_character_occurrences(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setMaximumCharacterOccurrences(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_maximum_character_occurrences(dpm_password_policy_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);
    *value = password.getMaximumCharacterOccurrences();

    if (*value > 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_maximum_numeric_sequence_length(dpm_password_policy_h handle, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);

    if (password.setMaximumNumericSequenceLength(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_maximum_numeric_sequence_length(dpm_password_policy_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    PasswordPolicy& password = GetPolicyInterface<PasswordPolicy>(handle);
    *value = password.getMaximumNumericSequenceLength();

    if (*value > 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

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
    return client.createPolicyInterface<Password>();
}

DPM_API int dpm_context_release_password_policy(dpm_password_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<Password>(handle);
    return 0;
}

int dpm_set_password_quality(dpm_password_policy_h handle, const char *username, dpm_password_quality_e quality)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setPasswordQuality(username, quality) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_password_minimum_length(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setPasswordMinimumLength(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_min_password_complex_chars(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setMinPasswordComplexChars(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_maximum_failed_password_for_wipe(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setMaximumFailedPasswordForWipe(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_password_expires(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setPasswordExpires(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_password_history(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setPasswordHistory(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_password_pattern(dpm_password_policy_h handle, const char *username, const char *pattern)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pattern, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setPasswordPattern(username, pattern) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_reset_password(dpm_password_policy_h handle, const char *username, const char *passwd)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(passwd, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.resetPassword(username, passwd) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_enforce_password_change(dpm_password_policy_h handle, const char *username)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.enforcePasswordChange(username) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_max_inactivity_time_device_lock(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setMaxInactivityTimeDeviceLock(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_get_max_inactivity_time_device_lock(dpm_password_policy_h handle, const char *username, int *p_value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(p_value, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    *p_value = password.getMaxInactivityTimeDeviceLock(username);

    if (*p_value > 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_password_status(dpm_password_policy_h handle, const char *username, const int status)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setPasswordStatus(username, status) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_delete_password_pattern(dpm_password_policy_h handle, const char *username)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.deletePasswordPattern(username) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

char *dpm_get_password_pattern(dpm_password_policy_h handle, const char *username)
{
    RET_ON_FAILURE(handle, NULL);
    RET_ON_FAILURE(username, NULL);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.getPasswordPattern(username).size() > 0) {
        return ::strdup(password.getPasswordPattern(username).c_str());
    }

    return NULL;
}

int dpm_set_maximum_character_occurrences(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setMaximumCharacterOccurrences(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_get_maximum_character_occurrences(dpm_password_policy_h handle, const char *username, int *p_value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(p_value, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);
    *p_value = password.getMaximumCharacterOccurrences(username);

    if (*p_value > 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_set_maximum_numeric_sequence_length(dpm_password_policy_h handle, const char *username, const int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);

    if (password.setMaximumNumericSequenceLength(username, value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_get_maximum_numeric_sequence_length(dpm_password_policy_h handle, const char *username, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(username, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    Password& password = GetPolicyInterface<Password>(handle);
    *value = password.getMaximumNumericSequenceLength(username);

    if (*value > 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

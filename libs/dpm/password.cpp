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

#include "array.h"
#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_password_set_quality(device_policy_manager_h handle, dpm_password_quality_e quality)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setPasswordPolicyQuality(quality) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_quality(device_policy_manager_h handle, dpm_password_quality_e *quality)
{
    int ret = 0;
    int password_quality = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    password_quality = password.getPasswordPolicyQuality();
    switch (password_quality) {
    case DPM_PASSWORD_QUALITY_UNSPECIFIED:
        *quality = DPM_PASSWORD_QUALITY_UNSPECIFIED;
        break;
    case DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD:
        *quality = DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD;
        break;
    case DPM_PASSWORD_QUALITY_SOMETHING:
        *quality = DPM_PASSWORD_QUALITY_SOMETHING;
        break;
    case DPM_PASSWORD_QUALITY_NUMERIC:
        *quality = DPM_PASSWORD_QUALITY_NUMERIC;
        break;
    case DPM_PASSWORD_QUALITY_ALPHABETIC:
        *quality = DPM_PASSWORD_QUALITY_ALPHABETIC;
        break;
    case DPM_PASSWORD_QUALITY_ALPHANUMERIC:
        *quality = DPM_PASSWORD_QUALITY_ALPHANUMERIC;
        break;
    default:
        *quality = DPM_PASSWORD_QUALITY_UNSPECIFIED;
        break;
    }

    if (*quality >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_minimum_length(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setPasswordPolicyMinimumLength(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_minimum_length(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getPasswordPolicyMinimumLength();
    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_min_complex_chars(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setMinPasswordPolicyComplexChars(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_min_complex_chars(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getMinPasswordPolicyComplexChars();
    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_maximum_failed_attempts_for_wipe(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setMaximumFailedPasswordPolicyForWipe(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_maximum_failed_attempts_for_wipe(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getMaximumFailedPasswordPolicyForWipe();
    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_expires(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setPasswordPolicyExpires(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_expires(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getPasswordPolicyExpires();
    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_history(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setPasswordPolicyHistory(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_history(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getPasswordPolicyHistory();
    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_pattern(device_policy_manager_h handle, const char *pattern)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pattern, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setPasswordPolicyPattern(pattern) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_reset(device_policy_manager_h handle, const char *passwd)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(passwd, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.resetPasswordPolicy(passwd) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_enforce_change(device_policy_manager_h handle)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.enforcePasswordPolicyChange() == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_max_inactivity_time_device_lock(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setMaxInactivityTimeDeviceLock(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_max_inactivity_time_device_lock(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getMaxInactivityTimeDeviceLock();

    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_status(device_policy_manager_h handle, dpm_password_status_e status)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setPasswordPolicyStatus(status) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_delete_pattern(device_policy_manager_h handle)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.deletePasswordPolicyPattern() == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_pattern(device_policy_manager_h handle, char **pattern)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(*pattern, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *pattern = ::strdup(password.getPasswordPolicyPattern().c_str());

    return ret;
}

int dpm_password_set_maximum_character_occurrences(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setMaximumCharacterOccurrences(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_maximum_character_occurrences(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getMaximumCharacterOccurrences();
    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_set_maximum_numeric_sequence_length(device_policy_manager_h handle, int value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    if (password.setMaximumNumericSequenceLength(value) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

int dpm_password_get_maximum_numeric_sequence_length(device_policy_manager_h handle, int *value)
{
    int ret = 0;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(value, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    *value = password.getMaximumNumericSequenceLength();
    if (*value >= 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

typedef runtime::Array<std::string> dpm_password_iterator;

dpm_password_iterator_h dpm_password_create_iterator(device_policy_manager_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    return reinterpret_cast<dpm_password_iterator_h>(new dpm_password_iterator(password.getForbiddenStrings()));
}

int dpm_password_iterator_next(dpm_password_iterator_h iter, const char **result)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    dpm_password_iterator *it = reinterpret_cast<dpm_password_iterator *>(iter);

    if (it->isEnd())
        *result = NULL;
    else
        *result = it->next()->c_str();

    return 0;
}

int dpm_password_destroy_iterator(dpm_password_iterator_h iter)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<dpm_password_iterator *>(iter);

    return 0;
}

int dpm_password_set_forbidden_strings(device_policy_manager_h handle, const char *strings[], int length)
{
    int iter;
    int ret = 0;
    std::vector<std::string> forbiddenStrings;

    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(strings, DPM_ERROR_INVALID_PARAMETER);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    PasswordPolicy password = client.createPolicyInterface<PasswordPolicy>();

    for (iter = 0; iter < length; iter++)
        forbiddenStrings.push_back(strings[iter]);
    if (password.setForbiddenStrings(forbiddenStrings) == 0)
        ret = DPM_ERROR_NONE;
    else
        ret = DPM_ERROR_NOT_SUPPORTED;

    return ret;
}

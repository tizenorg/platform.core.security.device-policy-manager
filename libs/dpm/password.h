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

#ifndef __CAPI_PASSWORD_POLICY_H__
#define __CAPI_PASSWORD_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file password.h
 * @brief This file provides APIs to control password functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_PASSWORD_POLICY_MODULE
 * @{
 */

/**
 * @brief       Enumeration for dpm password quality type
 * @since_tizen 3.0
 */
typedef enum {
    DPM_PASSWORD_QUALITY_UNSPECIFIED     = 0x00,    /**< No requirements for password. */
    DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD = 0x01,    /**< EAS(Exchanage ActiveSync) requirement for simple password */
    DPM_PASSWORD_QUALITY_SOMETHING       = 0x10,    /**< Some kind password is required, but doesn't care what it is */
    DPM_PASSWORD_QUALITY_NUMERIC         = 0x20,    /**< Containing at least numeric characters */
    DPM_PASSWORD_QUALITY_ALPHABETIC      = 0x40,    /**< Containing at least alphabetic (or other symbol) characters */
    DPM_PASSWORD_QUALITY_ALPHANUMERIC    = 0x80,    /**< Containing at least numeric and alphabetic characters */
} dpm_password_quality_e;

/**
 * @brief       Sets password quality.
 * @details     An administrator can set the password restrictions it is imposing.
 *              After setting this, the user will not be able to
 *              enter a new password that is not at least as restrictive as what has been set.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   quality Password Quality Type
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_quality(dpm_client_h handle, const char *username, dpm_password_quality_e quality);

/**
 * @brief       Sets password minimum length.
 * @details     Sets the minimum allowed password length. After setting this,
 *              the user will not be able to enter a new password that is
 *              shorter than the setting length.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Allowed minimum password length
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_minimum_length(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Sets minimum complex char in password.
 * @details     Complex characters are all non-alphabetic characters;
 *              that is, numbers and symbols. Admin can configure this
 *              setting and make the password more secure.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Number of minimum complex char in password.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_min_password_complex_chars(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Sets maximum number of failed attempts before
 *              device is wiped.
 * @details     If user fails the last attempt, device will be wiped.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Maximum count for failed passwords.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_maximum_failed_password_for_wipe(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Sets the number of days password expires.
 * @details     An administrator can configure the password age to force
 *              the user to enter a new password after every expiration period.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Number of days after which the password expires.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_expires(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Sets the number of min password history to avoid
 *              previous password.
 * @details     An administrator can configure the number of previous
 *              passwords which cannot be used when entering a new password.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Number of previous passwords which cannot be used when
 *              settings a new password.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_history(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Sets the required password pattern.
 * @details     An administrator can force User to enter password based on
 *              a regular expression.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   pattern Password Pattern. If regular expression is
 *              [a-zA-Z]{4}[0-9]{4}, we can force user to enter a 8 character
 *              password with first 4 alphabetic characters and next 4
 *              numeric characters. An administrator must take care when
 *              setting this pattern.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_pattern(dpm_client_h handle, const char *username, const char *pattern);

/**
 * @brief       Resets password.
 * @details     This takes effect immediately to the device password.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   password New Password
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_reset_password(dpm_client_h handle, const char *username, const char *password);

/**
 * @brief       Enforces password change.
 * @details     An administrator can enforce password change. Password
 *              change setting is launched.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_enforce_password_change(dpm_client_h handle, const char *username);

/**
 * @brief       Sets the maximum number of seconds of inactivity time
 *              before the screen timeout occurs.
 * @details     An administrator sets the maximum number of seconds of inactivity
 *              time before the screen timeout occurs and a device user must
 *              type the password to unlock the device.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Maximum inactivity time for device lock. Specifies how soon
 *              the device can be unlocked again after use, without reprompting for
 *              the passcode.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_max_inactivity_time_device_lock(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Gets the maximum number of seconds of inactivity time
 *              before the screen timeout occurs.
 * @details     Called by an application that is managing the device to get
 *              the value of timeout period.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[out]   value Pointer of Maximum inactivity time for device lock.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_get_max_inactivity_time_device_lock(dpm_client_h handle, const char *username, int *value);

/**
 * @brief       Sets password status
 * @details     An administrator can know password status for this API.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   status Password Status
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_status(dpm_client_h handle, const char *username, const int status);

/**
 * @brief       Removes all password patterns.
 * @details     An administrator can remove all password patterns.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_delete_password_pattern(dpm_client_h handle, const char *username);

/**
 * @brief       Gets password pattern.
 * @details     This API can be used for applying complexity on new password value.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @return      Password Pattern data, otherwise a null pointer if there is no password pattern.
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API char *dpm_get_password_pattern(dpm_client_h handle, const char *username);

/**
 * @brief       Sets the maximum number of times a character can occur in
 *              the device password.
 * @details     Called by an admin that is managing the device to specify that
 *              any character in the device password cannot occur more than
 *              the specified maximum number of times. Characters can be numeric
 *              or alphabetic or symbolic. "aaabcde" has 'a' which occurs 3 times,
 *              "1b1c1de" has '1' which occurs 3 times and "a@b@c@" has '@' which
 *              occurs 3 times. A value of '0' specifies that no restrictions are
 *              applied
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Maximum Character Occurences
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_maximum_character_occurrences(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Gets the maximum number of times a character can occur in
 *              the device password.
 * @details     An administrator can retrieve the maximum number of times
 *              a character can occur in the device password. If more than
 *              one admin has set this value then the least value will take
 *              preference.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[out]   value Pointer of Maximum Character Occurences
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_get_maximum_character_occurrences(dpm_client_h handle, const char *username, int *value);

/**
 * @brief       Sets the maximum length of the numeric sequence
 *              which is allowed in the device password.
 * @details     Called by an administrator that is managing the device to set
 *              the maximum numeric sequence length. This specifies that
 *              the device password must not contain numeric sequences greater
 *              than the given length.
 *              Numeric sequences can be increasing successively by one like
 *              "12345", or decreasing successively by one like "98765", or
 *              repeating like "55555".
 *              These are all numeric sequences of length '5'.
 *              If maximum value is set to '5' then "123456" or "987654" or
 *              "555555" are not allowed.
 *              A value of '0' specifies that no such numeric sequence
 *              restrictions are applied.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[in]   value Maximum Numeric Sequence Length
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_maximum_numeric_sequence_length(dpm_client_h handle, const char *username, const int value);

/**
 * @brief       Gets the maximum numeric sequence length allowed in
 *              the device password.
 * @details     An administrator can retrieve the length of numeric sequences
 *              which are allowed in the device password.
 *              For instance, if the return value is '3' then "123", "987",
 *              "555" would all be numeric sequences of length '3' and will be
 *              allowed in the device password.
 *              If more than one admin has set this value then the least value
 *              will take preference.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   username User name
 * @param[out]   value Pointer of Maximum Numeric Sequence Length
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_get_maximum_numeric_sequence_length(dpm_client_h handle, const char *username, int *value);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*  __CAPI_PASSWORD_POLICY_H__ */

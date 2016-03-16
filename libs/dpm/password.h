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

#ifndef __CAPI_PASSWORD_POLICY__
#define __CAPI_PASSWORD_POLICY__

#include <dpm/device-policy-client.h>
#include <glib.h>

/**
 * @file password.h
 * @brief This file provides APIs to control password functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  DPM_PASSWORD_POLICY
 * @{
 */

/**
 * @brief       Enumeration for memory header id type
 * @since_tizen 3.0
 */
typedef enum {
    DPM_GET_PASSWORD_POLOCY = 0x00,
	DPM_GET_PASSWORD_PATTERN  = 0x01,
	DPM_GET_PASSWORD_FORBIDDEN_STRINGS = 0x10,
} mem_header_id_t;

/**
 * @brief       Structure for memory header type
 * @since_tizen 3.0
 */
typedef struct _mem_header {
	mem_header_id_t id;					/**< Internal id */
	void (* free_func)(void *block);	/**< Pointer function used to free a block of memory */
} mem_header_t;

/**
 * @brief       Structure for dpm data type
 * @since_tizen 3.0
 */
typedef struct _dpm_data {
	int ret;			/**< dpm result*/ 
	mem_header_t mem_header;	/**< Header structure*/
	void *data;					/**< data*/
} dpm_data_type;

/**
 * @brief       Enumeration for dpm password quality type
 * @since_tizen 3.0
 */
typedef enum {
	DPM_PASSWORD_QUALITY_UNSPECIFIED     = 0x00,	/**< No requirements for password. */
	DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD = 0x01,	/**< Eas requirement for simple password */
	DPM_PASSWORD_QUALITY_SOMETHING       = 0x10,	/**< Some kind password is required, but doesn't care what it is */
	DPM_PASSWORD_QUALITY_NUMERIC         = 0x20,	/**< Containing at least numeric characters */
	DPM_PASSWORD_QUALITY_ALPHABETIC      = 0x40,	/**< Containing at least alphabetic (or other symbol) characters */
	DPM_PASSWORD_QUALITY_ALPHANUMERIC    = 0x80,	/**< Containing at least numeric and alphabetic characters */
} dpm_password_quality_e;

/**
 * @brief       Structure for dpm password policy type
 * @since_tizen 3.0
 */
typedef struct _dpm_password_policy {
    int qualityType;
	int minLength;
	int minComplexCharNumber;
	int maxAttempts;
	int validPeriod;
	int historySize;	
	int maxInactivityTime;
	int maxCharOccurrences;
	int maxNumSeqLength;
	char* pattern;
	void* forbiddenStringList;
} dpm_password_policy_type;


/**
 * @brief       API to set password quality.
 * @details     An administrator can set the password restrictions it is imposing. After setting this, the user will not be able to 
 *              enter a new password that is not at least as restrictive as what has been set.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   DPM Password Quality Type
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_quality(dpm_client_h handle, const char* p_username, dpm_password_quality_e pwd_quality);

/**
 * @brief       API to set password minimum length.
 * @details     set the minimum allowed password length. After setting this, the user will not be able to enter 
 *              a new password that is shorter than the setting length.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Allowed minimum password length
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_minimum_length(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to set minimum complex char in password.
 * @details     Complex characters are all non-alphabetic characters; that is, numbers and symbols. Admin can configure  
 *              this setting and make the password more secure.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Num of minimum complex char in password.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_min_password_complex_chars(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to set maximum number of failed attempts before device is wiped.
 * @details     If user fails the last attempt, device will be wiped.  
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Maximum count for failed passwords.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_maximum_failed_password_for_wipe(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to set the number of days password expires.
 * @details     An administrator can configure the password age to force the user to enter a new password after every expiration period.  
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Num of days after which the password expires.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_expires(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to set the number of min password history to avoid previous password.
 * @details     An administrator can configure the number of previous passwords which cannot be used when entering a new password.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Num of previous passwords which cannot be used when settings a new password.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_history(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to set the required password pattern.
 * @details     An administrator can force User to enter password based on a regular expression.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Password Pattern. If regular expression is [a-zA-Z]{4}[0-9]{4},
 *            we can force user to enter a 8 character password with first 4 alphabetic characters and
 *            next 4 numeric characters. An administrator must take care when setting this pattern. 
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_pattern(dpm_client_h handle, const char* p_username, const char* p_pattern);

/**
 * @brief       API to reset password.
 * @details     This takes effect immediately to the device password.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   New Password
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_reset_password(dpm_client_h handle, const char* p_username, const char* p_password);

/**
 * @brief       API to enforce password change.
 * @details     An administrator can enforce password change. Password change setting is launched.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_enforce_password_change(dpm_client_h handle, const char* p_username);

/**
 * @brief       API to set the maximum number of seconds of inactivity time before the screen timeout occurs.
 * @details     An administrator sets the maximum number of seconds of inactivity time before the screen timeout occurs and 
 *                 a device user must type the password to unlock the device.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Maximum inactivity time for device lock. Specifies how soon the device can be unlocked again after use, without reprompting for the passcode.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_max_inactivity_time_device_lock(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to get the maximum number of seconds of inactivity time before the screen timeout occurs.
 * @details     Called by an application that is managing the device to get the value of timeout period. 
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Pointer of Maximum inactivity time for device lock. 
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_get_max_inactivity_time_device_lock(dpm_client_h handle, const char* p_username, int* p_value);

/**
 * @brief       API to get password policy.
 * @details     The policy includes all password policy such as minimum length or expire time or password quality. 
 * @since_tizen 3.0
 * @code
 *    dpm_data_type* dpm_data;
 *    dpm_password_policy_type* passwdPolicy;
 *    dpm_data = dpm_get_password_policy(handle, "owner");
 *    if (dpm_data != NULL) {
 *        passwdPolicy = (dpm_password_policy_type*)dpm_data->data;
 *        dpm_data->mem_header.free_func(dpm_data);
 *    }
 *
 * @endcode
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @return      DPM Data Structure
 * @retval      ret: DPM ERROR TYPE
 * @retval      mem_header: Header type and Free function pointer
 * @retval      data: real Policy data
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API dpm_data_type* dpm_get_password_policy(dpm_client_h handle, const char* p_username);

/**
 * @brief       API to set password status
 * @details     An administrator can know password status for this API. 
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Password Status 
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_password_status(dpm_client_h handle, const char* p_username, const int status);

/**
 * @brief       API to remove all password patterns.
 * @details     An administrator can remove all password patterns. 
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_delete_password_pattern(dpm_client_h handle, const char* p_username);

/**
 * @brief       API to get password pattern.
 * @details     This API can be used for applying complexity on new password value. 
 * @since_tizen 3.0
 * @code
 *    dpm_data_type* dpm_data;
 *    char* passwdPattern;
 *    dpm_data = dpm_get_password_pattern(handle, "owner");
 *    if (dpm_data != NULL) {
 *        passwdPattern = (char*)dpm_data->data;
 *        dpm_data->mem_header.free_func(dpm_data);
 *    }
 *
 * @endcode
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @return      DPM Data Structure
 * @retval      ret: DPM ERROR TYPE
 * @retval      mem_header: Header type and Free function pointer
 * @retval      data: real Password Pattern data
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API dpm_data_type* dpm_get_password_pattern(dpm_client_h handle, const char* p_username);

/**
 * @brief       API to set the maximum number of times a character can occur in the device password.
 * @details     Called by an admin that is managing the device to specify that any character in the device password cannot occur more than the specified maximum number of times.
 *                 Characters can be numeric or alphabetic or symbolic. "aaabcde" has 'a' which occurs 3 times, "1b1c1de" has '1'
 *                 which occurs 3 times and "a@b@c@" has '@' which occurs 3 times. A value of '0' specifies that no restrictions are applied
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Maximum Character Occurences 
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_maximum_character_occurrences(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to get the maximum number of times a character can occur in the device password.
 * @details     An administrator can retrieve the maximum number of times a character can occur in the device password. 
 *                 If more than one admin has set this value then the least value will take preference.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Pointer of Maximum Character Occurences 
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_get_maximum_character_occurrences(dpm_client_h handle, const char* p_username, int* p_value);

/**
 * @brief       API to set the maximum length of the numeric sequence which is allowed in the device password.
 * @details     Called by an administrator that is managing the device to set the maximum numeric sequence length. 
 *                 This specifies that the device password must not contain numeric sequences greater than the given length.
 *                 Numeric sequences can be increasing successively by one like "12345", or decreasing successively by one like "98765", or repeating like "55555".
 *                 These are all numeric sequences of length '5'. If maximum value is set to '5' then "123456" or "987654" or "555555" are not allowed.
 *                 A value of '0' specifies that no such numeric sequence restrictions are applied.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Maximum Numeric Sequence Length
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_maximum_numeric_sequence_length(dpm_client_h handle, const char* p_username, const int value);

/**
 * @brief       API to get the maximum numeric sequence length allowed in the device password.
 * @details     An administrator can retrieve the length of numeric sequences which are allowed in the device password. 
 *                 For instance, if the return value is '3' then "123", "987", "555" would all be numeric sequences of length '3' and will be allowed in the device password.
 *                 If more than one admin has set this value then the least value will take preference.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Pointer of Maximum Numeric Sequence Length
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_get_maximum_numeric_sequence_length(dpm_client_h handle, const char* p_username, int* p_value);

/**
 * @brief       API to set strings which are forbidden in the device password.
 * @details     Called by an admin that is managing the device to set strings that are forbidden to be used in the device password. 
 *                 This specifies any strings which must not be present in the device password such as personal data (variations on the user's name, email address or X400 address), or any other strings.
 *                 If the parameter list has only one blank string(""), then the stored strings are cleared.
 * @since_tizen 3.0
 * @code
 *    GList *p_string_list = NULL;
 *    p_string_list = g_list_append(p_string_list,g_strdup("forbidden"));
 *    p_string_list = g_list_append(p_string_list,g_strdup("strings"));
 *    if (dpm_set_forbidden_strings(handle, "owner", p_string_list) == DPM_ERROR_NONE) {
	    g_list_foreach((GList*)p_string_list,(GFunc)g_free,NULL);
	    g_list_free((GList*)p_string_list);
 *    } 
 *
 * @endcode
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @param[in]   Forbidden string list
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_forbidden_strings(dpm_client_h handle, const char* p_username, const GList* p_string_list);

/**
 * @brief       API to retrieve the forbidden strings which cannot be used in the device password.
 * @details     An administrator can retrieve the strings which are forbidden in the device password. 
 *                 Personal data (variations on the user's name, email address or X400 address) are examples of data which can be forbidden.
 *                 If more than one admin has set this value then the effective forbidden string list is the concatenation of the forbidden strings specified by all admins.
 * @since_tizen 3.0
 * @code
 *    dpm_data_type* dpm_data;
 *    GList *p_string_list = NULL;
 *    dpm_data = dpm_get_forbidden_strings(handle, "owner");
 *    if (dpm_data != NULL) {
 *        p_string_list = (GList*)dpm_data->data;
 *        while(p_string_list != NULL)
 *        {
 *            printf("Forbidden Strings: %s\n",(const char*)(p_string_list->data));
 *            p_string_list = p_string_list->next;
 *        }
 *        dpm_data->mem_header.free_func(dpm_data);
 *    }
 *
 * @endcode
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.password
 * @param[in]   handle Device Policy Client handle
 * @param[in]   User name
 * @return      DPM Data Structure
 * @retval      ret: DPM ERROR TYPE
 * @retval      mem_header: Header type and Free function pointer
 * @retval      data: real Fobidden string list
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API dpm_data_type *dpm_get_forbidden_strings(dpm_client_h handle, const char* p_username);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __PASSWORD_POLICY__

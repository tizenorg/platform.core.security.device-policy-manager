/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __CAPI_RESTRICTION_POLICY_H__
#define __CAPI_RESTRICTION_POLICY_H__

#include <dpm/context.h>

/**
 * @file restriction.h
 * @brief This file provides APIs to control restriction functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  DPM_RESTRICTION_POLICY
 * @{
 */

/**
 * @brief       The record handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_record_policy()
 * @see         dpm_context_release_record_policy()
 */
typedef void* dpm_restriction_policy_h;

/**
 * @brief       Acquires the restriction policy handle.
 * @details     This API acquires restriction policy handle required to enforce
 *              the restriction policies.
 * @since_tizen 3.0
 * @param[in]   handle The restriction policy context handle
 * @return      Restriction policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @see         dpm_context_release_restriction_policy()
 * @see         get_last_result()
 */
DPM_API dpm_restriction_policy_h dpm_context_acquire_restriction_policy(dpm_context_h handle);

/**
 * @brief       Releases the restriction policy handle.
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The record policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_restriction_policy().
 * @see         dpm_context_acquire_restriction_policy()
 */
DPM_API int dpm_context_release_restriction_policy(dpm_context_h context, dpm_restriction_policy_h handle);

/**
 * @brief       Allows or disallows the microphone.
 * @details     An administrator can use this API to set whether the microphone
 *              is allowed or not.
 * @since_tizen 3.0
 * @param[in]   handle The record policy handle
 * @param[in]   enable If true, disallow the microphone, if false, allow the microphone
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_context_acquire_restriction_policy().
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_get_microphone_state()
 */
DPM_API int dpm_restriction_set_microphone_state(dpm_restriction_policy_h handle, int enable);

/**
 * @brief       Gets the allow status of the microphone.
 * @details     An administrator can use this API to check the state of
 *              the microphone.
 * @since_tizen 3.0
 * @param[in]   handle The record policy handle
 * @return      status The allow status of the microphone
 * @pre         The handle must be created by dpm_context_acquire_restriction_policy().
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_set_microphone_state()
 */
DPM_API int dpm_restriction_get_microphone_state(dpm_restriction_policy_h handle);

/**
 * @brief       Aallows or disallows the user to change the location state
 * @details     An administrator can use this API to allow or disallow to change
 *              the location state.
 * @since_tizen 3.0
 * @param[in]   handle The geofence policy handle
 * @param[in]   enable If TRUE, allow to change the location state, if false, disallow
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_restriction_policy()
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_geofence_policy()
 */
DPM_API int dpm_restriction_set_location_state(dpm_restriction_policy_h handle, int enable);

/**
 * @brief       API to get the allow status of the location state change.
 * @details     An administrator can use this API to get the allow status of
 *              the location state change.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle The geofence policy handle
 * @return      status Allow status of localtion state change
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_geofence_policy()
 * @see         dpm_context_release_geofence_policy()
 */
DPM_API int dpm_restriction_get_location_state(dpm_restriction_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the settings changes.
 * @details     An administrator can use this API to set whether the Settins
 *              changes is allowed or not.
 * @since_tizen 3.0
 * @param[in]   handle The settings policy handle
 * @param[in]   enable If true, disallow the settings changes, if false, allow
 *              the settings changes
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_restriction_policy()
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_get_settings_changes_state()
 */
DPM_API int dpm_restriction_set_settings_changes_state(dpm_restriction_policy_h handle, int enable);

/**
 * @brief       Checks whether the settings changes restriction is enable or not.
 * @details     An administrator can use this API to get the allow statsus of
 *              the Settins changes.
 * @since_tizen 3.0
 * @param[in]   handle The settings policy handle
 * @return      true if the settings changes is disallowed, else false
 * @pre         handle must be created by dpm_context_acquire_restriction_policy()
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_set_settings_changes_state()
 */
DPM_API int dpm_restriction_get_settings_changes_state(dpm_restriction_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the usb mass storage
 * @details     An administrator can use this API to set whether the usb mass
 *              storage is allowed or not.
 * @since_tizen 3.0
 * @param[in]   handle The storage policy handle
 * @param[in]   enable If true, disallow the usb mass storage, if false, allow
 *              the usb mas storage
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_restriction_policy()
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_storage_get_external_storage_state()
 */
DPM_API int dpm_restriction_set_external_storage_state(dpm_restriction_policy_h handle, int enable);

/**
 * @brief       Checks whether the usb mass storage restriction is enable or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the usb mass storage.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle The storage policy handle
 * @return      true if the usb mass storage is disallowed, else false
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_storage_set_external_storage_state()
 */
DPM_API int dpm_restriction_get_external_storage_state(dpm_restriction_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the clipboard.
 * @details     An administrator can use this API to set whether the clipboard.
 *              is allowed or not
 * @since_tizen 3.0
 * @param[in]   handle DatasharePolicy Context Handle
 * @param[in]   enable If true, disallow the clipboard, if false, allow the clipboard
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_restriction_policy()
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_get_clipboard_state()
 */
DPM_API int dpm_restriction_set_clipboard_state(dpm_restriction_policy_h handle, int enable);

/**
 * @brief       Checks whether the the use of clipboard is allowed or not.
 * @details     An administrator can use this API to check the use of clipboard is allowed.
 * @since_tizen 3.0
 * @param[in]   handle DatasharePolicy Context Handle
 * @return      true if the clipboard is disallowed, else false
 * @pre         The handle must be created by dpm_context_acquire_restriction_policy().
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_set_clipboard_state()
 */
DPM_API int dpm_restriction_get_clipboard_state(dpm_restriction_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the usb debugging
 * @details     An administrator can use this API to set whether the usb debugging
 *              is allowed or not.
 * @since_tizen 3.0
 * @param[in]   handle DeveloperPolicy Context Handle
 * @param[in]   enable If true, disallow the usb debugging, if false, disallow the usb
 *              debugging
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_context_acquire_restriction_policy().
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_get_usb_debugging_state()
 */
DPM_API int dpm_restriction_set_usb_debugging_state(dpm_restriction_policy_h handle, int enable);

/**
 * @brief       Checks whether the usb debugging is allowed or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the usb debugging.
 * @since_tizen 3.0
 * @param[in]   handle DeveloperPolicy Context Handle
 * @return      true if the usb debugging is disallowed, else false
 * @pre         The handle must be created by dpm_context_acquire_restriction_policy().
 * @see         dpm_context_acquire_restriction_policy()
 * @see         dpm_context_release_restriction_policy()
 * @see         dpm_restriction_set_usb_debugging_state()
 */
DPM_API int dpm_restriction_get_usb_debugging_state(dpm_restriction_policy_h handle);

/**
 * @} // end of DPM_RESTRICTION_POLICY
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPI_RESTRICTION_POLICY_H__ */

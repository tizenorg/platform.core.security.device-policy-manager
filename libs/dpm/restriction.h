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

#include <dpm/device-policy-client.h>

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
  * @brief       The Bluetooth Policy handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_bluetooth_policy()
  * @see         dpm_context_release_bluetooth_policy()
  */
 typedef void* dpm_datashare_policy_h;

 /**
  * @brief       The Bluetooth Policy handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_bluetooth_policy()
  * @see         dpm_context_release_bluetooth_policy()
  */
 typedef void* dpm_developer_policy_h;

 /**
  * @brief       The Bluetooth Policy handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_bluetooth_policy()
  * @see         dpm_context_release_bluetooth_policy()
  */
 typedef void* dpm_storage_policy_h;

 /**
  * @brief       The Bluetooth Policy handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_bluetooth_policy()
  * @see         dpm_context_release_bluetooth_policy()
  */
 typedef void* dpm_settings_policy_h;

 /**
  * @brief       Acquires the Bluetooth Policy handle.
  * @details     This API acquires device bluetooth policy handle required to call
  *              the bluetooth policy APIs.
  * @since_tizen 3.0
  * @param[in]   handle Device Policy Context Handle
  * @param[in]   zone Target container name.
  * @return      Password Policy handle on success, otherwise NULL
  * @remark      The specific error code can be obtained by using the
  *              get_last_result() method. Error codes are described in
  *              exception section.
  * @exception   #DPM_ERROR_NONE No error
  * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
  * @exception   #DPM_ERROR_TIMED_OUT Time out
  * @exception   #DPM_ERROR_PERMISSION_DENIED The application does not have
  *              the privilege to access the target container.
  * @see         dpm_context_release_password_policy()
  * @see         get_last_result()
  */
 DPM_API dpm_datashare_policy_h dpm_context_acquire_datashare_policy(dpm_context_h handle);

 /**
  * @brief       Releases the Device Administration Policy Handle.
  * @details     This API must be called if interaction with the Device
  *              Policy Manager is no longer required.
  * @since_tizen 3.0
  * @param[in]   handle Password Policy Handle
  * @return      #DPM_ERROR_NONE on success, otherwise a negative value
  * @retval      #DPM_ERROR_NONE Successful
  * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
  * @retval      #DPM_ERROR_TIMED_OUT Time out
  * @pre         The handle must be created by dpm_context_acquire_password_policy().
  * @see         dpm_context_acquire_password_policy()
  */
 DPM_API int dpm_context_release_datashare_policy(dpm_datashare_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the clipboard.
 * @details     An administrator can use this API to set whether the clipboard.
 *              is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, disallow the clipboard, if false, allow the clipboard
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_clipboard_get_state()
 */
DPM_API int dpm_datashare_set_clipboard_state(dpm_datashare_policy_h handle, int enable);

/**
 * @brief       Checks whether the clipboard restriction is enable or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the clipboard
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the clipboard is disallowed, else false
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_clipboard_set_state()
 */
DPM_API int dpm_datashare_get_clipboard_state(dpm_datashare_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the settings changes.
 * @details     An administrator can use this API to set whether the Settins
 *              changes is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, disallow the settings changes, if false, allow
 *              the settings changes
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_set_settings_changes_restriction()
 */
DPM_API int dpm_settings_set_changes_restriction(dpm_settings_policy_h handle, int enable);

/**
 * @brief       Checks whether the settings changes restriction is enable or not.
 * @details     An administrator can use this API to get the allow statsus of
 *              the Settins changes.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the settings changes is disallowed, else false
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_settings_set_restriction()
 */
DPM_API int dpm_settings_is_changes_restricted(dpm_settings_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the usb debugging
 * @details     An administrator can use this API to set whether the usb debugging
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, disallow the usb debugging, if false, disallow the usb
 *              debugging
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_developer_get_usb_debugging_state()
 */
DPM_API int dpm_developer_set_usb_debugging_state(dpm_developer_policy_h handle, int enable);

/**
 * @brief       Checks whether the usb debugging restriction is enable or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the usb debugging.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the usb debugging is disallowed, else false
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_developer_set_usb_debugging_state()
 */
DPM_API int dpm_developer_get_usb_debugging_state(dpm_developer_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the usb mass storage
 * @details     An administrator can use this API to set whether the usb mass
 *              storage is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, disallow the usb mass storage, if false, allow
 *              the usb mas storage
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_storage_get_usb_mass_storage_state()
 */
DPM_API int dpm_storage_set_usb_mass_storage_state(dpm_storage_policy_h handle, int enable);

/**
 * @brief       Checks whether the usb mass storage restriction is enable or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the usb mass storage.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the usb mass storage is disallowed, else false
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_storage_set_usb_mass_storage_state()
 */
DPM_API int dpm_storage_get_usb_mass_storage_state(dpm_storage_policy_h handle);

/**
 * @brief       Wipe all container data.
 * @details     An administrator can wipe data in container for solving security issues.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
DPM_API int dpm_device_wipe_data(dpm_storage_policy_h handle);

/**
 * @} // end of DPM_RESTRICTION_POLICY
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPI_RESTRICTION_POLICY_H__ */

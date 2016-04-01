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

#ifndef __CAPI_DEVELOPER_POLICY_H__
#define __CAPI_DEVELOPER_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file developer.h
 * @brief This file provides APIs to register/deregister Device Policy Admin Client
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_DEVELOPER_POLICY_MODULE
 * @{
 */

/**
  * @brief       The developer Policy handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_bluetooth_policy()
  * @see         dpm_context_release_bluetooth_policy()
  */
 typedef void* dpm_developer_policy_h;

/**
  * @brief       Acquires the BluetoothPolicy Policy handle.
  * @details     This API acquires device bluetooth policy handle required to call
  *              the bluetooth policy APIs.
  * @since_tizen 3.0
  * @param[in]   handle Device Policy Context Handle
  * @param[in]   zone Target container name.
  * @return      PasswordPolicy Policy handle on success, otherwise NULL
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
 DPM_API dpm_developer_policy_h dpm_context_acquire_developer_policy(dpm_context_h handle);

 /**
  * @brief       Releases the Device AdministrationPolicy Policy Handle.
  * @details     This API must be called if interaction with the Device
  *              Policy Manager is no longer required.
  * @since_tizen 3.0
  * @param[in]   handle PasswordPolicy Policy Handle
  * @return      #DPM_ERROR_NONE on success, otherwise a negative value
  * @retval      #DPM_ERROR_NONE Successful
  * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
  * @retval      #DPM_ERROR_TIMED_OUT Time out
  * @pre         The handle must be created by dpm_context_acquire_password_policy().
  * @see         dpm_context_acquire_password_policy()
  */
 DPM_API int dpm_context_release_developer_policy(dpm_developer_policy_h handle);

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


#ifdef __cplusplus
}
#endif

#endif //__CAPI_DEVELOPER_POLICY_H__

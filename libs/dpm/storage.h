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

#ifndef __CAPI_STORAGE_POLICY_H__
#define __CAPI_STORAGE_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file storage.h
 * @brief This file provides APIs to register/deregister Device Policy Admin Client
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_STORAGE_POLICY_MODULE
 * @{
 */

 /**
  * @brief       The StoragePolicy Context handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_storage_policy()
  * @see         dpm_context_release_storage_policy()
  */
 typedef void* dpm_storage_policy_h;

/**
  * @brief       Acquires the StoragePolicy Context handle.
  * @details     This API acquires device storage policy handle required to call
  *              the storage policy APIs.
  * @since_tizen 3.0
  * @param[in]   handle StoragePolicy Context Handle
  * @param[in]   zone Target container name.
  * @return      StoragePolicy Policy handle on success, otherwise NULL
  * @remark      The specific error code can be obtained by using the
  *              get_last_result() method. Error codes are described in
  *              exception section.
  * @exception   #DPM_ERROR_NONE No error
  * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
  * @exception   #DPM_ERROR_TIMED_OUT Time out
  * @exception   #DPM_ERROR_PERMISSION_DENIED The application does not have
  *              the privilege to access the target container.
  * @see         dpm_context_release_storage_policy()
  */
 DPM_API dpm_storage_policy_h dpm_context_acquire_storage_policy(dpm_context_h handle);

 /**
  * @brief       Releases the StoragePolicy Context handle.
  * @details     This API must be called if interaction with the Device
  *              Policy Manager is no longer required.
  * @since_tizen 3.0
  * @param[in]   handle StoragePolicy Context Handle
  * @return      #DPM_ERROR_NONE on success, otherwise a negative value
  * @retval      #DPM_ERROR_NONE Successful
  * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
  * @retval      #DPM_ERROR_TIMED_OUT Time out
  * @pre         The handle must be created by dpm_context_acquire_storage_policy().
  * @see         dpm_context_acquire_storage_policy()
  */
 DPM_API int dpm_context_release_storage_policy(dpm_storage_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the usb mass storage
 * @details     An administrator can use this API to set whether the usb mass
 *              storage is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.usb
 * @param[in]   handle StoragePolicy Context Handle
 * @param[in]   enable If true, disallow the usb mass storage, if false, allow
 *              the usb mas storage
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_storage_policy()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_context_acquire_storage_policy()
 * @see         dpm_context_release_storage_policy()
 * @see         dpm_storage_get_usb_mass_storage_state()
 */
DPM_API int dpm_storage_set_usb_mass_storage_state(dpm_storage_policy_h handle, int enable);

/**
 * @brief       Checks whether the usb mass storage restriction is enable or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the usb mass storage.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.usb
 * @param[in]   handle StoragePolicy Context Handle
 * @return      true if the usb mass storage is disallowed, else false
 * @pre         handle must be created by dpm_context_acquire_storage_policy()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_context_acquire_storage_policy()
 * @see         dpm_context_release_storage_policy()
 * @see         dpm_storage_set_usb_mass_storage_state()
 */
DPM_API int dpm_storage_get_usb_mass_storage_state(dpm_storage_policy_h handle);

/**
 * @brief       Wipe all container data.
 * @details     An administrator can wipe data in container for solving critical security issues.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.wipe
 * @param[in]   handle StoragePolicy Context Handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_storage_policy()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_context_acquire_storage_policy()
 * @see         dpm_context_release_storage_policy()
 */
DPM_API int dpm_device_wipe_data(dpm_storage_policy_h handle);


#ifdef __cplusplus
}
#endif

#endif //__CAPI_STORAGE_POLICY_H__

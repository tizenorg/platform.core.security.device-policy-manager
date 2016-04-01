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

#ifndef __CAPI_ADMINISTRATION_POLICY_H__
#define __CAPI_ADMINISTRATION_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file administration.h
 * @brief This file provides APIs to register/deregister Device Policy Admin Client
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_ADMINISTRATION_POLICY_MODULE
 * @{
 */

 /**
  * @brief       The AdministrationPolicy Policy handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_admin_policy()
  * @see         dpm_context_release_admin_policy()
  */
 typedef void* dpm_admin_policy_h;

 /**
  * @brief       Acquires the Device AdministrationPolicy Policy handle.
  * @details     This API acquires device administration policy handle required to call
  *              the device admin APIs.
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
 DPM_API dpm_admin_policy_h dpm_context_acquire_admin_policy(dpm_context_h handle);

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
 DPM_API int dpm_context_release_admin_policy(dpm_admin_policy_h handle);

/**
 * @brief       Registers Device Policy Admin Client.
 * @details     This API is used to register the Device Policy Admin Client to
 *              the Device Policy Manager. The Device Policy Admin Client must be
 *              registered to use Device AdministrationPolicy APIs.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @param[in]   name Device Policy Admin package name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid package name
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_admin_deregister_client()
 */
DPM_API int dpm_admin_register_client(dpm_admin_policy_h handle, const char* name);

/**
 * @brief       Deregisters the Device Policy Admin Client.
 * @details     This API is used to deregister the Device Policy Client from
 *              the Device Policy Manager.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @param[in]   name Device Policy Admin package name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid package name
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_admin_register_client()
 */
DPM_API int dpm_admin_deregister_client(dpm_admin_policy_h handle, const char* name);

#ifdef __cplusplus
}
#endif

#endif //__CAPI_ADMINISTRATION_POLICY_H__

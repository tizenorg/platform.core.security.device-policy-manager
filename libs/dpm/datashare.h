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

#ifndef __CAPI_DATASHARE_POLICY_H__
#define __CAPI_DATASHARE_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file datashare.h
 * @brief This file provides APIs to register/deregister Device Policy Admin Client
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_DATASHARE_POLICY_MODULE
 * @{
 */

/**
 * @brief       The datashare policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_datashare_policy()
 * @see         dpm_context_release_datashare_policy()
 */
typedef void* dpm_datashare_policy_h;

/**
 * @brief       Acquires the datashare policy handle.
 * @details     This API acquires datashare policy handle required to enforce
 *              datashare policies.
 * @since_tizen 3.0
 * @param[in]   handle Device policy context handle
 * @return      Datashare policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @exception   #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to access the target container.
 * @see         dpm_context_release_datashare_policy()
 * @see         get_last_result()
 */
DPM_API dpm_datashare_policy_h dpm_context_acquire_datashare_policy(dpm_context_h handle);

/**
 * @brief       Releases the datashare policy andle.
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle Datashare policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_datashare_policy().
 * @see         dpm_context_acquire_datashare_policy()
 */
DPM_API int dpm_context_release_datashare_policy(dpm_datashare_policy_h handle);

/**
 * @brief       Allows or disallows the user to use the clipboard.
 * @details     An administrator can use this API to set whether the clipboard.
 *              is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.clipboard
 * @param[in]   handle DatasharePolicy Context Handle
 * @param[in]   enable If true, disallow the clipboard, if false, allow the clipboard
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_datashare_policy()
 * @see         dpm_context_acquire_datashare_policy()
 * @see         dpm_context_release_datashare_policy()
 * @see         dpm_datashare_get_clipboard_state()
 */
DPM_API int dpm_datashare_set_clipboard_state(dpm_datashare_policy_h handle, int enable);

/**
 * @brief       Checks whether the the use of clipboard is allowed or not.
 * @details     An administrator can use this API to check the use of clipboard is allowed.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.clipboard
 * @param[in]   handle DatasharePolicy Context Handle
 * @return      true if the clipboard is disallowed, else false
 * @pre         The handle must be created by dpm_context_acquire_datashare_policy().
 * @see         dpm_context_acquire_datashare_policy()
 * @see         dpm_context_release_datashare_policy()
 * @see         dpm_datashare_set_clipboard_state()
 */
DPM_API int dpm_datashare_get_clipboard_state(dpm_datashare_policy_h handle);

#ifdef __cplusplus
}
#endif

#endif //__CAPI_DATASHARE_POLICY_H__

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

#ifndef __CAPI_SETTINGS_POLICY_H__
#define __CAPI_SETTINGS_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file settings.h
 * @brief This file provides APIs to register/deregister Device Policy Admin Client
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_SETTINGS_POLICY_MODULE
 * @{
 */

 /**
  * @brief       The BluetoothPolicy Policy handle
  * @since_tizen 3.0
  * @see         dpm_context_acquire_bluetooth_policy()
  * @see         dpm_context_release_bluetooth_policy()
  */
 typedef void* dpm_settings_policy_h;


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


#ifdef __cplusplus
}
#endif

#endif //__CAPI_SETTINGS_POLICY_H__

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

#ifndef __CAPI_MISC_POLICY_H__
#define __CAPI_MISC_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file misc.h
 * @brief This file provides APIs to control misc functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  DPM_MISC_POLICY
 * @{
 */

/**
 * @brief       The Camera Policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_camera_policy()
 * @see         dpm_context_release_camera_policy()
 */
typedef void* dpm_location_policy_h;

/**
 * @brief       The Camera Policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_camera_policy()
 * @see         dpm_context_release_camera_policy()
 */
typedef void* dpm_storage_policy_h;

/**
 * @brief       Aallows or disallows the user to change the location state
 * @details     An administrator can use this API to allow or disallow to change
 *              the location state.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.location
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If TRUE, allow to change the location state, if false, disallow
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @post
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
DPM_API int dpm_location_allow_state_change(dpm_location_policy_h handle, int enable);

/**
 * @brief       API to get the allow status of the location state change.
 * @details     An administrator can use this API to get the allow status of
 *              the location state change.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      status Allow status of localtion state change
 * @pre         handle must be created by dpm_context_create()
 * @post
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
DPM_API int dpm_location_is_state_change_allowed(dpm_location_policy_h handle);

/**
 * @brief       Allows or disallows the SD Card.
 * @details     An administrator can use this API to set whether the sd card
 *              is restricted or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.misc
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If TRUE, disallow SD Card, if FALSE, allow the SD Card
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMOUT Timeout
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @post
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
DPM_API int dpm_set_sd_card_restriction(dpm_storage_policy_h handle, int enable);

/**
 * @brief       Gets the restriction status of the sd card.
 * @details     An administrator can use this API to get the restriction status of
 *              the sd card.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      status Restriction status of the SD Card
 * @pre         handle must be created by dpm_context_create()
 * @post
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
DPM_API int dpm_is_sd_card_restricted(dpm_storage_policy_h handle);

/**
 * @} // end of DPM_MISC_POLICY
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPI_MISC_POLICY_H__ */

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

#ifndef __CAPI_GEOFENCE_POLICY_H__
#define __CAPI_GEOFENCE_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file geofence.h
 * @brief This file provides APIs to control location/geofence functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_GEOFENCE_POLICY_MODULE
 * @{
 */

/**
 * @brief       The geofence policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_geofence_policy()
 * @see         dpm_context_release_geofence_policy()
 */
typedef void* dpm_geofence_policy_h;

/**
 * @brief       Acquires the geofence policy handle.
 * @details     This API acquires geofence policy handle required to enforce
 *              the geofence policies.
 * @since_tizen 3.0
 * @param[in]   handle The device policy context handle
 * @return      Geofence policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @exception   #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to enforce the geofence policies.
 * @see         dpm_context_release_geofence_policy()
 * @see         get_last_result()
 */
DPM_API dpm_geofence_policy_h dpm_context_acquire_geofence_policy(dpm_context_h handle);

/**
 * @brief       Releases the geofence policy handle.
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The geofence policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_geofence_policy().
 * @see         dpm_context_acquire_geofence_policy()
 */
DPM_API int dpm_context_release_geofence_policy(dpm_geofence_policy_h handle);

/**
 * @brief       Aallows or disallows the user to change the location state
 * @details     An administrator can use this API to allow or disallow to change
 *              the location state.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.geofence
 * @param[in]   handle The geofence policy handle
 * @param[in]   enable If TRUE, allow to change the location state, if false, disallow
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_geofence_policy()
 * @see         dpm_context_release_geofence_policy()
 */
DPM_API int dpm_geofence_allow_state_change(dpm_geofence_policy_h handle, int enable);

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
DPM_API int dpm_geofence_is_state_change_allowed(dpm_geofence_policy_h handle);

#ifdef __cplusplus
}
#endif

#endif //__CAPI_GEOFENCE_POLICY_H__

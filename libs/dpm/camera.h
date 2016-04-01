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

#ifndef __CAPI_CAMERA_POLICY_H__
#define __CAPI_CAMERA_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file camera.h
 * @brief This file provides APIs to register/deregister Device Policy Admin Client
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_CAMERA_POLICY_MODULE
 * @{
 */

/**
 * @brief		The Camera Policy handle
 * @since_tizen 3.0
 * @see 		dpm_context_acquire_camera_policy()
 * @see 		dpm_context_release_camera_policy()
 */
typedef void* dpm_camera_policy_h;

/**
 * @brief       Acquires the Camera Policy handle.
 * @details     This API acquires camera policy handle required to call
 *              the device policy camera APIs.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Context Handle
 * @param[in]   zone Target container name.
 * @return      Camera Policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @exception   #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to access the target container.
 * @see         dpm_context_release_camera_policy()
 * @see         get_last_result()
 */
DPM_API dpm_camera_policy_h dpm_context_acquire_camera_policy(dpm_context_h handle, const char* zone);

/**
 * @brief       Releases the Camera Policy Handle.
 * @details     This API must be called if interaction with the Device
 *              Policy Manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle Camera Policy Handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_camera_policy().
 * @see         dpm_context_acquire_camera_policy()
 */
DPM_API int dpm_context_release_camera_policy(dpm_camera_policy_h handle);

/**
 * @brief       Allows or disallows the camera.
 * @details     An administrator can use this API to set whether the camera
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.misc
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, disallow the camera, if false, allow the camera
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_context_create().
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
DPM_API int dpm_camera_set_state(dpm_camera_policy_h handle, int enable);

/**
 * @brief       Gets the allow status of the camera.
 * @details     An administrator can use this API to get the allow status of
 *              the camera.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @return      status The restriction status of the camera
 * @pre         The handle must be created by dpm_context_create().
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
DPM_API int dpm_camera_get_state(dpm_camera_policy_h handle);

/**
 * @} // end of DPM_MISC_POLICY
 */
#ifdef __cplusplus
}
#endif

#endif //__CAPI_CAMERA_POLICY_H__

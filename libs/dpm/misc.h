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
 * @pre         The handle must be created by dpm_create_client().
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_camera_restriction(dpm_client_h handle, int enable);

/**
 * @brief       Gets the allow status of the camera.
 * @details     An administrator can use this API to get the allow status of
 *              the camera.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[out] status The allow status of the camera
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_create_client().
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_is_camera_restricted(dpm_client_h handle, int *status);

/**
 * @brief       Allows or disallows the microphone.
 * @details     An administrator can use this API to set whether the microphone
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.misc
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, disallow the microphone, if false, allow the microphone
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_create_client().
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_microphone_restriction(dpm_client_h handle, int enable);

/**
 * @brief        Gets the allow status of the microphone.
 * @details     An administrator can use this API to check the state of
 *              the microphone.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[out] status The allow status of the microphone
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_create_client().
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_is_microphone_restricted(dpm_client_h handle, int *status);

/**
 * @brief       API to allow or disable the location.
 * @details     An administrator can use this API to set whether the location
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.misc
 * @param[in]   handle Device Policy Client handle
 * @param[in]   allow If true, allow the location, if false, disable the location
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_location_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the location.
 * @details     An administrator can use this API to get the allow status of
 *              the location.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the location is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_is_location_restricted(dpm_client_h handle);

/**
 * @brief       API to allow or disable the sd card.
 * @details     An administrator can use this API to set whether the sd card
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.misc
 * @param[in]   handle Device Policy Client handle
 * @param[in]   allow If true, allow the sd card, if false, disable the sd card
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_sd_card_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the sd card.
 * @details     An administrator can use this API to get the allow status of
 *              the sd card.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the sd card is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_is_sd_card_restricted(dpm_client_h handle);

/**
 * @} // end of DPM_MISC_POLICY
 */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPI_MISC_POLICY_H__ */

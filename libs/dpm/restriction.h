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
 * @brief       API to allow or disable the clipboard
 * @details     An administrator can use this API to set whether the clipboard
 *              is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, allow the clipboard, if false, disable the clipboard
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_clipboard_restricted()
 */
DPM_API int dpm_set_clipboard_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the clipboard
 * @details     An administrator can use this API to get the allow status of
 *              the clipboard
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the clipboard is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_set_clipboard_restriction()
 */
DPM_API int dpm_is_clipboard_restricted(dpm_client_h handle);

/**
 * @brief       API to allow or disable the settings changes
 * @details     An administrator can use this API to set whether the Settins
 *              changes is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   allow If true, allow the settings changes, if false, disable
 *              the settings changes
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_setting_changes_allowed()
 */
DPM_API int dpm_set_settings_changes_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the settings changes
 * @details     An administrator can use this API to get the allow statsus of
 *              the Settins changes
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the settings changes is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_allow_setting_changes()
 */
DPM_API int dpm_is_settings_changes_restricted(dpm_client_h handle);

/**
 * @brief       API to allow or disable the background data
 * @details     An administrator can use this API to set whether the background
 *              data is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   allow If true, allow the background data, if false, disable
 *              the background data
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_background_data_allowed()
 */
DPM_API int dpm_set_background_data_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the background data
 * @details     An administrator can use this API to get the allow status of
 *              the background data
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the background data is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_allow_background_data()
 */
DPM_API int dpm_is_background_data_restricted(dpm_client_h handle);

/**
 * @brief       API to allow or disable the usb debugging
 * @details     An administrator can use this API to set whether the usb debugging
 *              is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   allow If true, allow the usb debugging, if false, disable the usb
 *              debugging
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_usb_debugging_allowed()
 */
DPM_API int dpm_set_usb_debugging_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the usb debugging
 * @details     An administrator can use this API to get the allow status of
 *              the usb debugging
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the usb debugging is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_allow_usb_debugging()
 */
DPM_API int dpm_is_usb_debugging_restricted(dpm_client_h handle);

/**
 * @brief       API to allow or disable the usb mass storage
 * @details     An administrator can use this API to set whether the usb mass
 *              storage is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   allow If true, allow the usb mass storage, if false, disable
 *              the usb mas storage
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_usb_mass_storage_allowed()
 */
DPM_API int dpm_set_usb_mass_storage_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the usb mass storage
 * @details     An administrator can use this API to get the allow status of
 *              the usb mass storage
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the usb mass storage is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_allow_usb_mass_storage()
 */
DPM_API int dpm_is_usb_mass_storage_restricted(dpm_client_h handle);

/**
 * @brief       API to allow or disable the factory reset
 * @details     An administrator can use this API to set whether the factory
 *              reset is allowed or not
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.restriction
 * @param[in]   handle Device Policy Client handle
 * @param[in]   allow If true, allow the factory reset, if false, disbale
 *              the factory reset
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_factory_reset_allowed()
 */
DPM_API int dpm_set_factory_reset_restriction(dpm_client_h handle, int enable);

/**
 * @brief       API to get the allow status of the factory reset
 * @details     An administrator can use this API to get the allow status of
 *              factory reset
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      true if the factory reset is allowed, else false
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_allow_factory_reset()
 */
DPM_API int dpm_is_factory_reset_restricted(dpm_client_h handle);

/**
 * @} // end of DPM_RESTRICTION_POLICY
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPI_RESTRICTION_POLICY_H__ */

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

#ifndef __CAPI_BLUETOOTH_POLICY_H__
#define __CAPI_BLUETOOTH_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file bluetooth.h
 * @brief This file provides APIs to control bluetooth functionality such as
 *        bluetooth device or uuid restriction
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_BLUETOOTH_POLICY_MODULE
 * @{
 */

/**
 * @brief       Adds MAC address to blacklist
 * @details     An administrator can use this API to add new MAC address to
 *              blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @param[in]   mac_address MAC Address that should be added
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_add_bluetooth_device_to_blacklist(dpm_client_h handle, const char* mac_address);

/**
 * @brief       Removes MAC address from blacklist
 * @details     An administrator can use this API to remove MAC address from
 *              blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @param[in]   mac_address MAC Address that should be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_remove_bluetooth_device_from_blacklist(dpm_client_h handle, const char* mac_address);

/**
 * @brief       Enables or disables device restriction of bluetooth
 * @details     An administrator can use this API to set whether the bluetooth
 *              is allowed or not by device restriction
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, allow the bluetooth, if false, disable the bluetooth
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_bluetooth_device_restriction(dpm_client_h handle, const int enable);

/**
 * @brief       Gets the allow status of the bluetooth's device restriction
 * @details     An administrator can use this API to get the allow status of the
 *              bluetooth. The device restricted bluetooth will be worked by MAC
 *              address blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @return      true if the bluetooth cannot pairing device, else false
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_is_bluetooth_device_restricted(dpm_client_h handle);

/**
 * @brief       Adds UUID to blacklist
 * @details     An administrator can use this API to add new UUID to blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @param[in]   uuid UUID that should be added
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_add_bluetooth_uuid_to_blacklist(dpm_client_h handle, const char* uuid);

/**
 * @brief       Removes UUID from blacklist
 * @details     An administrator can use this API to remove UUID from blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @param[in]   uuid UUID that should be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_remove_bluetooth_uuid_from_blacklist(dpm_client_h handle, const char* uuid);

/**
 * @brief       Enables or disables UUID restriction of bluetooth
 * @details     An administrator can use this API to set whether the bluetooth
 *              is allowed or not by UUID restriction
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable If true, allow the bluetooth, if false, disable the bluetooth
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_set_bluetooth_uuid_restriction(dpm_client_h handle, const int enable);

/**
 * @brief       Gets the allow status of the bluetooth's UUID restriction
 * @details     An administrator can use this API to get the allow status of the
 *              bluetooth. The UUID restricted bluetooth will be worked by
 *              UUID blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device Policy Client handle
 * @return      true if the bluetooth cannot pairing device, else false
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
DPM_API int dpm_is_bluetooth_uuid_restricted(dpm_client_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_BLUETOOTH_POLICY_H__

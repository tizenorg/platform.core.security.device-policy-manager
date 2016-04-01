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
 * @brief       The bluetooth policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 */
typedef void* dpm_bluetooth_policy_h;

/**
 * @brief       Acquires the bluetooth policy handle.
 * @details     This API acquires bluetooth policy handle required to call
 *              the bluetooth policy APIs.
 * @since_tizen 3.0
 * @param[in]   handle The device policy context handle
 * @return      Bluetooth policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @see         dpm_context_release_bluetooth_policy()
 * @see         get_last_result()
 */
DPM_API dpm_bluetooth_policy_h dpm_context_acquire_bluetooth_policy(dpm_context_h handle);

/**
 * @brief       Releases the bluetooth policy handle.
 * @details     This API must be called if interaction with the Device
 *              Policy Manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The bluetooth policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_bluetooth_policy().
 * @see         dpm_context_acquire_bluetooth_policy()
 */
DPM_API int dpm_context_release_bluetooth_policy(dpm_bluetooth_policy_h handle);

/**
 * @brief       Adds MAC address to blacklist
 * @details     An administrator can use this API to add new MAC address to
 *              blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @param[in]   address The MAC address that should be added
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_remove_device_from_blacklist()
 */
DPM_API int dpm_bluetooth_add_device_to_blacklist(dpm_bluetooth_policy_h handle, const char* address);

/**
 * @brief       Removes MAC address from blacklist
 * @details     An administrator can use this API to remove MAC address from
 *              blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @param[in]   address The MAC address that should be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_add_device_to_blacklist()
 */
DPM_API int dpm_bluetooth_remove_device_from_blacklist(dpm_bluetooth_policy_h handle, const char* address);

/**
 * @brief       Enables or disables device restriction of bluetooth
 * @details     An administrator can use this API to set whether the bluetooth
 *              is allowed or not by device restriction
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @param[in]   enable If true, restrict the bluetooth pairing, if false, allow the bluetooth pairing
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_is_device_restricted()
 */
DPM_API int dpm_bluetooth_set_device_restriction(dpm_bluetooth_policy_h handle, const int enable);

/**
 * @brief       Gets the allow status of the bluetooth's device restriction
 * @details     An administrator can use this API to get the allow status of the
 *              bluetooth. The device restricted bluetooth will be worked by MAC
 *              address blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @return      true if the bluetooth pairing has been restricted, else false
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_set_device_restriction()
 */
DPM_API int dpm_bluetooth_is_device_restricted(dpm_bluetooth_policy_h handle);

/**
 * @brief       Adds UUID to blacklist
 * @details     An administrator can use this API to add new UUID to blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @param[in]   uuid The UUID that should be added
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_remove_uuid_from_blacklist()
 */
DPM_API int dpm_bluetooth_add_uuid_to_blacklist(dpm_bluetooth_policy_h handle, const char* uuid);

/**
 * @brief       Removes UUID from blacklist
 * @details     An administrator can use this API to remove UUID from blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @param[in]   uuid The UUID that should be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_add_uuid_to_blacklist()
 */
DPM_API int dpm_bluetooth_remove_uuid_from_blacklist(dpm_bluetooth_policy_h handle, const char* uuid);

/**
 * @brief       Enables or disables UUID restriction of bluetooth
 * @details     An administrator can use this API to set whether the bluetooth
 *              is allowed or not by UUID restriction
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @param[in]   enable If true, restrict the bluetooth pairing, if false, allow the bluetooth pairing
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_is_uuid_restricted()
 */
DPM_API int dpm_bluetooth_set_uuid_restriction(dpm_bluetooth_policy_h handle, const int enable);

/**
 * @brief       Gets the allow status of the bluetooth's UUID restriction
 * @details     An administrator can use this API to get the allow status of the
 *              bluetooth. The UUID restricted bluetooth will be worked by
 *              UUID blacklist
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle The bluetooth policy handle
 * @return      true if the bluetooth pairing has been restricted, else false
 * @pre         handle must be created by dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_acquire_bluetooth_policy()
 * @see         dpm_context_release_bluetooth_policy()
 * @see         dpm_bluetooth_set_uuid_restriction()
 */
DPM_API int dpm_bluetooth_is_uuid_restricted(dpm_bluetooth_policy_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_BLUETOOTH_POLICY_H__

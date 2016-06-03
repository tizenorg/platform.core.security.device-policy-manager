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

#ifndef __CAPI_SECURITY_POLICY_H__
#define __CAPI_SECURITY_POLICY_H__

#include <dpm/device-policy-manager.h>

/**
 * @file security.h
 * @brief This file provides APIs to control security functionality such as
 *        device encryption and screen lock
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_SECURITY_POLICY_MODULE
 * @{
 */

/**
 * @brief       API to immediately lock device screen
 * @details     An administrator can use this API to lock the device screen
 *              immediately
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 */
DPM_API int dpm_security_lockout_screen(device_policy_manager_h handle);

/**
 * @brief       API to encrypt internal storage
 * @details     An administrator can use this API to enable full device
 *              encryption, which includes device memory and internal SD card.
 *              Before calling this API, administrator must ensure that
 *              the device password is set to alphanumeric quality.
 *              The administrator can set an alphanumeric password by using
 *              dpm_set_password_quality() API
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[in]   encrypt TRUE if encryption is required, FALSE if decryption is
 *              required
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 */
DPM_API int dpm_security_set_internal_storage_encryption(device_policy_manager_h handle, const int encrypt);

/**
 * @brief       API to check the state of internal storage encryption
 * @details     An administrator can use this API to check whether internal
 *              storage encryption is enabled.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  state TRUE if internal storage is encrypted or being encrypted,
 *              else false.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @pre         handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_security_is_internal_storage_encrypted()
 */
DPM_API int dpm_security_is_internal_storage_encrypted(device_policy_manager_h handle);

/**
 * @brief       API to encrypt external storage
 * @details     An administrator can use this API to enable external SD card
 *              encryption. Before calling this API, administrator must
 *              ensure that the device password is set to alphanumeric quality.
 *              The administrator can set an alphanumeric password by using
 *              dpm_set_password_quality() API
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[in]   encrypt TRUE if encryption is required, FALSE if decryption is
 *              required
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_security_set_internal_storage_encryption()
 */
DPM_API int dpm_security_set_external_storage_encryption(device_policy_manager_h handle, const int encrypt);

/**
 * @brief       API to check the state of external storage encryption
 * @details     An administrator can use this API to check whether external
 *              storage encryption is enabled.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out   TRUE if external storage is encrypted or being encrypted,
 *              else FALSE
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @pre         handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 */
DPM_API int dpm_security_is_external_storage_encrypted(device_policy_manager_h handle, int *state);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_SECURITY_POLICY_H__

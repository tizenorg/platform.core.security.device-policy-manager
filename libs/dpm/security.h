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

#include <dpm/device-policy-client.h>

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
 * @brief       Enumeration for device wipe type
 * @since_tizen 3.0
 */
typedef enum {
    WIPE_INTERNAL_MEMORY = (1 << 0), /**< Wipe internal memory. */
    WIPE_EXTERNAL_MEMORY = (1 << 1)  /**< Wipe external memory. */
} dpm_wipe_type_e;

/**
 * @brief       The security policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_security_policy()
 * @see         dpm_context_release_security_policy()
 */
typedef void* dpm_security_policy_h;

/**
 * @brief       Acquires the security policy handle.
 * @details     This API acquires security policy handle required to enforce
 *              the security policies.
 * @since_tizen 3.0
 * @param[in]   handle Device policy context handle
 * @return      Security policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @exception   #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to enforce security policies.
 * @see         dpm_context_release_security_policy()
 * @see         get_last_result()
 */
DPM_API dpm_security_policy_h dpm_context_acquire_security_policy(dpm_context_h handle);

/**
 * @brief       Releases the security policy andle.
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle Security policy Handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_security_policy().
 * @see         dpm_context_acquire_security_policy()
 */
DPM_API int dpm_context_release_security_policy(dpm_security_policy_h handle);

/**
 * @brief       API to immediately lock device screen
 * @details     An administrator can use this API to lock the device screen
 *              immediately
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.security
 * @param[in]   handle Security policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_security_policy()
 * @see         dpm_context_release_security_policy()
 */
DPM_API int dpm_security_lockout_screen(dpm_security_policy_h handle);

/**
 * @brief       API to selectively wipe external memory, internal memory,
 *              or both.
 * @details     Device Admin can use this API to wipe both SD card data
 *              and application data.
 *              Calling this API may require rebooting the device.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.security
 * @param[in]   handle Security policy handle
 * @param[in]   type DPM_WIPE_INTERNAL_MEMORY or DPM_WIPE_EXTERNAL_MEMORY
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_security_policy()
 * @see         dpm_context_release_security_policy()
 */
DPM_API int dpm_security_wipe_data(dpm_security_policy_h handle, const dpm_wipe_type_e type);

/**
 * @brief       API to encrypt internal storage
 * @details     An administrator can use this API to enable full device
 *              encryption, which includes device memory and internal SD card.
 *              Before calling this API, administrator must ensure that
 *              the device password is set to alphanumeric quality.
 *              The administrator can set an alphanumeric password by using
 *              dpm_set_password_quality() API
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.security
 * @param[in]   handle Security policy handle
 * @param[in]   encrypt TRUE if encryption is required, FALSE if decryption is
 *              required
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_security_policy()
 * @see         dpm_context_release_security_policy()
 */
DPM_API int dpm_security_set_internal_storage_encryption(dpm_security_policy_h handle, const int encrypt);

/**
 * @brief       API to check the state of internal storage encryption
 * @details     An administrator can use this API to check whether internal
 *              storage encryption is enabled.
 * @since_tizen 3.0
 * @param[in]   handle Security policy handle
 * @return      TRUE if internal storage is encrypted or being encrypted,
 *              else false.
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_security_policy()
 * @see         dpm_context_release_security_policy()
 * @see         dpm_security_is_internal_storage_encrypted()
 */
DPM_API int dpm_security_is_internal_storage_encrypted(dpm_security_policy_h handle);

/**
 * @brief       API to encrypt external storage
 * @details     An administrator can use this API to enable external SD card
 *              encryption. Before calling this API, administrator must
 *              ensure that the device password is set to alphanumeric quality.
 *              The administrator can set an alphanumeric password by using
 *              dpm_set_password_quality() API
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.security
 * @param[in]   handle Security policy handle
 * @param[in]   encrypt TRUE if encryption is required, FALSE if decryption is
 *              required
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_security_policy()
 * @see         dpm_context_release_security_policy()
 * @see         dpm_security_set_internal_storage_encryption()
 */
DPM_API int dpm_security_set_external_storage_encryption(dpm_security_policy_h handle, const int encrypt);

/**
 * @brief       API to check the state of external storage encryption
 * @details     An administrator can use this API to check whether external
 *              storage encryption is enabled.
 * @since_tizen 3.0
 * @param[in]   handle Security policy handle
 * @return      TRUE if external storage is encrypted or being encrypted,
 *              else FALSE
 * @pre         handle must be created by dpm_context_create()
 * @see         dpm_context_acquire_security_policy()
 * @see         dpm_context_release_security_policy()
 */
DPM_API int dpm_security_is_external_storage_encrypted(dpm_security_policy_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_SECURITY_POLICY_H__

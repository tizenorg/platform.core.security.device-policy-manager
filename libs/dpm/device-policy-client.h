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

#ifndef __DEVICE_POLICY_CLIENT_H__
#define __DEVICE_POLICY_CLIENT_H__

#include <tizen.h>

/**
 * @file device-policy-client.h
 * @brief This file defines common data types required to device policy APIs.
 */

#ifndef DPM_API
#define DPM_API __attribute__((visibility("default")))
#endif // API

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_POLICY_CLIENT_MODULE
 * @{
 */

/**
 * @brief       The Device Policy Client handle
 * @details     The Device Policy Client Handle is an abstraction of the
 *              logical connection between the device policy manager and
 *              it's client. The Device Policy Client handle must be
 *              created by using dpm_create_client() before attempting to
 *              use almost any of the device policy APIs, and it should
 *              be freed when interaction with the Device Policy Manager
 *              is no longer required.
 *              To release the handle, use dpm_destroy_client().
 * @since_tizen 3.0
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 */
typedef void* dpm_client_h;

/**
 * @brief       Enumeration of device policy API errors
 * @since_tizen 3.0
 */
typedef enum {
    DPM_ERROR_NONE                 = TIZEN_ERROR_NONE,                 /**< The operation was successful */
    DPM_ERROR_INVALID_PARAMETER    = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
    DPM_ERROR_CONNECTION_REFUSED   = TIZEN_ERROR_CONNECTION_REFUSED,   /**< Connection refused */
    DPM_ERROR_TIMED_OUT            = TIZEN_ERROR_TIMED_OUT,            /**< Timeout */
    DPM_ERROR_PERMISSION_DENIED    = TIZEN_ERROR_PERMISSION_DENIED,    /**< Access privilege is not sufficient */
    DPM_ERROR_NOT_SUPPORTED        = TIZEN_ERROR_NOT_SUPPORTED,        /**< Operation is not supported */
    DPM_ERROR_NO_SUCH_FILE         = TIZEN_ERROR_NO_SUCH_FILE,         /**< No such file or directory */
    DPM_ERROR_FILE_EXISTS          = TIZEN_ERROR_FILE_EXISTS,          /**< File exists */
    DPM_ERROR_OUT_OF_MEMORY        = TIZEN_ERROR_OUT_OF_MEMORY         /**< Out of memory */
} dpm_error_type_e;

/**
 * @brief       Creates the Device Policy Client handle
 * @details     This API creates device policy client handle required to
 *              the device policy APIs.
 *              This API is also used to verify whether caller is authorized
 *              or not
 * @since_tizen 3.0
 * @return      Device Policy Client handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_CONNECTION_REFUSED Connection refused
 * @pre         N/A
 * @post        N/A
 * @see         dpm_destroy_client()
 * @see         get_last_result()
 */
DPM_API dpm_client_h dpm_create_client(void);

/**
 * @brief       Releases the Device Policy Client Handle
 * @details     This API must be called if interaction with the Device
 *              Policy Manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client Handle
 * @return      None
 * @pre         The handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 */
DPM_API void dpm_destroy_client(dpm_client_h handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __DEVICE_POLICY_CLIENT_H__

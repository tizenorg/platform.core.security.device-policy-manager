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
 * @file context.h
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
 * @brief       Called when a policy is changed.
 * @since_tizen 3.0
 * @param[in]   name The name of the policy
 * @param[in]   state The current state of the policy
 * @param[in]   user_data The user data passed from dpm_context_add_policy_changed_cb
 * @see         dpm_context_add_policy_changed_cb()
 * @see         dpm_context_remove_policy_changed_cb()
 */
typedef void (*dpm_policy_changed_cb)(const char* name, const char* state, void *user_data);

/**
 * @brief       The device policy context handle
 * @details     The device policy context handle is an abstraction of the
 *              logical connection between the device policy manager and
 *              it's client. The device policy context handle must be
 *              created by using dpm_context_create() before attempting to
 *              use almost any of the device policy APIs, and it should
 *              be freed when interaction with the device policy manager
 *              is no longer required.
 *              To release the handle, use dpm_context_destroy().
 * @since_tizen 3.0
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 */
typedef void* dpm_context_h;

/**
 * @brief       Enumeration of device policy API errors
 * @since_tizen 3.0
 */
typedef enum {
    DPM_ERROR_NONE                 = TIZEN_ERROR_NONE,                 /**< The operation was successful */
    DPM_ERROR_INVALID_PARAMETER    = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
    DPM_ERROR_CONNECTION_REFUSED   = TIZEN_ERROR_CONNECTION_REFUSED,   /**< Connection refused */
    DPM_ERROR_TIMED_OUT            = TIZEN_ERROR_TIMED_OUT,            /**< Time out */
    DPM_ERROR_PERMISSION_DENIED    = TIZEN_ERROR_PERMISSION_DENIED,    /**< Access privilege is not sufficient */
    DPM_ERROR_NOT_SUPPORTED        = TIZEN_ERROR_NOT_SUPPORTED,        /**< Operation is not supported */
    DPM_ERROR_NO_SUCH_FILE         = TIZEN_ERROR_NO_SUCH_FILE,         /**< No such file or directory */
    DPM_ERROR_FILE_EXISTS          = TIZEN_ERROR_FILE_EXISTS,          /**< File exists */
    DPM_ERROR_OUT_OF_MEMORY        = TIZEN_ERROR_OUT_OF_MEMORY         /**< Out of memory */
} dpm_error_type_e;

/**
 * @brief       Creates the device policy context handle.
 * @details     This API creates device policy context handle required to
 *              the device policy APIs.
 *              This API is also used to verify whether caller is authorized
 *              or not.
 * @since_tizen 3.0
 * @return      Device policy context handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_CONNECTION_REFUSED Connection refused
 * @see         dpm_context_destroy()
 * @see         get_last_result()
 */
DPM_API dpm_context_h dpm_context_create(void);

/**
 * @brief       Releases the device policy context handle.
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle device policy context handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_create()
 * @see         dpm_context_create()
 */
DPM_API int dpm_context_destroy(dpm_context_h handle);

/**
 * @brief       Adds policy change callback to the device policy
 *              manager.
 * @details     This API can be used to subscribe policy change callback.
 *              The callback specified to this API is asynchronously called when
 *              policy is changed on runtime.
 * @since_tizen 3.0
 * @param[in]   handle device policy context handle
 * @param[in]   name Policy name to subscribe
 * @param[in]   callback The callback when policy is changed
 * @param[in]   user_data User specified data passed to the callback
 * @param[out]  id Policy change callback identifier
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_create().
 * @see         dpm_context_create()
 * @see         dpm_context_remove_policy_changed_cb()
 */
DPM_API int dpm_context_add_policy_changed_cb(dpm_context_h handle,
                                              const char* name,
                                              dpm_policy_changed_cb callback,
                                              void* user_data,
                                              int* id);
/**
 * @brief       Removes policy change callback from the device policy
 *              manager.
 * @details     This API should be called if policy change subscription is no longer
 *              required.
 * @since_tizen 3.0
 * @param[in]   handle Device policy context handle
 * @param[in]   id Policy change callback identifier
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_create().
 * @pre         The cb_handle must be created by dpm_context_add_policy_changed_cb().
 * @see         dpm_context_create()
 * @see         dpm_context_add_policy_changed_cb()
 */
DPM_API int dpm_context_remove_policy_changed_cb(dpm_context_h handle,
                                                 int id);
/**
 * @brief       Called when a zone raises a signal.
 * @since_tizen 3.0
 * @param[in]   name The zone name
 * @param[in]   object The object name triggered the signal
 * @param[in]   user_data The user data passed from dpm_zone_add_signal_cb
 * @see         dpm_zone_add_signal_cb()
 * @see         dpm_zone_remove_signal_cb()
 */
typedef void(*dpm_signal_cb)(const char* name, const char* object, void *user_data);

/**
 * @brief       Adds signal callback.
 * @details     This API can be used to receive signals raised by the device policy manager.
 *              The callback specified to this function is automatically called when
 *              the device policy manager raises signal.
 * @since_tizen 3.0
 * @param[in]   context The device policy context handle
 * @param[in]   signal The signal name to receive
 * @param[in]   callback The signal callback
 * @param[in]   user_data The user data passed to the callback function
 * @param[out]  id Signal identifier
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_create().
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_context_remove_signal_cb()
 */
DPM_API int dpm_context_add_signal_cb(dpm_context_h context, const char* signal,
                                      dpm_signal_cb callback, void* user_data,
                                      int* id);

/**
 * @brief       Removes signal callback.
 * @details     This API removes signal callback.
 * @since_tizen 3.0
 * @param[in]   context The device policy context
 * @param[in]   id Signal identifier
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The context must be created by dpm_context_create().
 * @see         dpm_context_create()
 * @see         dpm_context_destroy()
 * @see         dpm_context_add_signal_cb()
 */
DPM_API int dpm_context_remove_signal_cb(dpm_context_h context, int id);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_POLICY_CLIENT_H__ */

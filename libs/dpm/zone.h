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

#ifndef __CAPI_ZONE_POLICY_H__
#define __CAPI_ZONE_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file zone.h
 * @brief This file provides APIs to control containers
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_ZONE_POLICY_MODULE
 * @{
 */

/**
 * @brief       The Zone Policy handle
 * @details     The Zone Policy Handle is an abstraction of the
 *              logical connection between the device policy manager and
 *              it's client. The Zone Policy handle must be
 *              created by using dpm_context_create_zone_policy() before attempting to
 *              use almost any of the device policy zone APIs, and it should
 *              be freed when interaction with the Device Policy Manager
 *              is no longer required.
 *              To release the handle, use dpm_context_destroy_zone_policy().
 * @since_tizen 3.0
 * @see         dpm_context_create_zone_policy()
 * @see         dpm_context_destroy_zone_policy()
 */
typedef void* dpm_zone_policy_h;

/**
 * @brief       Creates the Zone Policy handle
 * @details     This API creates zone policy handle required to
 *              the device policy zone APIs.
 *              This API is also used to verify whether caller is authorized
 *              or not
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Context Handle
 * @return      Zone Policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_CONNECTION_REFUSED Connection refused
 * @see         dpm_context_destroy_zone_policy()
 * @see         get_last_result()
 */
DPM_API dpm_zone_policy_h dpm_context_create_zone_policy(dpm_context_h handle);

/**
 * @brief       Releases the Zone Policy Handle
 * @details     This API must be called if interaction with the Device
 *              Policy Manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle Zone Policy Handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_create_zone_policy()
 * @see         dpm_context_create_zone_policy()
 */
DPM_API int dpm_context_destroy_zone_policy(dpm_zone_policy_h handle);

/**
 * @brief       Creates a new container.
 * @details     An administrator can use this API to create container. This API
 *              can initiate UI flow, which user has to complete before the actual
 *              container can be created.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name to be created
 * @param[in]   pkgid The package id used to setup the container
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_context_create_zone_policy()
 * @see         dpm_context_create_zone_policy()
 * @see         dpm_context_destroy_zone_policy()
 * @see         dpm_zone_destroy()
 * @see         dpm_zone_create_iterator()
 */
DPM_API int dpm_zone_create(dpm_zone_policy_h handle, const char* name, const char* pkgid);

/**
 * @brief       Removes existing zone.
 * @details     If specified zone exists, it will be removed.
 *              All file system objects created for the zone will be also erased.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by dpm_context_create_zone_policy()
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_context_create_zone_policy()
 * @see         dpm_context_destroy_zone_policy()
 * @see         dpm_zone_create()
 * @see         dpm_zone_create_iterator()
 */
DPM_API int dpm_zone_destroy(dpm_zone_policy_h handle, const char* name);

/**
 * @brief       Zone list iterator handle
 * @since_tizen 3.0
 */
typedef void* dpm_zone_iterator_h;

/**
 * @brief       Gets an iterator to traverse the list of zone.
 * @details     The list contains all of created zones.
 *              The iterator indicates the begin of the list first.
 *              It can be used for getting a value in list and traversing.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @return      A new iterator handle of zone list on success, otherwise
 *              null value
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_OUT_OF_MEMORY Out of memory
 * @pre         The handle must be created by dpm_context_create_zone_policy()
 * @see         dpm_context_create_zone_policy()
 * @see         dpm_context_destroy_zone_policy()
 * @see         dpm_zone_create()
 * @see         dpm_zone_destroy()
 * @see         dpm_zone_iterator_next()
 * @see         dpm_zone_destroy_iterator()
 */
DPM_API dpm_zone_iterator_h dpm_zone_create_iterator(dpm_zone_policy_h handle);

/**
 * @brief       Get a value and move the iterator to next.
 * @details     First, API gets the value indicated by the iterator, and then
 *              API moves the iterator to the next position.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be controlled
 * @return      The value if the iterator has object, otherwise null value
 * @pre         The iter must be created by dpm_zone_create_iterator()
 * @see         dpm_zone_create_iterator()
 * @see         dpm_zone_destroy_iterator()
 */
DPM_API const char* dpm_zone_iterator_next(dpm_zone_iterator_h iter);

/**
 * @brief       Frees the iterator.
 * @details     This API frees the iterator.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The iter must be created by dpm_zone_create_iterator()
 * @see         dpm_zone_create_iterator()
 * @see         dpm_zone_iterator_next()
 */
DPM_API int dpm_zone_destroy_iterator(dpm_zone_iterator_h iter);

/*
 * @brief       Enumeration for zone state
 * @since_tizen 3.0
 */
typedef enum {
    DPM_ZONE_STATE_DEFINED      = 0x01, /**< Zone has been defined, but it is not running. */
    DPM_ZONE_STATE_RUNNING      = 0x02, /**< Zone has been started. */
    DPM_ZONE_STATE_LOCKED       = 0x03  /**< Zone has been defined, but it can not start. */
} dpm_zone_state_e;

/**
 * @brief       Gets the zone state
 * @details     The zone can have one of the three states(defined, running, locked).
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name
 * @param[out]  state The zone state
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such zone to get state
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_create_zone_policy()
 * @see         dpm_context_create_zone_policy()
 * @see         dpm_context_destroy_zone_policy()
 * @see         dpm_zone_create()
 * @see         dpm_zone_destroy()
 * @see         dpm_zone_add_signal_cb()
 * @see         dpm_zone_remove_signal_cb()
 */
DPM_API int dpm_zone_get_state(dpm_zone_policy_h handle, const char *name, dpm_zone_state_e *state);

/*
 * @brief       Enumeration for zone signal
 * @since_tizen 3.0
 */
typedef enum {
    DPM_ZONE_SIGNAL_CREATE        = 0x001, /**< Zone setup process has been finished. */
    DPM_ZONE_SIGNAL_DESTROY       = 0x008, /**< Zone has been removed. */
} dpm_zone_signal_e;

/**
 * @brief       Called when a zone raises a signal
 * @since_tizen 3.0
 * @param[in]   signal The signal from the container
 * @param[in]   name The name of the container
 * @param[in]   info The information of the signal
 * @param[in]   user_data The user data passed from dpm_zone_add_signal_cb
 * @see         dpm_zone_add_signal_cb()
 * @see         dpm_zone_remove_signal_cb()
 */
typedef void(*dpm_zone_signal_cb)(dpm_zone_signal_e signal, const char* name, const char* info, void *user_data);

/**
 * @brief       Adds zone signal callback.
 * @details     This API can be used to receive zone signal. The callback specified to
 *              this function is automatically called when a zone raises signal.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   signal The signal of the container to be monitored
 * @param[in]   callback The zone signal callback
 * @param[in]   user_data The user data passed to the callback function
 * @return      Zone signal callback id on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_create_zone_policy()
 * @see         dpm_context_create_zone_policy()
 * @see         dpm_context_destroy_zone_policy()
 * @see         dpm_zone_remove_signal_cb()
 */
DPM_API int dpm_zone_add_signal_cb(dpm_zone_policy_h handle, dpm_zone_signal_e signal, dpm_zone_signal_cb callback, void* user_data);

/**
 * @brief       Removes zone signal callback.
 * @details     This API removes signal callback.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   id Zone signal callback id
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_create_zone_policy()
 * @pre         The id must be created by dpm_zone_add_signal_cb() 
 * @see         dpm_context_create_zone_policy()
 * @see         dpm_context_destroy_zone_policy()
 * @see         dpm_zone_add_signal_cb()
 */
DPM_API int dpm_zone_remove_signal_cb(dpm_zone_policy_h handle, int id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_ZONE_POLICY__ */

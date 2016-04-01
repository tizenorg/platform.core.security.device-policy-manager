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
 * @brief This file provides APIs to control app containers
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_ZONE_POLICY_MODULE
 * @{
 */

/**
 * @brief       API to create a new application container.
 * @details     This API calls zone admin package to start zone provisioning
 *              process according to the enterprise policy. Thus, the zone
 *              admin package should be able to dispatch zone policy which are
 *              required to configure the zone.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone name to be created
 * @param[in]   pkgid The package id to be run for zone creation
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_zone()
 * @see         dpm_zone_list_get_iterator()
 */
DPM_API int dpm_create_zone(dpm_client_h handle, const char* name, const char* pkgid);

/**
 * @brief       API to remove existing zone.
 * @details     If specified zone exists, it will be removed.
 *              All of directories of zone will be also erased.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone name to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_zone()
 * @see         dpm_zone_list_get_iterator()
 */
DPM_API int dpm_remove_zone(dpm_client_h handle, const char* name);

/**
 * @brief       Zone list iterator handle
 * @since_tizen 3.0
 */
typedef void* dpm_zone_iterator_h;

/**
 * @brief       API to get an iterator to traverse the list of zone.
 * @details     The list contains all of created zones.
 *              The iterator indicates the begin of the list first.
 *              It can be used for getting a value in list and traversing.
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @return      A new iterator handle of zone list on success, otherwise
 *              null value
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_OUT_OF_MEMORY Out of memory
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_create_zone()
 * @see         dpm_remove_zone()
 * @see         dpm_zone_iterator_next()
 * @see         dpm_free_zone_iterator()
 */
DPM_API dpm_zone_iterator_h dpm_get_zone_iterator(dpm_client_h handle);

/**
 * @brief       API to get a value and move the iterator to next.
 * @details     First, API gets the value indicated by the iterator, and then
 *              API moves the iterator to the next position.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be controlled
 * @return      The value if the iterator has object, otherwise null value
 * @pre         iter must be created by dpm_zone_list_get_iterator()
 * @post
 * @see         dpm_zone_list_get_iterator()
 * @see         dpm_zone_free_iterator()
 */
DPM_API const char* dpm_zone_iterator_next(dpm_zone_iterator_h iter);

/**
 * @brief       API to free the iterator.
 * @details     The iterator and what it indicates are freed from memory.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be removed
 * @pre         iter must be created by dpm_zone_list_get_iterator()
 * @see         dpm_get_zone_iterator()
 * @see         dpm_zone_iterator_next()
 */
DPM_API void dpm_free_zone_iterator(dpm_zone_iterator_h iter);

/*
 * @brief       Enumeration for zone state
 * @since_tizen 3.0
 */
typedef enum {
    DPM_ZONE_STATE_DEFINED, /**< Zone has been defined, but it is not running. */
    DPM_ZONE_STATE_RUNNING, /**< Zone has been started. */
    DPM_ZONE_STATE_LOCKED   /**< Zone has been defined, but it can not start. */
} dpm_zone_state_e;

/**
 * @brief       API to get the zone state
 * @details     The zone can have one of the three states(defined, running, locked).
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone name
 * @param[out]  state The zone state
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such zone to get state
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_create_zone()
 * @see         dpm_remove_zone()
 * @see         dpm_subscribe_zone_signal()
 * @see         dpm_unsubscribe_zone_signal()
 */
DPM_API int dpm_get_zone_state(dpm_client_h handle, const char *name, dpm_zone_state_e *state);

/**
 * @brief       Called when a zone signal occurs
 * @since_tizen 3.0
 */
typedef void(*dpm_zone_signal_cb)(dpm_zone_state_e event, const char* name, void *info, void *user_data);

/**
 * @brief       API to attach a listener to get zone signal.
 * @details     Each zone signals are sent when zone state is changed.
 *              To catch the events, listener should be added in advance.
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @param[in]   callback The listener function to be called
 * @param[in]   user_data The user data passed to the listener function
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_subscribe_zone_signal()
 */
DPM_API int dpm_subscribe_zone_signal(dpm_client_h handle, dpm_zone_signal_cb callback, void* user_data);

/**
 * @brief       API to detach the listener from zone signal.
 * @details     After removed, listener function will be no longer called even
 *              though zone state is changed.
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @param[in]   callback The listener function to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_unsubscribe_zone_signal()
 */
DPM_API int dpm_unsubscribe_zone_signal(dpm_client_h handle, dpm_zone_signal_cb callback);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_ZONE_POLICY__

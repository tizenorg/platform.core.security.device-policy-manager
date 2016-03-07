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

#ifndef __CAPI_ZONE_POLICY__
#define __CAPI_ZONE_POLICY__

#include <dpm/device-policy-client.h>

/**
 * @file zone.h
 * @brief This file provides APIs to control zone functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  DPM_ZONE_POLICY
 * @{
 */

/**
 * @brief       API to create a new application zone.
 * @details     Zone creation process is a two step process, where
 *              the API call just initiates a zone creation package
 *              process (such as, setup-wizard) and doesn't actually
 *              creates a zone.
 * @remark
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone name to be created
 * @param[in]   pkgid The package id to be run for zone creation
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_zone()
 * @see         dpm_zone_list_get_iterator()
 */
DPM_API int dpm_create_zone(dpm_client_h handle, const char* name, const char* pkgid);

/**
 * @brief       API to remove existing zone.
 * @details     If specified zone exists, it will be removed.
                All of directories of zone will be also destroyed.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone name to be created
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_zone()
 * @see         dpm_zone_list_get_iterator()
 */
DPM_API int dpm_remove_zone(dpm_client_h handle, const char* name);

/**
 * @brief       Zone list iterator handle
 */
typedef void* dpm_zone_iterator_h;

/**
 * @brief       API to get an iterator to traverse the list of zone.
 * @details     The list contains all of created zones.
 *              The iterator indicates the begin of the list first.
                It can be used for getting a value in list and traversing.
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @return      A new iterator handle of zone list on success, otherwise
                null value
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_create_zone()
 * @see         dpm_remove_zone()
 * @see         dpm_zone_iterator_next()
 * @see         dpm_free_zone_iterator()
 */
DPM_API dpm_zone_iterator_h dpm_get_zone_iterator(dpm_client_h handle);

/**
 * @brief       API to get a value and move the iterator to next iterator.
 * @details     First, API gets the value indicated by the iterator, and then
                API moves the iterator to the next position.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be controlled
 * @return      The vale indicated by iterator on success, otherwise null value
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
 * @param[in]   iter The interator to be removed
 * @return      none
 * @pre         iter must be created by dpm_zone_list_get_iterator()
 * @post
 * @see         dpm_get_zone_iterator()
 * @see         dpm_zone_iterator_next()
 */
DPM_API void dpm_free_zone_iterator(dpm_zone_iterator_h iter);

/*
 * @brief       Enumeration for zone state
 */
typedef enum {
    DPM_ZONE_ACTIVE,
    /**< Zone can be used. */
    DPM_ZONE_CREATION_IN_PROGRESS,
    /**< Zone is on provisioning */
    DPM_ZONE_CREATION_REQUIRES_MANIFEST,
    /**< In creation, zone is requiring the manifest of itself */
    DPM_ZONE_REMOVE_IN_PROGRESS,
    /**< Zone is destroying */
    DPM_ZONE_LOCKED,
    /**< Zone was locked. */
    DPM_ZONE_DOESNT_EXIST,
    /**< Zone doesn't exist. */
} zone_state_e;

/**
 * @brief       API to get the zone state
 * @details     The zone state is 3 state (active, creating, locked).
                and it also presets that there is no specified zone.
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone state
 * @return      #DPM_ZONE_DOESNT_EXISTS if zone doesn't exist,
                otherwise current zone state.
 * @retval      #DPM_ZONE_ACTIVE Zone can be used.
 * @retval      #DPM_ZONE_CREATION_IN_PROGRESS Zone is creating.
 * @retval      #DPM_ZONE_REMOVE_IN_PROGRESS Zone is removing.
 * @retval      #DPM_ZONE_LOCKED zone was locked.
 * @retval      #DPM_ZONE_DOESNT_EXIST zone doesn't exist.
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_create_zone()
 * @see         dpm_remove_zone()
 * @see         dpm_subscribe_zone_signal()
 * @see         dpm_unsubscribe_zone_signal()
 */
DPM_API zone_state_e dpm_get_zone_state(dpm_client_h handle, const char *name);

/**
 * @brief       Called when a zone signal occurs
 */
typedef void(*dpm_zone_signal_cb)(zone_state_e event, const char* name, void *info, void *user_data);

/**
 * @brief       API to attach a listener to get zone signal.
 * @details     Each zone signals are sent when zone state is changed.
                To catch the events, listener should be added in advance.
 * @since_tizen 3.0
 * @param[in]   handle the device policy client handle
 * @param[in]   callback The listener function to be called
 * @param[in]   user_data The user data passed to the listener function
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_subscribe_zone_signal()
 */
DPM_API int dpm_subscribe_zone_signal(dpm_client_h handle, dpm_zone_signal_cb callback, void* user_data);

/**
 * @brief       API to detach the listener from zone signal.
 * @details     After removed, listner function will be no longer called even
                though zone state is changed.
 * @since_tizen 3.0
 * @param[in]   handle the device policy client handle
 * @param[in]   callback The listener function to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
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

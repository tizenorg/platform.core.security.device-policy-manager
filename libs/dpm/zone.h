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
 * @brief       Creates a new container.
 * @details     An administrator can use this API to create container. This API
 *              can initiate UI flow, which user has to complete before the actual
 *              container can be created.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone name to be created
 * @param[in]   pkgid The package id used to setup the container
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
 * @brief       Removes existing zone.
 * @details     If specified zone exists, it will be removed.
 *              All file system objects created for the zone will be also erased.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The zone name to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
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
 * @brief       Gets an iterator to traverse the list of zone.
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
 * @brief       Get a value and move the iterator to next.
 * @details     First, API gets the value indicated by the iterator, and then
 *              API moves the iterator to the next position.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be controlled
 * @return      The value if the iterator has object, otherwise null value
 * @pre         iter must be created by dpm_zone_list_get_iterator()
 * @see         dpm_zone_list_get_iterator()
 * @see         dpm_zone_free_iterator()
 */
DPM_API const char* dpm_zone_iterator_next(dpm_zone_iterator_h iter);

/**
 * @brief       Frees the iterator.
 * @details     This API frees the iterator.
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
    DPM_ZONE_STATE_DEFINED      = 0x01, /**< Zone has been defined, but it is not running. */
    DPM_ZONE_STATE_RUNNING      = 0x02, /**< Zone has been started. */
    DPM_ZONE_STATE_LOCKED       = 0x03  /**< Zone has been defined, but it can not start. */
} dpm_zone_state_e;

/**
 * @brief       Gets the zone state
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
 * @see         dpm_add_zone_signal_cb()
 * @see         dpm_remove_zone_signal_cb()
 */
DPM_API int dpm_get_zone_state(dpm_client_h handle, const char *name, dpm_zone_state_e *state);

/*
 * @brief       Enumeration for zone signal
 * @since_tizen 3.0
 */
typedef enum {
    DPM_ZONE_SIGNAL_SETUP_SUCCESS           = 0x001, /**< Zone setup process has been succeed. */
    DPM_ZONE_SIGNAL_SETUP_FAILURE           = 0x002, /**< Zone setup process has been failed. */
    DPM_ZONE_SIGNAL_CANCELLED               = 0x004, /**< Zone setup process has been cancelled. */
    DPM_ZONE_SIGNAL_REMOVED                 = 0x008, /**< Zone has been removed. */
    DPM_ZONE_SIGNAL_PKG_INSTALL_SUCCESS     = 0x010, /**< Package installation has been succeed in the zone. */
    DPM_ZONE_SIGNAL_PKG_INSTALL_FAILURE     = 0x020, /**< Package installation has been failed in the zone. */
    DPM_ZONE_SIGNAL_PKG_UNINSTALL_SUCCESS   = 0x040, /**< Package uninstallation has been succeed in the zone. */
    DPM_ZONE_SIGNAL_PKG_UNINSTALL_FAILURE   = 0x080, /**< Package uninstallation has been failed in the zone */
    DPM_ZONE_SIGNAL_CHANGE_PWD_SUCCESS      = 0x100, /**< Changing password has been succeed in the zone. */
    DPM_ZONE_SIGNAL_CHANGE_PWD_FAILURE      = 0x200, /**< Changing password has been failed in the zone. */
    DPM_ZONE_SIGNAL_VERIFY_PWD_SUCCESS      = 0x400, /**< Verifying password has been succeed in the zone. */
    DPM_ZONE_SIGNAL_VERIFY_PWD_FAILURE      = 0x800  /**< Verifying password has been failed in the zone. */
} dpm_zone_signal_e;

/**
 * @brief       Called when a zone raises a signal
 * @since_tizen 3.0
 * @param[in]   name The name of the container
 * @param[in]   signal The signal from the container
 * @param[in]   user_data The user data passed from dpm_add_zone_signal_handler
 * @see         dpm_add_zone_signal_handler()
 * @see         dpm_remove_zone_signal_handler()
 */
typedef void(*dpm_zone_signal_cb)(const char* name, dpm_zone_signal_e signal, void *user_data);

/**
 * @brief       Adds zone signal handler.
 * @details     This API can be used to receive zone signal. The handler specified to
 *              this function is automatically called when a zone raises signal.
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @param[in]   signals The signals of the container to be monitored
 * @param[in]   handler The zone signal handler
 * @param[in]   user_data The user data passed to the listener function
 * @return      Zone signal handler id on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_zone_signal_handler()
 */
DPM_API int dpm_add_zone_signal_handler(dpm_client_h handle, dpm_zone_signal_e signals, dpm_zone_signal_cb handler, void* user_data);

/**
 * @brief       Removes zone signal handler.
 * @details     This API removes signal handler.
 * @since_tizen 3.0
 * @param[in]   handle The device policy client handle
 * @param[in]   id Zone signal handler id
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_add_zone_signal_handler()
 */
DPM_API int dpm_remove_zone_signal_handler(dpm_client_h handle, int id);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_ZONE_POLICY__

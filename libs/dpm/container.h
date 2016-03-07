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

#ifndef __CAPI_CONTAINER_POLICY__
#define __CAPI_CONTAINER_POLICY__

#include <dpm/device-policy-client.h>

/**
 * @file container.h
 * @brief This file provides APIs to control container functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  DPM_CONTAINER_POLICY
 * @{
 */

/**
 * @brief       API to create a new application container.
 * @details     Container creation process is a two step process, where
 *              the API call just initiates a container creation package
 *              process (such as, setup-wizard) and doesn't actually
 *              creates a container.
 * @remark
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The container name to be created
 * @param[in]   pkgid The package id to be run for container creation
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_container()
 * @see         dpm_container_list_get_iterator()
 */
DPM_API int dpm_create_container(dpm_client_h handle, const char* name, const char* pkgid);

/**
 * @brief       API to remove existing container.
 * @details     If specified container exists, it will be removed.
                All of directories of container will be also destroyed.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The container name to be created
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @pre         The container corresponding to the given name must be
 *              created before use of this API.
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_container()
 * @see         dpm_container_list_get_iterator()
 */
DPM_API int dpm_remove_container(dpm_client_h handle, const char* name);

/**
 * @brief       API to lock existing container.
 * @details     If specified container exists, it will be locked.
 *              If the container is locked, it can't be run.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The container name to be created
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @pre         The container corresponding to the given name must be
 *              created before use of this API.
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_unlock_container()
 */
DPM_API int dpm_lock_container(dpm_client_h handle, const char* name);

/**
 * @brief       API to unlock container already locked.
 * @details     If specified container exists and is already locked,
 *              it will be un locked.
 * @since_tizen 3.0
 * @privlevel   public
 * @privelege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The device policy client handle
 * @param[in]   name The container name to be created
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVLAID Invalid parameter
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @pre         The container corresponsing to the given name must be in
 *              locked state
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_lock_container()
 */
DPM_API int dpm_unlock_container(dpm_client_h handle, const char* name);

/**
 * @brief       Container list iterator handle
 */
typedef struct dpm_container_list_iterator_s* dpm_container_list_iterator_h;

/**
 * @brief       API to get an iterator to traverse the list of container.
 * @details     The list contains all of created containers.
 *              The iterator indicates the begin of the list first.
                It can be used for getting a value in list and traversing.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle The device policy client handle
 * @return      A new iterator handle of container list on success, otherwise
                null value
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_create_container()
 * @see         dpm_remove_container()
 * @see         dpm_container_iterator_next()
 * @see         dpm_container_free_iterator()
 */
DPM_API dpm_container_list_iterator_h container_list_get_iterator(dpm_client_h handle);

/**
 * @brief       API to get a value and move the iterator to next iterator.
 * @details     First, API gets the value indicated by the iterator And then, 
                API moves the iterator to the next position.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   iter The iterator to be controlled
 * @return      The vale indicated by iterator on success, otherwise null value
 * @pre         iter must be created by dpm_container_list_get_iterator()
 * @post
 * @see         dpm_container_list_get_iterator()
 * @see         dpm_container_free_iterator()
 */
DPM_API const char* dpm_container_iterator_next(dpm_container_list_iterator_h iter);

/**
 * @brief       API to free the iterator.
 * @details     The iterator and what it indicates are freed from memory.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   iter The interator to be removed
 * @return      none
 * @pre         iter must be created by dpm_container_list_get_iterator()
 * @post
 * @see         dpm_container_list_get_iterator()
 * @see         dpm_container_iterator_next()
 */
DPM_API void dpm_container_free_iterator(dpm_container_list_iterator_h iter);

/*
 * @brief       Enumeration for container state
 */
enum container_state_e {
    DPM_CONTAINER_ACTIVE,
    /**< Container can be used. */
    DPM_CONTAINER_CREATION_IN_PROGRESS,
    /**< Container is on provisioning */
    DPM_CONTAINER_LOCKED,
    /**< Container was locked. */
    DPM_CONTAINER_DOESNT_EXISTS,
    /**< Container doesn't exist. */
};

/**
 * @brief       API to get the container state
 * @details     The container state is 3 state (active, creating, locked).
                and it also presets that there is no specified container.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle The device policy client handle
 * @param[in]   name The container state
 * @return      #DPM_CONTAINER_DOESNT_EXISTS if container doesn't exist,
                otherwise current container state.
 * @retval      #DPM_CONTAINER_ACTIVE Container can be used.
 * @retval      #DPM_CONTAINER_CREATION_IN_PROGRESS Container is creating.
 * @retval      #DPM_CONTAINER_LOCKED container was locked.
 * @retval      #DPM_CONTAINER_DOESNT_EXIST container doesn't exist.
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_create_container()
 * @see         dpm_remove_container()
 * @see         dpm_container_list_get_iterator()
 */
DPM_API container_state_e dpm_container_get_state(dpm_client_h handle, const char *name);

/*
 * @brief       Enumeration for container provisioning event.
 */
enum container_provisioning_event_e {
    DPM_CONTAINER_PROVISION_START,
    /**< Provisioning was started. */
    DPM_CONTAINER_PROVISION_REQUIRES_MANIFEST,
    /**< Manifest is needed for next steps.*/
    DPM_CONTAINER_PROVISION_CREATED_DIRECTORIES,
    /**< Directories for container are created. */
    DPM_CONTAINER_PROVISION_EXECUTED_HOOK,
    /**< Container hook was executed */
    DPM_CONTAINER_PROVISION_UNLOCKED,
    /**< Container was ready to start by unlocking*/
    DPM_CONTAINER_PROVISION_COMPLETED,
    /**< All the provisioning step was completed */
};

/**
 * @brief       Called when container provisioning event is received.
 */
typedef void(*dpm_container_provisioning_cb)(container_provisioning_event_e result, const char* name, const char* pkgid, void *user_data);

/**
 * @brief       API to attach a listener to get container provisioning event.
 * @details     Container provisioning events are sent as result when creation
                process is running. To catch the events, listener should be
                added in advance.
 * @since_tizen 3.0
 * @privlevel   public
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
 * @see         dpm_remove_container_provisioning_listener()
 */
DPM_API int dpm_add_container_provisioning_listener(dpm_client_h handle, dpm_container_provisioning_cb callback, void* user_data);

/**
 * @brief       API to detach the listener from container provisioning event.
 * @details     After removed, listner function will be no longer called even
                though container provisioning events is sent.
 * @since_tizen 3.0
 * @privlevel   public
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
 * @see         dpm_add_container_provisioning_listener()
 */
DPM_API int dpm_remove_container_provisioning_listener(dpm_client_h handle, dpm_container_provisioning_cb callback);


/**
 * @brief       Called when container state is changed
 */
typedef void(*dpm_container_state_cb)(container_state_e event, const char* name, void *user_data);

/**
 * @brief       API to attach a listener to get container event.
 * @details     Container events are sent when container state is changed.
                To catch the events, listener should be added in advance.
 * @since_tizen 3.0
 * @privlevel   public
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
 * @see         dpm_remove_container_provisioning_listener()
 */
DPM_API int dpm_add_container_state_listener(dpm_client_h handle, dpm_container_provisioning_cb callback, void* user_data);

/**
 * @brief       API to detach the listener from container event.
 * @details     After removed, listner function will be no longer called even
                though container state is changed.
 * @since_tizen 3.0
 * @privlevel   public
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
 * @see         dpm_add_container_provisioning_listener()
 */
DPM_API int dpm_remove_container_state_listener(dpm_client_h handle, dpm_container_provisioning_cb callback);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_CONTAINER_POLICY__

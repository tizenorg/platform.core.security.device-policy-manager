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
 * @brief This file provides APIs to control zones
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_ZONE_POLICY_MODULE
 * @{
 */

/**
 * @brief       The zone policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 */
typedef void* dpm_zone_policy_h;

/**
 * @brief       Acquires the zone policy handle
 * @details     This API acquires zone policy handle required to call
 *              the zone policy APIs.
 * @since_tizen 3.0
 * @param[in]   handle Device policy context handle
 * @return      Zone policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_CONNECTION_REFUSED Connection refused
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @exception   #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to enforce zone policies.
 * @see         dpm_context_release_zone_policy()
 * @see         get_last_result()
 */
DPM_API dpm_zone_policy_h dpm_context_acquire_zone_policy(dpm_context_h handle);

/**
 * @brief       Releases the zone policy handle
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy()
 * @see         dpm_context_acquire_zone_policy()
 */
DPM_API int dpm_context_release_zone_policy(dpm_zone_policy_h handle);

/**
 * @brief       Creates a new zone.
 * @details     An administrator can use this API to create a container. Once the container
 *              is created, the admin package given to the parameter will be installed inside
 *              the container. Then ownership of the container gets transferred to the admin
 *              package from the client which triggered the container creation process.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name to be created
 * @param[in]   pkgname Admin package container ownership will be transferred
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_destroy()
 * @see         dpm_zone_create_iterator()
 */
DPM_API int dpm_zone_create(dpm_zone_policy_h handle, const char* name, const char* pkgname);

/**
 * @brief       Removes existing zone.
 * @details     Administrator can use this API to remove zone. All file system objects
 *              created for the zone will be also erased.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.zone
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_create()
 * @see         dpm_zone_create_iterator()
 */
DPM_API int dpm_zone_destroy(dpm_zone_policy_h handle, const char* name);

/**
 * @brief       The zone list iterator handle
 * @since_tizen 3.0
 * @see         dpm_zone_create_iterator()
 * @see         dpm_zone_iterator_next()
 * @see         dpm_zone_destroy_iterator()
 */
typedef void* dpm_zone_iterator_h;

/**
 * @brief       Creates a zone list iterator.
 * @details     The zone list iterator can be used to get all defined zones.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @return      A zone list iterator on success, otherwise
 *              null value
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_OUT_OF_MEMORY Out of memory
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_create()
 * @see         dpm_zone_destroy()
 * @see         dpm_zone_iterator_next()
 * @see         dpm_zone_destroy_iterator()
 * @see         get_last_result()
 */
DPM_API dpm_zone_iterator_h dpm_zone_create_iterator(dpm_zone_policy_h handle);

/**
 * @brief       Fetches a zone name and forwards the iterator.
 * @details     This API returns zone name indicated by the iterator, and then
 *              the iterator is moved to the next position. If the iterator reaches
 *              the end of the list, null value will be returned.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be controlled
 * @param[out]  zone_name The zone name got from the iterator
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The iter must be created by dpm_zone_create_iterator().
 * @see         dpm_zone_create_iterator()
 * @see         dpm_zone_destroy_iterator()
 */
DPM_API int dpm_zone_iterator_next(dpm_zone_iterator_h iter, const char ** zone_name);

/**
 * @brief       Frees the iterator.
 * @details     This API frees the iterator. This API must be called if the iterator
 *              no longer used.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The iter must be created by dpm_zone_create_iterator()
 * @see         dpm_zone_create_iterator()
 * @see         dpm_zone_iterator_next()
 */
DPM_API int dpm_zone_destroy_iterator(dpm_zone_iterator_h iter);

/**
 * @brief       Called to get all the name of created zones.
 * @since_tizen 3.0
 * @param[in]   name The zone name
 * @param[in]   user_data The user data passed from dpm_zone_foreach_name
 * @see         dpm_zone_foreach_name()
 */
typedef void(*dpm_zone_foreach_cb)(const char* name, const char *user_data);

/**
 * @brief       Retrieves all the name of created zones
 * @details     This API calls dpm_zone_foreach_cb() once for each zone name
 *              with traversing the created zones list.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_create()
 * @see         dpm_zone_destroy()
 */
DPM_API int dpm_zone_foreach_name(dpm_zone_policy_h handle,
                                  dpm_zone_foreach_cb callback, void* user_data);

/*
 * @brief       Enumeration for zone state
 * @since_tizen 3.0
 */
typedef enum {
    DPM_ZONE_STATE_DEFINED      = 0x01, /**< ZonePolicy has been defined, but it is not running. */
    DPM_ZONE_STATE_RUNNING      = 0x02, /**< ZonePolicy has been started. */
    DPM_ZONE_STATE_LOCKED       = 0x03  /**< ZonePolicy has been defined, but it can not start. */
} dpm_zone_state_e;

/**
 * @brief       Gets the zone state.
 * @details     This API can be used to get the state of the zone. The zone can
 *              have one of the three states(defined, running, locked).
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name
 * @param[out]  state The zone state
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such zone to get state
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_create()
 * @see         dpm_zone_destroy()
 * @see         dpm_zone_add_signal_cb()
 * @see         dpm_zone_remove_signal_cb()
 */
DPM_API int dpm_zone_get_state(dpm_zone_policy_h handle, const char *name, dpm_zone_state_e *state);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_ZONE_POLICY__ */

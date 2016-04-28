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

#include <dpm/context.h>

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
 * @see         dpm_context_release_zone_policy()
 * @see         get_last_result()
 */
DPM_API dpm_zone_policy_h dpm_context_acquire_zone_policy(dpm_context_h handle);

/**
 * @brief       Releases the zone policy handle
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The device policy context
 * @param[in]   handle The zone policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy()
 * @see         dpm_context_acquire_zone_policy()
 */
DPM_API int dpm_context_release_zone_policy(dpm_context_h context, dpm_zone_policy_h handle);

/**
 * @brief       Creates a new zone.
 * @details     An administrator can use this API to create a container. Once the container
 *              is created, the admin package given to the parameter will be installed inside
 *              the container. Then ownership of the container gets transferred to the admin
 *              package from the client which triggered the container creation process.
 * @since_tizen 3.0
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
DPM_API int dpm_zone_iterator_next(dpm_zone_iterator_h iter, const char** zone_name);

/**
 * @brief       Frees the zone iterator.
 * @details     This API frees the zone iterator. This API must be called
 *              if the iterator no longer used.
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
typedef void(*dpm_zone_foreach_cb)(const char* name, void *user_data);

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
 */
DPM_API int dpm_zone_get_state(dpm_zone_policy_h handle, const char* name, dpm_zone_state_e *state);

/**
 * @brief       The zone package handle
 * @since_tizen 3.0
 * @see         dpm_zone_pkg_iterator_next()
 * @see         dpm_zone_foreach_pkg_id()
 * @see         dpm_zone_pkg_acquire()
 * @see         dpm_zone_pkg_release()
 */
typedef void* dpm_zone_pkg_h;

/**
 * @brief       Acquires the handle of package in the zone
 * @details     This API acquires zone package handle required to get information
 *              of the pacakge in the zone
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name
 * @param[in]   pkgid The package ID
 * @return      Zone package handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_pkg_release()
 * @see         package_manager_get_package_info()
 * @see         get_last_result()
 */
DPM_API dpm_zone_pkg_h dpm_zone_acquire_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgid);

/**
 * @brief       Releases the handle of package in the zone
 * @details     This API releases the zone package handle. This API must be called
 *              if the handle no longer used.
 * @since_tizen 3.0
 * @param[in]   pkg The package handle
 * @return      Zone package handle on success, otherwise NULL
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_zone_pkg_acquire()
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_pkg()
 */
DPM_API int dpm_zone_pkg_release(dpm_zone_pkg_h pkg);

/**
 * @brief       The zone package list iterator handle
 * @since_tizen 3.0
 * @see         dpm_zone_pkg_create_iterator()
 * @see         dpm_zone_pkg_iterator_next()
 * @see         dpm_zone_pkg_destroy_iterator()
 */
typedef void* dpm_zone_pkg_iterator_h;

/**
 * @brief       Creates a package list iterator for the installed in the zone.
 * @details     The package list iterator can be used to get all installed
 *              package IDs in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name
 * @return      A package ID list iterator on success, otherwise
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
 * @see         dpm_zone_pkg_iterator_next()
 * @see         dpm_zone_pkg_destroy_iterator()
 * @see         get_last_result()
 */
DPM_API dpm_zone_pkg_iterator_h dpm_zone_pkg_create_iterator(dpm_zone_policy_h handle, const char *name);

/**
 * @brief       Fetches a package hanndle and forwards the iterator.
 * @details     This API returns package handle indicated by the iterator, and then
 *              the iterator is moved to the next position. If the iterator reaches
 *              the end of the list, null value will be returned.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be controlled
 * @param[out]  pkgid The package handle got from the iterator
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The iter must be created by dpm_zone_create_iterator().
 * @see         dpm_zone_pkg_create_iterator()
 * @see         dpm_zone_pkg_destroy_iterator()
 */
DPM_API int dpm_zone_pkg_iterator_next(dpm_zone_iterator_h iter, dpm_zone_pkg_h* result);

/**
 * @brief       Frees the zone package iterator.
 * @details     This API frees the zone package iterator. This API must be called
 *              if the iterator no longer used.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The iter must be created by dpm_zone_create_iterator()
 * @see         dpm_zone_pkg_create_iterator()
 * @see         dpm_zone_pkg_iterator_next()
 */
DPM_API int dpm_zone_pkg_destroy_iterator(dpm_zone_iterator_h iter);

/**
 * @brief       Called to get all the handles of the installed package in the zone.
 * @since_tizen 3.0
 * @param[in]   pkg The package handle
 * @param[in]   user_data The user data passed from dpm_zone_foreach_name
 * @see         dpm_zone_foreach_name()
 */
typedef void(*dpm_zone_pkg_foreach_cb)(dpm_zone_pkg_h pkg, void *user_data);

/**
 * @brief       Retrieves all the IDs of the installed package in the zone.
 * @details     This API calls dpm_zone_pkg_foreach_cb() once for each package
 *              ID with traversing the installed package list in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         package_manager_foreach_package_info()
 */
DPM_API int dpm_zone_foreach_pkg(dpm_zone_policy_h handle, const char* name,
                                 dpm_zone_pkg_foreach_cb callback, void* user_data);

/**
 * @brief       Gets the ID of package in the zone.
 * @details     This API can be used to get the ID of package in the zone.
 * @since_tizen 3.0
 * @remarks     The @type should be freed using free().
 * @param[in]   pkg The package handle
 * @param[out]  type The package type
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such package to get type
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_get_type()
 * @see         dpm_zone_pkg_get_icon()
 * @see         dpm_zone_pkg_get_label()
 * @see         dpm_zone_pkg_get_version()
 * @see         package_info_get_id()
 */
DPM_API int dpm_zone_pkg_get_id(dpm_zone_pkg_h pkg, const char** id);

/**
 * @brief       Gets the type of package in the zone.
 * @details     This API can be used to get the type of package in the zone.
 * @since_tizen 3.0
 * @remarks     The @type should be freed using free().
 * @param[in]   pkg The package handle
 * @param[out]  type The package type
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such package to get type
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_get_id()
 * @see         dpm_zone_pkg_get_icon()
 * @see         dpm_zone_pkg_get_label()
 * @see         dpm_zone_pkg_get_version()
 * @see         package_info_get_type()
 */
DPM_API int dpm_zone_pkg_get_type(dpm_zone_pkg_h pkg, const char** type);

/**
 * @brief       Gets the the absolute path to the icon image of package in the
 *              zone.
 * @details     This API can be used to get the icon image path of package
 *              in the zone.
 * @since_tizen 3.0
 * @remarks     The @icon should be freed using free().
 * @param[in]   pkg The package handle
 * @param[out]  icon The path of package icon image
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such package to get icon image path
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_get_id()
 * @see         dpm_zone_pkg_get_type()
 * @see         dpm_zone_pkg_get_label()
 * @see         dpm_zone_pkg_get_version()
 * @see         package_info_get_icon()
 */
DPM_API int dpm_zone_pkg_get_icon(dpm_zone_pkg_h pkg, const char** icon);

/**
 * @brief       Gets the label of package in the zone.
 * @details     This API can be used to get the label of package in the zone.
 * @since_tizen 3.0
 * @remarks     The @label should be freed using free().
 * @param[in]   pkg The package handle
 * @param[out]  label The package label
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such package to get label
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_get_id()
 * @see         dpm_zone_pkg_get_type()
 * @see         dpm_zone_pkg_get_icon()
 * @see         dpm_zone_pkg_get_version()
 * @see         package_info_get_label()
 */
DPM_API int dpm_zone_pkg_get_label(dpm_zone_pkg_h pkg, const char** label);

/**
 * @brief       Gets the version of package in the zone.
 * @details     This API can be used to get the version of package in the zone.
 * @since_tizen 3.0
 * @remarks     The @version should be freed using free().
 * @param[in]   pkg The package handle
 * @param[out]  version The package version
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such package to get version
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_get_id()
 * @see         dpm_zone_pkg_get_type()
 * @see         dpm_zone_pkg_get_icon()
 * @see         dpm_zone_pkg_get_label()
 * @see         package_info_get_version()
 */
DPM_API int dpm_zone_pkg_get_version(dpm_zone_pkg_h pkg, const char** version);

/**
 * @brief       Checks whether the package in the zone is system package.
 * @details     This API can be used to check whether the package in the zone
 *              is system package.
 * @since_tizen 3.0
 * @param[in]   pkg The package handle
 * @param[out]  result true if the package is system package,
 *              otherwise false if the package is not system package
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such pacakge to check
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_is_removable()
 * @see         dpm_zone_pkg_is_preload()
 * @see         package_info_is_system_package()
 */
DPM_API int dpm_zone_pkg_is_system(dpm_zone_pkg_h pkg, int* result);

/**
 * @brief       Checks whether the package in the zone is removable package.
 * @details     This API can be used to check whether the package in the zone
 *              is removable package.
 * @since_tizen 3.0
 * @param[in]   pkg The package handle
 * @param[out]  result true if the package can be removed,
 *              otherwise false if the package can not be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such pacakge to check
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_is_system()
 * @see         dpm_zone_pkg_is_preload()
 * @see         package_info_is_removable_package()
 */
DPM_API int dpm_zone_pkg_is_removable(dpm_zone_pkg_h pkg, int* result);

/**
 * @brief       Checks whether the package in the zone is preload package.
 * @details     This API can be used to check whether the package in the zone
 *              is preload package.
 * @since_tizen 3.0
 * @param[in]   pkg The package handle
 * @param[out]  result true if the package is preloaded,
 *              otherwise false if the package is not preloaded
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such pacakge to check
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_is_system()
 * @see         dpm_zone_pkg_is_removable()
 * @see         package_info_is_preload_package()
 */
DPM_API int dpm_zone_pkg_is_preload(dpm_zone_pkg_h pkg, int* result);

/**
 * @brief       Installs the package located at the given path into the zone.
 * @details     Administrator can use this API to install the package into the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   name The zone name
 * @param[in]   pkgpath The absolute path to the package to be installed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_NO_SUCH_FILE No such package file
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_uninstall()
 * @see         package_manager_request_install()
 */
DPM_API int dpm_zone_install_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgpath);

/**
 * @brief       Uinstalls the package with the given ID from the zone.
 * @details     Administrator can use this API to uninstall the package from the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   pkg The package handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_NO_DATA No such pacakge to unintall
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_pkg_install()
 * @see         package_manager_request_uninstall()
 */
DPM_API int dpm_zone_uninstall_pkg(dpm_zone_policy_h handle, dpm_zone_pkg_h pkg);

/**
 * @brief       The zone application handle
 * @since_tizen 3.0
 * @see         dpm_zone_uiapp_iterator_next()
 * @see         dpm_zone_foreach_uiapp()
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_release_app()
 */
typedef void* dpm_zone_app_h;

/**
 * @brief       Acquires the handle of application in the zone
 * @details     This API acquires zone package handle required to control
 *              the pacakge in the zone
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   pkg The package handle
 * @param[in]   appid The Application ID
 * @return      Zone application handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @see         dpm_zone_pkg_acquire()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_app_release()
 * @see         get_last_result()
 */
DPM_API dpm_zone_pkg_h dpm_zone_acquire_app(dpm_zone_policy_h handle, dpm_zone_pkg_h pkg, const char* appid);

/**
 * @brief       Releases the handle of application in the zone
 * @details     This API releases the zone application handle. This API must be called
 *              if the handle no longer used.
 * @since_tizen 3.0
 * @param[in]   pkg The package handle
 * @return      Zone package handle on success, otherwise NULL
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_zone_app_acquire()
 * @see         dpm_zone_app_acquire()
 */
DPM_API int dpm_zone_app_release(dpm_zone_app_h app);

/**
 * @brief       The zone application list iterator handle
 * @since_tizen 3.0
 * @see         dpm_zone_pkg_uiapp_create_iterator()
 * @see         dpm_zone_pkg_uiapp_iterator_next()
 * @see         dpm_zone_pkg_uiapp_destroy_iterator()
 */
typedef void* dpm_zone_uiapp_iterator_h;

/**
 * @brief       Creates an application list iterator for the installed in the zone.
 * @details     The application list iterator can be used to get all installed
 *              application IDs in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   pkg The package handle
 * @return      An UI application ID list iterator on success, otherwise
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
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         dpm_zone_uiapp_iterator_next()
 * @see         dpm_zone_uiapp_destroy_iterator()
 * @see         get_last_result()
 */
DPM_API dpm_zone_uiapp_iterator_h dpm_zone_uiapp_create_iterator(dpm_zone_policy_h handle, dpm_zone_pkg_h pkg);

/**
 * @brief       Fetches an application ID and forwards the iterator.
 * @details     This API returns application ID indicated by the iterator, and then
 *              the iterator is moved to the next position. If the iterator reaches
 *              the end of the list, null value will be returned.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be controlled
 * @param[out]  app_id The application ID got from the iterator
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The iter must be created by dpm_zone_create_iterator().
 * @see         dpm_zone_uiapp_create_iterator()
 * @see         dpm_zone_uiapp_destroy_iterator()
 */
DPM_API int dpm_zone_uiapp_iterator_next(dpm_zone_uiapp_iterator_h iter, const char* app_id);

/**
 * @brief       Frees the UI zone application iterator.
 * @details     This API frees the UI zone application iterator. This API must be
 *              called if the iterator no longer used.
 * @since_tizen 3.0
 * @param[in]   iter The iterator to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The iter must be created by dpm_zone_create_iterator()
 * @see         dpm_zone_uiapp_create_iterator()
 * @see         dpm_zone_uiapp_iterator_next()
 */
DPM_API int dpm_zone_uiapp_destroy_iterator(dpm_zone_uiapp_iterator_h iter);

/**
 * @brief       Called to get all the IDs of the installed application in the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   app The application handle
 * @param[in]   user_data The user data passed from dpm_zone_foreach_name
 * @see         dpm_zone_foreach_uiapp()
 */
typedef void(*dpm_zone_app_foreach_cb)(dpm_zone_app_h app, void *user_data);

/**
 * @brief       Retrieves all the handle of the installed UI application in the zone.
 * @details     This API calls dpm_zone_uiapp_foreach_cb() once for each UI
 *              application handle with traversing the installed UI application list
 *              in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   pkg The package handle
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_pkg()
 * @see         dpm_zone_pkg_release()
 * @see         package_info_foreach_app_from_package()
*/
DPM_API int dpm_zone_foreach_uiapp(dpm_zone_policy_h handle,
                                   dpm_zone_pkg_h pkg,
                                   dpm_zone_app_foreach_cb callback,
                                   void* user_data);

/**
 * @brief       Gets the the ID of application in the zone.
 * @details     This API can be used to get the ID of application in the zone.
 * @since_tizen 3.0
 * @remarks     The @icon should be freed using free().
 * @param[in]   app The application handle
 * @param[out]  id The application ID
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such application to get icon image path
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_app_get_icon()
 * @see         dpm_zone_app_get_label()
 */
DPM_API int dpm_zone_app_get_id(dpm_zone_app_h app, const char** id);

/**
 * @brief       Gets the the absolute path to the icon image of application
 *              in the zone.
 * @details     This API can be used to get the icon image path of application
 *              in the zone.
 * @since_tizen 3.0
 * @remarks     The @icon should be freed using free().
 * @param[in]   app The application handle
 * @param[out]  icon The path of application icon image
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such application to get icon image path
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_app_get_id()
 * @see         dpm_zone_app_get_label()
 */
DPM_API int dpm_zone_app_get_icon(dpm_zone_app_h app, const char** icon);

/**
 * @brief       Gets the label of application in the zone.
 * @details     This API can be used to get the label of application in the zone.
 * @since_tizen 3.0
 * @remarks     The @label should be freed using free().
 * @param[in]   app The application handle
 * @param[out]  label The application label
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such application to get label
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_app_get_id()
 * @see         dpm_zone_app_get_icon()
*/
DPM_API int dpm_zone_app_get_label(dpm_zone_app_h app, const char** label);

/**
 * @brief       Checks whether the application in the zone is displayed.
 * @details     This API can be used to check whether the application in the zone
 *              is displayed.
 * @since_tizen 3.0
 * @param[in]   app The application handle
 * @param[out]  result true if the package is displayed,
 *              otherwise false if the package is not displayed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such application to check
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_app_is_taskmanaged()
 */
DPM_API int dpm_zone_app_is_nodisplayed(dpm_zone_app_h app, int* result);

/**
 * @brief       Checks whether the application in the zone can be managed by task manager.
 * @details     This API can be used to check whether the application in the zone
 *              can be managed by task manager.
 * @since_tizen 3.0
 * @param[in]   app The application handle
 * @param[out]  result true if the package can be managed by task manager,
 *              otherwise false if the package can not be managed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such application to check
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_app_is_nodisplayed()
 */
DPM_API int dpm_zone_app_is_taskmanaged(dpm_zone_app_h app, int* result);

/**
 * @brief       Launch the application located at the given path into the zone.
 * @details     Administrator can use this API to launch the application in the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   appid The application handle to be launched
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_NO_SUCH_FILE No such package file
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_resume_app()
 * @see         dpm_zone_terminate_app()
 * @see         dpm_zone_is_running_app()
 * @see         app_control_send_launch_request()
 */
DPM_API int dpm_zone_launch_app(dpm_zone_policy_h handle, dpm_zone_app_h app);

/**
 * @brief       Resume the application located at the given path into the zone.
 * @details     Administrator can use this API to resume the application in the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   appid The application handle to be resumed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_NO_SUCH_FILE No such package file
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_launch_app()
 * @see         dpm_zone_terminate_app()
 * @see         dpm_zone_is_running_app()
 */
DPM_API int dpm_zone_resume_app(dpm_zone_policy_h handle, dpm_zone_app_h app);

/**
 * @brief       Terminate the application located at the given path into the zone.
 * @details     Administrator can use this API to terminate the application in
 *              the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   appid The application handle to be terminated
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_NO_SUCH_FILE No such package file
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_launch_app()
 * @see         dpm_zone_resume_app()
 * @see         dpm_zone_is_running_app()
 * @see         app_control_send_terminate_request()
 */
DPM_API int dpm_zone_terminate_app(dpm_zone_policy_h handle, dpm_zone_app_h app);

/**
 * @brief       Checks whether the application in the zone is running.
 * @details     This API can be used to check whether the application in the zone
 *              is running.
 * @since_tizen 3.0
 * @param[in]   handle The zone policy handle
 * @param[in]   app The application handle
 * @param[out]  result true if the application is running,
 *              otherwise false if the application is not running
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NO_DATA No such pacakge to check
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_policy().
 * @see         dpm_context_acquire_zone_policy()
 * @see         dpm_context_release_zone_policy()
 * @see         dpm_zone_acquire_app()
 * @see         dpm_zone_app_release()
 * @see         dpm_zone_launch_app()
 * @see         dpm_zone_terminate_app()
 */
DPM_API int dpm_zone_is_running_app(dpm_zone_policy_h handle, dpm_zone_app_h app, int* result);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_ZONE_POLICY__ */

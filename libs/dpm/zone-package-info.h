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

#ifndef __CAPI_ZONE_PACKAGE_INFO_H__
#define __CAPI_ZONE_PACKAGE_INFO_H__

#include <package_info.h>
#include <dpm/context.h>

/**
 * @file zone-package-info.h
 * @brief This file provides APIs to get the information of packages in the zone
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_ZONE_PACKAGE_INFO_MODULE
 * @{
 */

/**
 * @brief       The zone package manager handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_zone_package_manager()
 * @see         dpm_context_release_zone_package_manager()
 */
typedef void* zone_package_manager_h;

/**
 * @brief       Acquires the zone package manager handle.
 * @details     This API acquires zone package manager handle required to call
 *              the zone package manager APIs.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager context handle
 * @return      Zone package manager handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_CONNECTION_REFUSED Connection refused
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @see         dpm_context_release_zone_package_manager()
 * @see         get_last_result()
 */
DPM_API zone_package_manager_h dpm_context_acquire_zone_package_manager(dpm_context_h handle);

/**
 * @brief       Releases the zone package manager handle.
 * @details     This API must be called if interaction with the zone package
 *              manager handle is no longer required.
 * @since_tizen 3.0
 * @param[in]   context The device policy manager context
 * @param[in]   handle The zone package manager handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_zone_package_manager()
 * @see         dpm_context_acquire_zone_package_manager()
 */
DPM_API int dpm_context_release_zone_package_manager(dpm_context_h context, zone_package_manager_h handle);

/**
 * @brief       Gets the package information for the given package in the zone.
 * @details     This API gets package information handle required to get
 *              information of the pacakge in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package manager handle
 * @param[in]   name The zone name
 * @param[in]   pkgid The package ID
 * @return      Zone package information handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @remark      The handle should be destroyed using package_info_destroy().
 * @exception   #DPM_ERROR_NONE No error
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @see         dpm_context_acquire_zone_package manager()
 * @see         dpm_context_release_zone_package manager()
 * @see         package_manager_get_package_info()
 * @see         package_manager_destroy()
 * @see         get_last_result()
 */
DPM_API int zone_package_manager_get_package_info(zone_package_manager_h handle, const char* name, const char* packageid, package_info_h* pakcage_info);

/**
 * @brief       Retrieves all the IDs of the installed package in the zone.
 * @details     This API calls package_manager_package_info_cb() once for
 *              each package information handle with traversing the installed
 *              package list in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package manager handle
 * @param[in]   name The zone name
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_context_acquire_zone_package manager().
 * @see         dpm_context_acquire_zone_package manager()
 * @see         dpm_context_release_zone_package manager()
 * @see         package_manager_foreach_package_info()
 */
DPM_API int zone_package_manager_foreach_package_info(zone_package_manager_h handle, const char* name, package_manager_package_info_cb callback, void *user_data);

/**
 * @brief       Installs the package located at the given path into the zone.
 * @details     Administrator can use this API to install the package into the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package manager handle
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
DPM_API int zone_package_manager_install(dpm_zone_package_manager_h handle, const char* name, const char* pkgpath);

/**
 * @brief       Uinstalls the package with the given ID from the zone.
 * @details     Administrator can use this API to uninstall the package from the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone package manager handle
 * @param[in]   name The zone name
 * @param[in]   pkgid The pakcage ID
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
DPM_API int zone_package_manager_uninstall(zone_package_manager_h handle, const char* name, const char* pkgid);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_ZONE_PACKAGE_INFO_H__ */

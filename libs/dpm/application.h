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

#ifndef __CAPI_APPLICATION_POLICY_H__
#define __CAPI_APPLICATION_POLICY_H__

#include <dpm/context.h>

/**
 * @file application.h
 * @brief This file provides APIs to control application functionality
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_APPLICATION_POLICY_MODULE
 * @{
 */

/**
 * @brief       The application policy handle
 * @since_tizen 3.0
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 */
typedef void* dpm_application_policy_h;

/**
 * @brief       Acquires the application policy handle.
 * @details     This API acquires application policy handle required to enforce
 *              the application policies.
 * @since_tizen 3.0
 * @param[in]   handle The device policy context handle
 * @param[in]   zone The container name
 * @return      Application policy handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #DPM_ERROR_NONE Successful
 * @exception   #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #DPM_ERROR_TIMED_OUT Time out
 * @see         dpm_context_release_application_policy()
 * @see         get_last_result()
 */
DPM_API dpm_application_policy_h dpm_context_acquire_application_policy(dpm_context_h handle, const char* zone);

/**
 * @brief       Releases the application policy handle.
 * @details     This API must be called if interaction with the device
 *              policy manager is no longer required.
 * @since_tizen 3.0
 * @param[in]   context The device policy context
 * @param[in]   handle The application policy handle
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_context_acquire_application_policy().
 * @see         dpm_context_acquire_application_policy()
 */
DPM_API int dpm_context_release_application_policy(dpm_context_h context, dpm_application_policy_h handle);

/**
 * @brief       Enumeration for application state
 * @since_tizen 3.0
 */
typedef enum {
    DPM_PACKAGE_STATE_DISABLE = 0, /**< Package disable */
    DPM_PACKAGE_STATE_ENABLE  = 1  /**< Package enable */
} dpm_package_state_e;

/**
 * @brief       Enumeration for installation mode
 * @since_tizen 3.0
 */
typedef enum {
    PACKAGE_INSTALLATION_MODE_DISALLOW = 0, /**< Package installation disable */
    PACKAGE_INSTALLATION_MODE_ALLOW  = 1    /**< Pasckage installation enable */
} dpm_package_installation_mode_e;

/**
 * @brief       Enumeration for uninstallation mode
 * @since_tizen 3.0
 */
typedef enum {
    PACKAGE_UNINSTALLATION_MODE_DISALLOW = 0, /**< Package uninstallation disable */
    package_UNINSTALLATION_MODE_ALLOW  = 1    /**< Package uninstallation enable */
} dpm_package_uninstallation_mode_e;

/**
 * @brief       Sets the default mode for application installation.
 * @details     If the mode is set to DPM_PACKAGE_INSTALLATION_MODE_DISALLOW,
 *              no application can be installed on the device.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[in]   mode The installation mode to be set, one of DPM_PACKAGE_INSTALLATION_MODE_ALLOW or
 *              DPM_PACKAGE_INSTALLATION_MODE_DISALLOW
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE No error
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_get_installation_mode()
 */
DPM_API int dpm_application_set_installation_mode(dpm_application_policy_h handle, int mode);

/**
 * @brief       Sets the default mode for application uninstallation.
 * @details     If the mode is set to DPM_PACKAGE_UNINSTALLATION_MODE_DISALLOW,
 *              no application can be uninstalled from the device.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[in]   mode The unstallation mode to be set, one of DPM_PACKAGE_UNINSTALLATION_MODE_ALLOW of
 *              DPM_PACKAGE_UNINSTALLATION_MODE_DISALLOW
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_get_uninstallation_mode()
 */
DPM_API int dpm_application_set_uninstallation_mode(dpm_application_policy_h handle, int mode);

/**
 * @brief       Gets the current installation mode for all packages.
 * @details     The default mode is to allow any package to be installed.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[out]  mode Current mode of operation, which is one of the following:
 *              #DPM_PACKAGE_INSTALLATION_MODE_ALLOW Package installation is allowed
 *              #DPM_PACKAGE_INSTALLATION_MODE_DISALLOW Package installation is not allowed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_set_installation_mode()
 */
DPM_API int dpm_application_get_installation_mode(dpm_application_policy_h handle, int *mode);

/**
 * @brief       Gets the current uninstallation mode for all packages.
 * @details     The default mode is to allow any package to be uninstalled.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[out]  mode Current mode of operation, which is one of the following:
 *              #DPM_PACKAGE_UNINSTALLATION_MODE_ALLOW Package uninstallation is allowed
 *              #DPM_PACKAGE_UNINSTALLATION_MODE_DISALLOW Package uninstallation is not allowed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_get_uninstallation_mode()
 */
DPM_API int dpm_application_get_uninstallation_mode(dpm_application_policy_h handle, int *mode);

/**
 * @brief       Enables/disables an package without installation/uninstallation.
 * @details     Administrator can silently enable/disable any package without
 *              user interaction.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[in]   pkgid The package name
 * @param[in]   state The package state
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_get_package_state()
 */
DPM_API int dpm_application_set_package_state(dpm_application_policy_h handle, const char* pkgid, dpm_package_state_e state);

/**
 * @brief       Checks whether a geiven application package is enabled or disabled
 * @details     Administrator can use this API to silently disable installation of an application
 *              without user interaction. The user can not install applications for which
 *              the administrator has disabled installation.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[in]   pkgid The package name of the application whose installation is to be disabled
 * @param[out]  state The current state of the application package
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_set_package_state()
 */
DPM_API int dpm_application_get_package_state(dpm_application_policy_h handle, const char* pkgid, int *state);

/**
 * @brief       Adds package to blacklist
 * @details     Administrator can use this API to disallow package installation
 *              corresponding to the given pkgid.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[in]   pkgid The package name to be blacklisted
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_remove_package_from_blacklist()
 * @see         dpm_application_check_package_is_blacklisted()
 */
DPM_API int dpm_application_add_package_to_blacklist(dpm_application_policy_h handle, const char* pkgid);

/**
 * @brief       Removes package from blacklist
 * @details     Administrator can use this API to remove package from blacklist.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[in]   pkgid The package name which is removed from blacklist
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_add_package_to_blacklist()
 * @see         dpm_application_check_package_is_blacklisted()
 */
DPM_API int dpm_application_remove_package_from_blacklist(dpm_application_policy_h handle, const char* pkgid);

/**
 * @brief       Checks whether a package is added to blacklist
 * @details     Administrator can use this API to check whether the package is blacklisted.
 *              Once package is added to blacklist, it is prhibited to install on the device.
 * @since_tizen 3.0
 * @param[in]   handle The application policy handle
 * @param[in]   pkgid The package name of the application
 * @param[out]  blacklisted TRUE if the package installation is disabled, else FALSE
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @pre         handle must be created by dpm_context_acquire_application_policy()
 * @see         dpm_context_acquire_application_policy()
 * @see         dpm_context_release_application_policy()
 * @see         dpm_application_add_package_to_blacklist()
 * @see         dpm_application_remove_package_from_blacklist()
 */
DPM_API int dpm_application_check_package_is_blacklisted(dpm_application_policy_h handle, const char* pkgid, int *blacklisted);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_APPLICATION_POLICY_H__

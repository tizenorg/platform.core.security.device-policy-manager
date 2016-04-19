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

#include <dpm/device-policy-client.h>

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
  * @brief       Enumeration for application state
  * @since_tizen 3.0
  */
 typedef enum {
     APPLICATION_STATE_DISABLE = 0, /**< Application disable */
     APPLICATION_STATE_ENABLE  = 1  /**< Application enable */
 } dpm_application_state_e;

 /**
  * @brief       Enumeration for installation mode
  * @since_tizen 3.0
  */
 typedef enum {
     PACKAGE_INSTALLATION_MODE_DISALLOW = 0, /**< Package installation disable */
     PACKAGE_INSTALLATION_MODE_ALLOW  = 1    /**< Pasckage installation enable */
 } dpm_application_installation_mode_e;

 /**
  * @brief       Enumeration for uninstallation mode
  * @since_tizen 3.0
  */
 typedef enum {
     PACKAGE_UNINSTALLATION_MODE_DISALLOW = 0, /**< Package uninstallation disable */
     package_UNINSTALLATION_MODE_ALLOW  = 1    /**< Package uninstallation enable */
 } dpm_application_uninstallation_mode_e;

 /**
  * @brief       Sets the default mode for application installation.
  * @details     If the mode is set to APPLICATION_INSTALLATION_MODE_DISALLOW,
  *              no application can be installed on the device.
  * @since_tizen 3.0
  * @privlevel   public
  * @privilege   %http://tizen.org/privilege/dpm.application
  * @param[in]   handle Device Policy Client handle
  * @param[in]   mode Installation mode to be set, one of APPLICATION_INSTALLATION_MODE_ALLOW or
                 APPLICATION_INSTALLATION_MODE_DISALLOW
  * @return      #DPM_ERROR_NONE on success, otherwise a negative value
  * @retval      #DPM_ERROR_NONE Successful
  * @retval      #DPM_ERROR_TIMEOUT Timeout
  * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
  *              the privilege to call this API
  * @pre         handle must be created by dpm_create_client()
  * @post
  * @see         dpm_create_client()
  * @see         dpm_destroy_client()
  * @see         dpm_get_package_installation_mode()
  */
 DPM_API int dpm_set_package_installation_mode(dpm_client_h handle, int mode);

 /**
  * @brief       Sets the default mode for application uninstallation.
  * @details     If the mode is set to APPLICATION_UNINSTALLATION_MODE_DISALLOW,
  *              no application can be uninstalled from the device.
  * @since_tizen 3.0
  * @privlevel   public
  * @privilege   %http://tizen.org/privilege/dpm.application
  * @param[in]   handle Device Policy Client handle
  * @param[in]   mode Unstallation mode to be set, one of APPLICATION_UNINSTALLATION_MODE_ALLOW of
                 APPLICATION_UNINSTALLATION_MODE_DISALLOW
  * @return      #DPM_ERROR_NONE on success, otherwise a negative value
  * @retval      #DPM_ERROR_NONE Successful
  * @retval      #DPM_ERROR_TIMEOUT TImeout
  * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
  *              the privilege to call this API
  * @pre         handle must be created by dpm_create_client()
  * @post
  * @see         dpm_create_client()
  * @see         dpm_destroy_client()
  * @see         dpm_get_package_uninstallation_mode()
  */
 DPM_API int dpm_set_package_uninstallation_mode(dpm_client_h handle, int mode);

 /**
  * @brief       Gets the current installation mode for all packages.
  * @details     The default mode is to allow any package to be installed.
  * @since_tizen 3.0
  * @param[in]   handle Device Policy Client handle
  * @return      Current mode of operation, which is one of the following:
  *              DPM_APPLICATION_INSTALLATION_MODE_ALLOWED or
  *              DPM_APPLICATION_INSTALLATION_MODE_DISALLOWED
  * @retval      #DPM_APPLICATION_INSTALLATION_MODE_ALLOWED Application installation is allowed
  * @retval      #DPM_APPLICATION_INSTALLATION_MODE_DISALLOWED Application installation is not allowed
  * @pre         handle must be created by dpm_create_client()
  * @post
  * @see         dpm_create_client()
  * @see         dpm_destroy_client()
  * @see         dpm_set_package_installation_mode()
  */
 DPM_API int dpm_get_package_installation_mode(dpm_client_h handle);

 /**
  * @brief       Gets the current uninstallation mode for all packages.
  * @details     The default mode is to allow any package to be uninstalled.
  * @since_tizen 3.0
  * @param[in]   handle Device Policy Client handle
  * @return      Current mode of operation, which is one of the following:
  *              DPM_APPLICATION_UNINSTALLATION_MODE_ALLOWED or
  *              DPM_APPLICATION_UNINSTALLATION_MODE_DISALLOWED
  * @retval      #DPM_APPLICATION_UNINSTALLATION_MODE_ALLOWED Application uninstallation is allowed
  * @retval      #DPM_APPLICATION_UNINSTALLATION_MODE_DISALLOWED Application uninstallation is not allowed
  * @pre         handle must be created by dpm_create_client()
  * @post
  * @see         dpm_create_client()
  * @see         dpm_destroy_client()
  * @see          dpm_get_package_uninstallation_mode()
  */
 DPM_API int dpm_get_package_uninstallation_mode(dpm_client_h handle);

/**
 * @brief       Enables/disables an package without installation/uninstallation.
 * @details     Administrator can silently enable/disable any package without
 *              user interaction.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.application
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_get_application_state()
 */
DPM_API int dpm_set_application_state(dpm_client_h handle, const char* pkgid, dpm_application_state_e state);

/**
 * @brief       Checks whether a geiven application package is enabled or disabled
 * @details     Administrator can use this API to silently disable installation of an application
 *              without user interaction. The user can not install applications for which
 *              the administrator has disabled installation.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name of the application whose installation is to be disabled
 * @return      Current state of the application package
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_set_application_state()
 */
DPM_API int dpm_get_application_state(dpm_client_h handle, const char* pkgid);

/**
 * @brief       Adds package to blacklist
 * @details     Administrator can use this API to disallow package installation
 *              corresponding to the given pkgid.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name to be blacklisted
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_package_from_blacklist()
 * @see         dpm_check_package_is_blacklisted()
 */
DPM_API int dpm_add_package_to_blacklist(dpm_client_h handle, const char* pkgid);

/**
 * @brief       Removes package from blacklist
 * @details     Administrator can use this API to remove package from blacklist.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name of the application whose installation is to be disabled
 * @return      Current state of the application package
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_add_package_to_blacklist()
 * @see         dpm_check_package_is_blacklisted()
 */
DPM_API int dpm_remove_package_from_blacklist(dpm_client_h handle, const char* pkgid);

/**
 * @brief       Checks whether a package is added to blacklist
 * @details     Administrator can use this API to check whether the package is blacklisted.
 *              Once package is added to blacklist, it is prhibited to install on the device.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name of the application whose installation is to be disabled
 * @return      Current state of the application package
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_add_package_to_blacklist()
 * @see         dpm_remove_package_from_blacklist()
 */
DPM_API int dpm_check_package_is_blacklisted(dpm_client_h handle, const char* pkgid);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_APPLICATION_POLICY_H__

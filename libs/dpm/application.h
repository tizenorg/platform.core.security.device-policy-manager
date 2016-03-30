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
     APPLICATION_INSTALLATION_MODE_DISALLOW = 0, /**< Application installation disable */
     APPLICATION_INSTALLATION_MODE_ALLOW  = 1  /**< Application installation enable */
 } dpm_application_installation_mode_e;

 /**
  * @brief       Enumeration for uninstallation mode
  * @since_tizen 3.0
  */
 typedef enum {
     APPLICATION_UNINSTALLATION_MODE_DISALLOW = 0, /**< Application uninstallation disable */
     APPLICATION_UNINSTALLATION_MODE_ALLOW  = 1  /**< Application uninstallation enable */
 } dpm_application_uninstallation_mode_e;

 /**
  * @brief       API to set the default mode for application installation.
  * @details     If the mode is set to APPLICATION_INSTALLATION_MODE_DISALLOW,
  *              no application can be installed on the device.
  *              In DISALLOW MODE, an application can enable the installation of
  *              individual applications using dpm_set_application_installation_enabled()
  *              If the mode is set to APPLICATION_INSTALLATION_MODE_ALLOW, all
  *              applications can be installed to the device.
  *              In ALLOW MODE, an administrator can disable installation of individual
  *              applications using dpm_set_application_installation_disabled()
  * @since_tizen 3.0
  * @privlevel   public
  * @privilege   %http://tizen.org/privilege/dpm.application
  * @param[in]   handle Device Policy Client handle
  * @param[in]   mode Installation mode to be set, one of APPLICATION_INSTALLATION_MODE_ALLOW or
                 APPLICATION_INSTALLATION_MODE_DISALLOW
  * @return      #DPM_ERROR_NONE on success, otherwise a negative value
  * @retval      #DPM_ERROR_NONE Successful
  * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
  * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
  *              the privilege to call this API
  * @pre         handle must be created by dpm_create_client()
  * @post
  * @see         dpm_create_client()
  * @see         dpm_destroy_client()
  * @see         dpm_get_application_installation_mode()
  * @see         dpm_set_application_installation_enabled()
  * @see         dpm_set_application_installation_disabled()
  */
 DPM_API int dpm_set_application_installation_mode(dpm_client_h handle, int mode);

 /**
  * @brief       API to set the default mode for application uninstallation.
  * @details     If the mode is set to APPLICATION_UNINSTALLATION_MODE_DISALLOW,
  *              no application can be uninstalled from the device.
  *              In DISALLOW MODE, an administrator can enable the uninstallation of
  *              individual applications using set_application_uninstallation_enabled()
  *              If the mode is set to APPLICATION_UNINSTALLATION_MODE_ALLOW,
  *              all applications can be uninstalled from the device.
  *              In ALLOW MODE, an administrator can disable uninstallation of
  *              individual applications using dpm_set_application_uninstallation_disabled()
  * @since_tizen 3.0
  * @privlevel   public
  * @privilege   %http://tizen.org/privilege/dpm.application
  * @param[in]   handle Device Policy Client handle
  * @param[in]   mode Unstallation mode to be set, one of APPLICATION_UNINSTALLATION_MODE_ALLOW of
                 APPLICATION_UNINSTALLATION_MODE_DISALLOW
  * @return      #DPM_ERROR_NONE on success, otherwise a negative value
  * @retval      #DPM_ERROR_NONE Successful
  * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
  * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
  *              the privilege to call this API
  * @pre         handle must be created by dpm_create_client()
  * @post
  * @see         dpm_create_client()
  * @see         dpm_destroy_client()
  */
 DPM_API int dpm_set_application_uninstallation_mode(dpm_client_h handle, int mode);

 /**
  * @brief       API to get the current installation mode for all applications.
  * @details     The default mode is to allow any application package to be
  *              installed.
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
  */
 DPM_API int dpm_get_application_installation_mode(dpm_client_h handle);

 /**
  * @brief       API to get the current uninstallation mode for all applications.
  * @details     The default mode is to allow any application package to be
  *              uninstalled.
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
  */
 DPM_API int dpm_get_application_uninstallation_mode(dpm_client_h handle);

/**
 * @brief       API to check whether a particular application package can be installed
 * @details     Administrator can use this API to check whether a particular package
 *              can be installed. If the return value is FALSE, the user cannot
 *              install the application package.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name of the application whose installation state
 *              is to be checked
 * @return      TRUE if installation is allowed, else FALSE
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_application_running()
 */
DPM_API int dpm_get_application_installation_enabled(dpm_client_h handle, const char* pkgid);

/**
 * @brief       API to check whether a particular application package can be uninstalled
 * @details     Administrator can use this API to check whether a particular package
 *              can be uninstalled. If the return value is FALSE, the user cannot
 *              uninstall the application package.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name of the application whose installation state
 *              is to be checked
 * @return      TRUE if uninstallation is allowed, else FALSE
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_application_running()
 */
DPM_API int dpm_get_application_uninstallation_enabled(dpm_client_h handle, const char* pkgid);

/**
 * @brief       API to disable uninstallation of an application package.
 * @details     Administrator can use this API to silently disable installation of an application
 *              without user interaction. The user can not install applications for which
 *              the administrator has disabled installation.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.application
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name of the application whose installation is to be disabled
 * @return      TRUE if installation is allowed, else FALSE
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_application_running()
 */
DPM_API int dpm_set_application_uninstallation_disabled(dpm_client_h handle, const char* pkgid);

/**
 * @brief       API to enable / disable an application package without installation
 *              / uninstallation.
 * @details     Administrator can silently enable/disable any application without
 *              user interaction. It can be used to disable malware, unauthorized
 *              applications when user leaves the enterprise.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.application
 * @param[in]   handle Device Policy Client handle
 * @param[in]   pkgid The package name
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_ACCESS_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_set_application_state()
 */
DPM_API int dpm_set_application_state(dpm_client_h handle, const char* pkgid, dpm_application_state_e state);

/**
 * @brief       API to check whether a geiven application package is enabled or
 *              disabled
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
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_APPLICATION_POLICY_H__

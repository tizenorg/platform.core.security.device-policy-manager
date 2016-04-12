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

#ifndef __CAPI_WIFI_POLICY_H__
#define __CAPI_WIFI_POLICY_H__

#include <dpm/device-policy-client.h>

/**
 * @file wifi.h
 * @brief This file provides APIs to control wifi policy
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_WIFI_POLICY_MODULE
 * @{
 */

/**
 * @brief       Allows or disallows the user to change the Wi-Fi state.
 * @details     An administrator can use this API to allow or disallow the user to
 *              change the Wi-Fi state. If it is restricted, the user does not have UI
 *              access to change the state.
 *              Also, the apps that uses wifi_activate() or wifi_deactivate()
 *              follow this restriction.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.wifi
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable TRUE to enable wifi state change restriction, else FALSE
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dom_is_wifi_state_change_allowed()
 * @see         wifi_activate()
 * @see         wifi_deactivate()
 */
DPM_API int dpm_set_wifi_state_change_restriction(dpm_client_h handle, int enable);

/**
 * @brief       Checks whether the user is restricted to change the Wi-Fi state.
 * @details     An administrator can use this API to check whether or not the user is
 *              restricted to modify Wi-Fi settings. The user is restricted in modifying
 *              Wi-Fi settings if at least one administrator has set the value to FALSE.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @return      TRUE if the user is not allowed to change the Wi-Fi state, else FALSE
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_set_wifi_state_change_restriction()
 */
DPM_API int dpm_is_wifi_state_change_restricted(dpm_client_h handle);

/**
 * @brief       Allows or disallows user to modify some Wi-Fi settings of network settings.
 * @details     An administrator can use this API to allow or disallow users to modify selected
 *              Wi-Fi settings like static ip configuration, proxy settings, security type
 *              and others. When this policy is in effect the user is only allowed to
 *              modify only the username, password, anonymous identity, and wep key.
 *              In addition, the user cannot remove the networ. When false, the user can
 *              modify all Wi-fi network settings normally and also remove it.
 *              Also the apps that uses APIs to change some Wi-Fi settings follow
 *              this restriction such as wifi_ap_set_ip_address().
 *              Please refer to "see also" if you want to know the rest of APIs
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.wifi
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable TRUE to enable wifi setting changes, else FALSE
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dom_is_wifi_setting_changes_restricted()
 * @see         wifi_ap_set_ip_config_type()
 * @see         wifi_ap_set_ip_address()
 * @see         wifi_ap_set_subnet_mask()
 * @see         wifi_ap_set_gateway_address()
 * @see         wifi_ap_set_proxy_address()
 * @see         wifi_ap_set_proxy_tyype()
 * @see         wifi_ap_set_dns_address()
 * @see         wifi_ap_set_security_type()
 * @see         wifi_ap_set_encryption_type()
 * @see         wifi_ap_set_passphrase()
 */
DPM_API int dpm_set_wifi_setting_changes_restriction(dpm_client_h handle, int enable);

/**
 * @brief       Checks if the user is allowed to modify certain Wi-Fi network settings.
 * @details     An administrator can use this API to check whether or not the user is
 *              allowed to modify Wi-Fi settings. The user is restricted in modifying
 *              Wi-Fi settings if at least one administrator has set the value to TRUE.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @return      TRUE if one or more administrators enabled restriction
 *              FALSE if user can change all Wi-Fi network settings
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_set_wifi_setting_changes_restriction()
 */
DPM_API int dpm_is_wifi_setting_changes_restricted(dpm_client_h handle);

/**
 * @brief       Allows or disallows the user to change Wi-Fi hotspot settings
 * @details     An administrator can use this API to restrict changing Wi-Fi
 *              hotspot settings. When restricted, the UI is grayed out so the user cannot
 *              modify the settings.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.security
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable TRUE to restrict wifi hostspot setting, else FALSE
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_wifi_hostspot_restricted()
 */
DPM_API int dpm_set_wifi_hotspot_restriction(dpm_client_h handle, int enable);

/**
 * @brief       Checks whether the the Wi-Fi hotspot is restricted.
 * @details     An administrator can use this API to check whether the Wi-Fi hotspot
 *              is restricted.
 *              If the Wi-Fi hotspot is restricted, the UI is grayed out so user can not
 *              change its state.
 * @since_tizen 3.0
 * @privlevel   public
 * @param[in]   handle Device Policy Client handle
 * @return      TRUE if modification is allowed, false if modification is denied
 * @pre         handle must be created by dpm_create_client()
 * @post
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_set_wifi_hotspot_restriction()
 */
DPM_API int dpm_is_wifi_hotspot_restricted(dpm_client_h handle);

/**
 * @brief       Restricts network accessed based on the Wi-Fi network service set
 *              identifier(SSID).
 * @details     An administrator can use this API to restrict connecting to the Wi-Fi
 *              network based on the blocked network list.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.wifi
 * @param[in]   handle Device Policy Client handle
 * @param[in]   enable TRUE to enable the Wi-Fi network access restriction,
 *              FALSE to disable the Wi-Fi network access restriction
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @pre         SSID blacklist must be added by dpm_add_wifi_ssid_to_blocklist()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_is_wifi_network_access_restricted()
 * @see         dpm_add_wifi_ssid_to_blocklist()
 * @see         dpm_remove_wifi_ssid_from_blocklist()
 */
DPM_API int dpm_set_wifi_network_access_restriction(dpm_client_h handle, int activate);

/**
 * @brief       Checks whether the SSID-based Wi-Fi network access restriction is
 *              enabled.
 * @details     An administrator can use this API to check whether the SSID-based
 *              Wi-Fi network restriction is enabled.
 * @since_tizen 3.0
 * @param[in]   handle Device Policy Client handle
 * @return      TRUE if restriction is activated or FALSE if restriction is deactivated
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_set_wifi_network_access_restriction()
 */
DPM_API int dpm_is_wifi_network_access_restricted(dpm_client_h handle);

/**
 * @brief       Adds a service set identifier(SSID) to the list of blocked network.
 * @details     An administrator can use this API to add an SSID to the list of blocked
 *              networks, which prevents the user from connecting to it.
 *              The blocked network still appears in the Access Point list but is disabled.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.wifi
 * @param[in]   handle Device Policy Client handle
 * @param[in]   ssid The SSID to block
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @retval      #DPM_ERROR_OUT_OF_MEMORY Too many SSIDs in blocked network list
 * @pre         handle must be created by dpm_create_client()
 * @post        dpm_set_wifi_network_access_restriction() must be called
 *              when SSIDs in the blacklist are needed to get restricted
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_remove_wifi_ssid_to_blocklist()
 * @see         dpm_enable_wifi_network_access_restriction()
 */
DPM_API int dpm_add_wifi_ssid_to_blocklist(dpm_client_h handle, const char* ssid);

/**
 * @brief       Removes a service set identifier(SSID) from the list of blocked
 *              networks.
 * @details     An administrator can use this API to remove an SSID from the list of
 *              blocked networks, which allows the user to connect to it.
 * @since_tizen 3.0
 * @privlevel   public
 * @privilege   %http://tizen.org/privilege/dpm.wifi
 * @param[in]   handle Device Policy Client handle
 * @param[in]   ssid The SSID to be removed from the list of blocked networks
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Timeout
 * @retval      #DPM_ERROR_NOT_SUPPORTED Not supported
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         handle must be created by dpm_create_client()
 * @see         dpm_create_client()
 * @see         dpm_destroy_client()
 * @see         dpm_add_wifi_ssid_to_blocklist()()
 * @see         dpm_set_wifi_network_access_restriction()
 */
DPM_API int dpm_remove_wifi_ssid_from_blocklist(dpm_client_h handle, const char* ssid);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_SECURITY_POLICY_H__

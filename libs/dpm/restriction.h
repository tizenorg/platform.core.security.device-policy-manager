/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#ifndef __CAPI_RESTRICTION_POLICY_H__
#define __CAPI_RESTRICTION_POLICY_H__

#include <dpm/device-policy-manager.h>

/**
 * @file restriction.h
 * @brief This file provides APIs to control restriction functionality.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_DPM_RESTRICTION_POLICY_MODULE
 * @{
 */

/**
 * @partner
 * @brief       Allows or disallows the camera.
 * @details     An administrator can use this API to set whether the camera
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.camera
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, disallow the camera, if false, allow the camera
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_camera_state()
 */
int dpm_restriction_set_camera_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Gets the allow status of the camera.
 * @details     An administrator can use this API to check the state of
 *              the camera.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The camera allow status, true if the access is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_camera_state()
 */
int dpm_restriction_get_camera_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the microphone.
 * @details     An administrator can use this API to set whether the microphone
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.microphone
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, disallow the microphone, if false, allow the microphone
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_microphone_state()
 */
int dpm_restriction_set_microphone_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Gets the allow status of the microphone.
 * @details     An administrator can use this API to check the state of
 *              the microphone.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The microphone allow status, true if the access is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_microphone_state()
 */
int dpm_restriction_get_microphone_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to change the location state.
 * @details     An administrator can use this API to allow or disallow to change
 *              the location state.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.location
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, allow to change the location state, if false, disallow
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_location_state()
 */
int dpm_restriction_set_location_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       API to get the allow status of the location state change.
 * @details     An administrator can use this API to get the allow status of
 *              the location state change.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]   is_restricted The location state change restriction status, true if the change is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_location_state()
 */
int dpm_restriction_get_location_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to use the usb mass storage.
 * @details     An administrator can use this API to set whether the usb mass
 *              storage is allowed or not.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.storage
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, disallow the usb mass storage, if false, allow
 *              the usb mas storage
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_storage_get_external_storage_state()
 */
int dpm_restriction_set_external_storage_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the usb mass storage restriction is enable or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the usb mass storage.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The external storage restriction status, true if the access is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_storage_set_external_storage_state()
 */
int dpm_restriction_get_external_storage_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to use the clipboard.
 * @details     An administrator can use this API to set whether the clipboard.
 *              is allowed or not
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.clipboard
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, disallow the clipboard, if false, allow the clipboard
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_clipboard_state()
 */
int dpm_restriction_set_clipboard_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the the use of clipboard is allowed or not.
 * @details     An administrator can use this API to check the use of clipboard is allowed.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The clipboard restriction status, true if the access is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_clipboard_state()
 */
int dpm_restriction_get_clipboard_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to use the usb debugging.
 * @details     An administrator can use this API to set whether the usb debugging
 *              is allowed or not.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.debugging
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, disallow the usb debugging, if false, allow the usb
 *              debugging
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_usb_debugging_state()
 */
int dpm_restriction_set_usb_debugging_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the usb debugging is allowed or not.
 * @details     An administrator can use this API to check the restriction status of
 *              the usb debugging.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]   is_restricted The usb debugging restriction status, true if the usb debugging is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_usb_debugging_state()
 */
int dpm_restriction_get_usb_debugging_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to change the Wi-Fi state.
 * @details     An administrator can use this API to allow or disallow the user to
 *              change the Wi-Fi state. If it is restricted, the user does not have UI
 *              access to change the state.
 *              Also, the apps that uses wifi_activate() or wifi_deactivate()
 *              follow this restriction.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.wifi
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, enable Wi-Fi state change restriction, if false, allow Wi-Fi state change.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_wifi_state()
 */
int dpm_restriction_set_wifi_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the user is restricted to change the Wi-Fi state.
 * @details     An administrator can use this API to check whether or not the user is
 *              restricted to modify Wi-Fi settings. The user is restricted in modifying
 *              Wi-Fi settings if at least one administrator has set the value to FALSE.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The Wi-Fi state change restriction status, true if the change is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_wifi_state()
 */
int dpm_restriction_get_wifi_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to change Wi-Fi hotspot settings.
 * @details     An administrator can use this API to restrict changing Wi-Fi
 *              hotspot settings. When restricted, the UI is grayed out so the user cannot
 *              modify the settings.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.wifi
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, restrict Wi-Fi hostspot settings change, if false, allow Wi-Fi hotspot settings change.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_wifi_hotspot_state()
 */
int dpm_restriction_set_wifi_hotspot_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the the Wi-Fi hotspot settings change is restricted.
 * @details     An administrator can use this API to check whether the Wi-Fi hotspot settings change
 *              is restricted.
 *              If the Wi-Fi hotspot settings change is restricted, the UI is grayed out so user can not
 *              change its state.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The Wi-Fi hotspot settings change restriction status, true if the change is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_wifi_hotspot_state()
 */
int dpm_restriction_get_wifi_hotspot_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to change the bluetooth tethering settings.
 * @details     An administrator can use this API to restrict changing the bluetooth tethering settings.
 *              When restricted, the UI is grayed out so the user cannot modify the settings.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, restrict the bluetooth tethering settings change, if false, allow the bluetooth tethering settings change.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_bluetooth_tethering_state()
 */
int dpm_restriction_set_bluetooth_tethering_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the the bluetooth tethering settings change is restricted.
 * @details     An administrator can use this API to check whether the bluetooth tethering settings change
 *              is restricted.
 *              If the bluetooth tethering settings change is restricted, the UI is grayed out so user can not
 *              change its state.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The bluetooth tethering settings change restriction status, true if the change is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_bluetooth_tethering_state()
 */
int dpm_restriction_get_bluetooth_tethering_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to change USB tethering settings.
 * @details     An administrator can use this API to restrict changing USB tethering settings.
 *              When restricted, the UI is grayed out so the user cannot modify the settings.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.usb
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, restrict USB tethering settings chnage, if false, allow USB tethering settings change.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_usb_tethering_state()
 */
int dpm_restriction_set_usb_tethering_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the the USB tethering settings change is restricted.
 * @details     An administrator can use this API to check whether the USB tethering settings change
 *              is restricted.
 *              If the USB tethering settings change is restricted, the UI is grayed out so user can not
 *              change its state.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The USB tethering settings change restriction status, true if the change is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_usb_tethering_state()
 */
int dpm_restriction_get_usb_tethering_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Allows or disallows the user to change the bluetooth settings.
 * @details     An administrator can use this API to restrict changing the bluetooth settings.
 *              When restricted, the UI is grayed out so the user cannot modify the settings.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, restrict the bluetooth settings chnage, if false, allow the bluetooth settings change.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_bluetooth_mode_change_state()
 */
int dpm_restriction_set_bluetooth_mode_change_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the the bluetooth settings change is restricted.
 * @details     An administrator can use this API to check whether the bluetooth settings change is restricted.
 *              If the bluetooth setting change is restricted, the UI is grayed out so user can not change its state.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is restricted The bluetooth settings change restriction status, true if the change is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_bluetooth_mode_change_state()
 */
int dpm_restriction_get_bluetooth_mode_change_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Checks whether the the bluetooth desktop connectivity is restricted.
 * @details     An administrator can use this API to check whether the bluetooth desktop connectivity
 *              is restricted.
 *              If the bluetooth desktop connectivity is restricted, the UI is grayed out so user can not
 *              change its state.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device policy manager handle
 * @param[out]  restrict If true, restrict the bluetooth desktop connectivity, if false, allow the bluetooth desktop connectivitiy
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_bluetooth_desktop_connectivity_state()
 */
int dpm_restriction_set_bluetooth_desktop_connectivity_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the the Bluetooth desktop connectivity is restricted.
 * @details     An administrator can use this API to check whether the Bluetooth desktop connectivity is restricted.
 *              If the Bluetooth desktop connectivity is restricted, the UI is grayed out so user can not change its state.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The bluetooth desktop connectivity restriction status, true if the connectivity is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_bluetooth_desktop_connectivity_state()
 */
int dpm_restriction_get_bluetooth_desktop_connectivity_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Checks whether the the bluetooth pairing is restricted.
 * @details     An administrator can use this API to check whether the bluetooth pairing is restricted.
 *              If the bluetooth pairing is restricted, the UI is grayed out so user can not change its state.
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.bluetooth
 * @param[in]   handle Device policy manager handle
 * @param[out]  restrict If true, restrict the bluetooth pairing, if false, allow the bluetooth pairing.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_get_bluetooth_pairing_state()
 */
int dpm_restriction_set_bluetooth_pairing_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the the bluetooth pairing is restricted.
 * @details     An administrator can use this API to check whether the bluetooth pairing is restricted.
 *              If the bluetooth pairing is restricted, the UI is grayed out so user can not change its state.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The bluetooth pairing restriction status, true if the pairing is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_manager_create()
 * @see         dpm_restriction_set_bluetooth_pairing_state()
 */
int dpm_restriction_get_bluetooth_pairing_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Specifies whether the devce allows the use of SMS or text messaging.
 * @details     An administrator can disable the text messaging capability without any user interaction
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.message
 * @param[in]   handle Device policy manager handle
 * @param[in]   restict If true, restrict the use of SMS or text messaging, if false, allow the use of SMS or text messaging.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_restriction_get_messaging_state()
 */
int dpm_restriction_set_messaging_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the text messaging capability is enabled or not.
 * @details     An administrator can check whether text messaging capability is enabled or not.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The text messaging restriction status, true if the messaging is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_restriction_set_messaging_state()
 */
int dpm_restriction_get_messaging_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Specifies whether the device allows access to POP or IMAP e-mail.
 * @details     An administrator can disable the email capability without any user interaction
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.email
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If ture, restrict POP or IMAP e-mail, if false, allow POP or IMAP e-mail.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_restriction_get_popimap_email_state()
 */
int dpm_restriction_set_popimap_email_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the device allows access to POP or IMAP e-mail.
 * @details     An administrator can check the email capability without any user interaction
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The POP or IMAP e-mail restriction status, true if the e-mail is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_restriction_set_popimap_email_state()
 */
int dpm_restriction_get_popimap_email_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @partner
 * @brief       Specifies whether the device allows the use of web browser.
 * @details     An administrator can disable the browser capability without any user interaction
 * @since_tizen 3.0
 * @privlevel   partner
 * @privilege   %http://tizen.org/privilege/dpm.browser
 * @param[in]   handle Device policy manager handle
 * @param[in]   restrict If true, disallow the use of web browser, if false, allow the use of web browser.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_restriction_get_browser_state()
 */
int dpm_restriction_set_browser_state(device_policy_manager_h handle, bool restrict);

/**
 * @brief       Checks whether the web browser capability is enabled or not.
 * @details     An administrator can check whether web browser capability is enabled or not.
 * @since_tizen 3.0
 * @param[in]   handle Device policy manager handle
 * @param[out]  is_restricted The web browser capability restriction status, true if the browser is restricted, false otherwise.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_TIMEOUT Time out
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by dpm_manager_create().
 * @see         dpm_restriction_set_browser_state()
 */
int dpm_restriction_get_browser_state(device_policy_manager_h handle, bool *is_restricted);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPI_RESTRICTION_POLICY_H__ */

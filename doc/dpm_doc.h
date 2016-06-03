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

#ifndef __DPM_DOC_H__
#define __DPM_DOC_H__


 /**
  * @ingroup CAPI_SECURITY_FRAMEWORK
  * @defgroup CAPI_SECURITY_DPM_MODULE Device Policy Manager
  * @brief The Device Policy Management framework provides APIs to create
  * security-aware applications that are useful in enterprise settings.
  * <br>
  * @section CAPI_SECURITY_DPM_MODULE_OVERVIEW Overview
  * The primary purpose of the DPM(Device Policy Management) framework is
  * to support enterprise applications, which require integration with
  * policies enforced on the device to restrict their operations on
  * some circumstance, such as Exchange ActiveSync or corporate apps.<br>
  * The device policy API supports the policies listed in the below table.
  * <table>
  *  <tr>
  *   <td>Policy</td>
  *   <td>Description</td>
  *  </tr>
  *  <tr>
  *   <td>Minimum password length</td>
  *   <td>Specifies the required number of characters for the password.</td>
  *  </tr>
  *  <tr>
  *   <td>Alphanumeric password required</td>
  *   <td>Requires that passwords have a combination of letters and numbers.
  *       They may include symbolic characters.</td>
  *  </tr>
  *  <tr>
  *   <td>Complex password required</td>
  *   <td>Requires that passwords must contain at least a letter, a numerical
  *       digit, and a special symbol.</td>
  *  </tr>
  *  <tr>
  *   <td>PasswordPolicy expiration timeout</td>
  *   <td>When the password will expire, expressed as a delta in milliseconds
  *       from when a device admin sets the expiration timeout.</td>
  *  </tr>
  *  <tr>
  *   <td>Maximum failed password attempts</td>
  *   <td>Specifies how many times a user can enter the wrong password before
  *       the device wipes its data.</td>
  *  </tr>
  *  <tr>
  *   <td>Maximum inactivity time lock</td>
  *   <td>Specifies the maximum number of seconds of inactivity time before
  *       the screen timeout occurs</td>
  *  </tr>
  * </table>
  * <br>
  * @subsection Privilege
  * In order to use privileged APIs, the client must be registered to the device policy
  * manager as the device policy client by enrollment process. Other unregistered
  * application are only allowed to subscribe policy change notification published by
  * the device policy manager.
  *<br>
  * @ingroup CAPI_SECURITY_DPM_MODULE
  * @defgroup CAPI_DPM_MANAGER_MODULE Policy Manager Interface
  * @brief The policy client provides functions required to access the device policy APIs.
  * @section CAPI_DPM_MANAGER_MODULE_HEADER Required Header
  *  \#include <dpm/device-policy-manager.h>
  * @section CAPI_DPM_MANAGER_MODULE_OVERVIEW Overview
  * The Policy Manager module provides Device Policy Manager handle which keeps track of
  * state between the device policy client and the device policy manager.
  * The device policy clients must create the handle by using dpm_manager_create()
  * before attempting to use almost any of the device policy APIs, and the handle should be
  * freed by using dpm_manager_destroy() when interaction with the device policy manager
  * is no longer required.
  *
  * @ingroup CAPI_SECURITY_DPM_MODULE
  * @defgroup CAPI_DPM_PASSWORD_POLICY_MODULE PasswordPolicy policy group
  * @brief PasswordPolicy policy group provides APIs to control password policies.
  * @section CAPI_DPM_PASSWORD_POLICY_MODULE_HEADER Required Header
  *  \#include <dpm/password.h>
  * @section CAPI_DPM_PASSWORD_POLICY_MODULE_OVERVIEW Overview
  * PasswordPolicy policy group provides APIs to control password policies such as
  * password type, password quality and expiration date. Any API which requires privileges
  * is only available for the registered device policy client which is enrolled to the device
  * by enrollment process.
  *
  * @ingroup CAPI_SECURITY_DPM_MODULE
  * @defgroup CAPI_DPM_RESTRICTION_POLICY_MODULE RestrictionPolicy policy group
  * @brief RestrictionPolicy policy group provides APIs to control restriction functionality.
  * @section CAPI_DPM_RESTRICTION_POLICY_MODULE_HEADER Required Header
  *  \#include <dpm/restriction.h>
  * @section CAPI_DPM_RESTRICTION_POLICY_MODULE_OVERVIEW Overview
  * RestrictionPolicy policy group provides APIs to control  restriction functionality such as
  * Camera, Microphone, Wifi, bluetooth, USB and so on. Any API which requires privileges
  * is only available for the registered device policy client which is enrolled to the device
  * by enrollment process.
  *
  * @ingroup CAPI_SECURITY_DPM_MODULE
  * @defgroup CAPI_DPM_SECURITY_POLICY_MODULE SecurityPolicy policy group
  * @brief SecurityPolicy policy group provides APIs to control security functionality.
  * @section CAPI_DPM_SECURITY_POLICY_MODULE_HEADER Required Header
  *  \#include <dpm/security.h>
  * @section CAPI_DPM_SECURITY_POLICY_MODULE_OVERVIEW Overview
  * SecurityPolicy policy group provides APIs to control security functionality such as
  * device encryption and screen lock. Any API which requires privileges
  * is only available for the registered device policy client which is enrolled to the device
  * by enrollment process.
  *
  * @ingroup CAPI_SECURITY_DPM_MODULE
  * @defgroup CAPI_DPM_ZONE_POLICY_MODULE ZonePolicy policy group
  * @brief ZonePolicy policy group provides APIs to control containers
  * @section CAPI_DPM_ZONE_POLICY_MODULE_HEADER Required Header
  * \#include <dpm/zone.h>
  * @section CAPI_DPM_ZONE_POLICY_MODULE_OVERVIEW Overview
  * This module provides APIs to manage application containers. Any API which requires
  * privileges is only available for the registered device policy client which is enrolled to
  * the device by enrollment process.
  *
  */

#endif /* __DPM_DOC_H__ */

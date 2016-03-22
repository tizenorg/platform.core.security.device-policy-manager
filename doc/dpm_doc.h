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
  * @defgroup CAPI_DPM_FRAMEWORK Device Policy Manager
  * @brief The Device Policy Management framework provides APIs to create
  * security-aware applications that are useful in enterprise settings.
  * <br>
  * @section CAPI_DPM_FRAMEWORK_OVERVIEW Overview
  * The primary purpose of the DPM(Device Policy Management) framework is
  * to support enterprise applications, which require integration with
  * policies enforced on the device to restrict their operations on
  * some circumstance, such as exchange or corporate apps.<br>
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
  *   <td>Password expiration timeout</td>
  *   <td>When the password will expire, expressed as a delta in milliseconds
  *       from when a device admin sets the expiration timeout.</td>
  *  </tr>
  *  <tr>
  *   <td>Maximum failed password attempts</td>
  *   <td>Specifies how many times a user can enter the wrong password before
  *       the device wipes its data.The Device Policy API also allows to
  *       remotely reset the device to factory defaults.</td>
  *  </tr>
  *  <tr>
  *   <td>Maximum inactivity time lock</td>
  *   <td>Specifies the maximum number of secoinds of inactivity time before
  *       the screen timeout occurs</td>
  *  </tr>
  *  <tr>
  *   <td>Require storage encryption</td>
  *   <td>Specifies that the storage should be encrypted, if the device
  *       supports it.</td>
  *  </tr>
  * </table>
  *
  * <br>
  * @section CAPI_DPM_FRAMEWORK_ARCHITECTURE Architecture
  * The Device Policy Management framework consists of the device policy
  * manager and policy client library as shown in the figure.
  * The device policy manager manages all policies enforced on the device
  * and exposes APIs to administrator via the policy client library called
  * libdpm. Thus, the policy client should link the policy client library
  * to enforce policies.
  * <br>
  * @image html dpm.png "Tizen Device Policy Management Framework"
  * <br>
  * @section PolicyGroup Policy Groups
  * The Device Policy Management framework supposts following policy groups.
  * Each policy group has dedicated header file. So, you should include it
  * to use any API associated to the policy group.
  * <center>
  * <table>
  * <tr>
  *     <td><b>Policy Group</b></td>
  *     <td><center><b>Description</b></center></td>
  *     <td><b>Related header file</b></td>
  * </tr>
  * <tr>
  *     <td>Password</td>
  *     <td>Provide functionality to control password policy</td>
  *     <td>dpm/password.h</td>
  * </tr>
  * <tr>
  *     <td>Security</td>
  *     <td>Provide functionality to control security such as encryption
  *         and device wipe</td>
  *     <td>dpm/security.h</td>
  * </tr>
  * <tr>
  *     <td>Zone</td>
  *     <td>Provide functionality to control application containers</td>
  *     <td>dpm/zone.h</td>
  * </tr>
  * </table>
  * </center>
  *<br>
  *<br>
  *<br>
  * @section Priviliges
  * Only privileged client, which is registered to the policy manager can
  * enforce policies. Other unprivileged application are only allowed to
  * subscribe policy change notification published by the device policy manager.
  *<br>
  * @defgroup CAPI_DPM_POLICY_CLIENT_MODULE Policy Client Interface
  * @brief Client interface to access policy APIs.
  * @ingroup CAPI_DPM_FRAMEWORK
  * @section CAPI_DPM_POLICY_CLIENT_MODULE_HEADER Required Header
  *  \#include <dpm/device-policy-client.h>
  *
  * @defgroup CAPI_DPM_PASSWORD_POLICY_MODULE Password policy group
  * @brief Password policy group provides APIs to control password policies.
  * @ingroup CAPI_DPM_FRAMEWORK
  * @section CAPI_DPM_POLICY_CLIENT_MODULE_HEADER Required Header
  *  \#include <dpm/device-policy-client.h>
  *
  * @defgroup CAPI_DPM_SECURITY_POLICY_MODULE Security policy group
  * @brief Security policy group provides APIs to control security related
  * functionality on the device
  *  such as device encryption, and wipe.
  * @ingroup CAPI_DPM_FRAMEWORK
  * @section CAPI_DPM_SECURITY_POLICY_MODULE_HEADER Required Header
  * \#include <dpm/security.h>
  *
  * @defgroup CAPI_DPM_ZONE_POLICY_MODULE Zone policy group
  * @brief Zone policy group provides APIs to control containers
  * @ingroup CAPI_DPM_FRAMEWORK
  * @section CAPI_DPM_ZONE_POLICY_MODULE_HEADER Required Header
  * \#include <dpm/zone.h>
  *
  */

#endif /* __DPM_DOC_H__ */


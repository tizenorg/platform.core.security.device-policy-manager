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

#ifndef __CAPI_SECURITY_POLICY__
#define __CAPI_SECURITY_POLICY__

#include <dpm/device-policy-client.h>

#ifdef __cplusplus
extern "C" {
#endif

DPM_API int dpm_lockout_device(dpm_client_h handle);
DPM_API int dpm_lockout_screen(dpm_client_h handle);
DPM_API int dpm_wipe_data(dpm_client_h handle, const int id);
DPM_API int dpm_reboot(dpm_client_h handle);
DPM_API int dpm_poweroff_device(dpm_client_h handle);
DPM_API int dpm_set_internal_storage_encryption(dpm_client_h handle, const int encrypt);
DPM_API int dpm_is_internal_storage_encrypted(dpm_client_h handle);
DPM_API int dpm_set_external_storage_encryption(dpm_client_h handle, const int encrypt);
DPM_API int dpm_is_external_storage_encrypted(dpm_client_h handle);

enum {
    WIPE_EXTERNAL_MEMORY = (1 << 0),
    WIPE_INTERNAL_MEMORY = (1 << 1)
} dpm_storage_type;

#ifdef __cplusplus
}
#endif

#endif //! __CAPI_SECURITY_POLICY__

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

#ifndef __DEVICE_POLICY_CLIENT__
#define __DEVICE_POLICY_CLIENT__

#ifndef DPM_API
#define DPM_API __attribute__((visibility("default")))
#endif // API

#ifdef __cplusplus
extern "C" {
#endif

typedef void* dpm_client_h;

DPM_API dpm_client_h dpm_create_client(void);
DPM_API void dpm_destroy_client(dpm_client_h handle);

#ifdef __cplusplus
}
#endif

#endif //! __DEVICE_POLICY_CLIENT__

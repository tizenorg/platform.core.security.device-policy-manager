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

#ifndef __DEVICE_POLICY_CLIENT_H__
#define __DEVICE_POLICY_CLIENT_H__

#include <dpm/device-policy-manager.h>

#ifndef DPM_API
#define DPM_API __attribute__((visibility("default")))
#endif // API

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*dpm_policy_changed_cb)(const char* name, const char* state, void *user_data);

typedef void* dpm_context_h;

DPM_API dpm_context_h dpm_context_create(void);

DPM_API int dpm_context_destroy(dpm_context_h handle);

DPM_API int dpm_context_add_policy_changed_cb(dpm_context_h handle,
											  const char* name,
											  dpm_policy_changed_cb callback,
											  void* user_data,
											  int* id);

DPM_API int dpm_context_remove_policy_changed_cb(dpm_context_h handle,
												 int id);

typedef void(*dpm_signal_cb)(const char* name, const char* object, void *user_data);

DPM_API int dpm_context_add_signal_cb(dpm_context_h context, const char* signal,
									  dpm_signal_cb callback, void* user_data,
									  int* id);

DPM_API int dpm_context_remove_signal_cb(dpm_context_h context, int id);

#define DEFINE_POLICY_HANDLE(_n_)                                                          \
typedef void* dpm_##_n_##_policy_h;                                                      \
static inline dpm_##_n_##_policy_h dpm_context_acquire_##_n_##_policy(dpm_context_h handle)       \
{                                                                                          \
	return handle;                                                                         \
}                                                                                          \
																						   \
static inline int dpm_context_release_##_n_##_policy(dpm_context_h context, dpm_##_n_##_policy_h handle) \
{                                                                                          \
	return DPM_ERROR_NONE;                                                                 \
}

DEFINE_POLICY_HANDLE(password)
DEFINE_POLICY_HANDLE(application)
DEFINE_POLICY_HANDLE(wifi)
DEFINE_POLICY_HANDLE(bluetooth)
DEFINE_POLICY_HANDLE(security)
DEFINE_POLICY_HANDLE(restriction)
DEFINE_POLICY_HANDLE(zone)

#ifdef __cplusplus
}
#endif

#endif /* __DEVICE_POLICY_CLIENT_H__ */

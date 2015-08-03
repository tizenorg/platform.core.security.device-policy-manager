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

#ifndef API
#define API __attribute__((visibility("default")))
#endif // API

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dpm_admin * dpm_admin_t;


int dpm_create_admin(const char *name, const char *password, dpm_admin_t *admin);

int dpm_get_admin(const char *name, dpm_admin_t admin);

int dpm_destroy_admin(dpm_admin_t admin);

int dpm_connect_service(void);

#ifdef __cplusplus
}
#endif

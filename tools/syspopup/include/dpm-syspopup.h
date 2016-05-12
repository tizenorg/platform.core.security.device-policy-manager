/*
 * Tizen DPM Syspopup
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __DPM_SYSPOPUP_H__
#define __DPM_SYSPOPUP_H__

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <app.h>
#include <app_common.h>
#include <bundle.h>
#include <system_settings.h>
#include <dlog.h>
#include <Elementary.h>
#include <efl_extension.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "org.tizen.dpm-syspopup"

#define DPM_SYSPOPUP_DEFAULT_STYLE "toast"

typedef struct {
	const char *id;
	const char *text;
} popup_info_s;

void _create_syspopup(const char *style, const char *id);
popup_info_s *_get_dpm_popup_info(const char *id);

#endif /* __DPM_SYSPOPUP_H__ */

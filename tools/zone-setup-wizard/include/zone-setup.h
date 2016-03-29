/*
 * Tizen Zone Setup-Wizard application
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
#ifndef __ZONE_SETUP_H__
#define __ZONE_SETUP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <app.h>
#include <app_common.h>
#include <bundle.h>
#include <dlog.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <dpm/zone.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "zone-setup-wizard"

#if !defined(PACKAGE)
#define PACKAGE "org.tizen.zone-setup-wizard"
#endif

#define MANIFEST_PATH "/usr/apps/org.tizen.zone-setup-wizard/data/bundle-manifest.xml" /* [TBD] : removed */
#define SETUP_TEXT_STYLE "DEFAULT='font=Tizen:style=Regular font_size=34 color=#ffffff wrap=mixed align=center'"

typedef enum {
	WELCOME_WIN = 0,
	SETUP_WIN,
} view_n;

typedef struct {
	char *zone_name;
	dpm_client_h dpm_client;
	bool create_done;
} appdata_s;

void _create_base_window(appdata_s *data);
void _create_welcome_view(appdata_s *data);
void _create_zone_popup(appdata_s *data);
void _create_setup_zone_view(appdata_s *data);

int _send_zone_manifest(const char *zone_name);

#endif /* __ZONE_SETUP_H__ */

/*
 * Tizen ODE application
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: SeolHeui Kim <s414.kim@samsung.com>
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

#ifndef __ODE_APP_H__
#define __ODE_APP_H__

#include <stdio.h>

#include <bundle.h>
#include <ui-gadget.h>
#include <app.h>
#include <Elementary.h>
#include <dlog.h>
#include <efl_extension.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "ode-app"

#if !defined(PACKAGE)
#define PACKAGE "org.tizen.ode-app"
#endif

typedef struct {
	Evas_Object* win;
	Evas_Object* nf;
	Evas_Object* layout;
	Evas_Object* conform;

	ui_gadget_h ug;
	char* encryption;

} appdata_s;


#endif /* __ODE_APP_H__ */

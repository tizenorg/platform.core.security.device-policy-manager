/*
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __ODE_APP_WIDGET_H__
#define __ODE_APP_WIDGET_H__

#include "ode-app.h"

Evas_Object* dpm_encryption_create_layout(Evas_Object* parent, const char* file, const char* group);
Evas_Object* dpm_encryption_create_navigation(Evas_Object* parent);
Evas_Object* dpm_encryption_create_button(Evas_Object* parent, const char* text, const char* style);
Evas_Object* dpm_encryption_create_textblock(Evas_Object *parent, const char *text, Evas_Textblock_Style *style);

#endif /* __ODE_APP_WIDGET_H__ */

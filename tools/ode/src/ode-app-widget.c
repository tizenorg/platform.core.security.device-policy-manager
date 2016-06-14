/*
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

#include "ode-app.h"

Evas_Object* dpm_encryption_create_layout(Evas_Object* parent, const char* file, const char* group)
{
	Evas_Object* layout = NULL;

	if (parent == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "parent is null");
		return NULL;
	}

	layout = elm_layout_add(parent);
	if (layout == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Cannot add layout");
		return NULL;
	}

	if ((file != NULL) && (group != NULL))
		elm_layout_file_set(layout, file, group);
	else
		elm_layout_theme_set(layout, "layout", "application", "default");

	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_show(layout);

	return layout;
}

Evas_Object* dpm_encryption_create_navigation(Evas_Object* parent)
{
	Evas_Object* navi_bar = NULL;

	if (parent == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "parent is null");
		return NULL;
	}

	navi_bar = elm_naviframe_add(parent);
	eext_object_event_callback_add(navi_bar, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	elm_object_part_content_set(parent, "elm.swallow.content", navi_bar);
	evas_object_show(navi_bar);

	return navi_bar;
}

Evas_Object* dpm_encryption_create_button(Evas_Object* parent, const char* text, const char* style)
{
	Evas_Object* btn = elm_button_add(parent);

	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(btn, text);

	if (style != NULL)
		elm_object_style_set(btn, style);

	evas_object_show(btn);

	return btn;
}

Evas_Object* dpm_encryption_create_textblock(Evas_Object* parent, const char* text, Evas_Textblock_Style* style)
{
	Evas_Object* txt = evas_object_textblock_add(parent);

	if (style != NULL)
		evas_object_textblock_style_set(txt, style);
	evas_object_textblock_text_markup_set(txt, text);
	evas_object_show(txt);

	return txt;

}

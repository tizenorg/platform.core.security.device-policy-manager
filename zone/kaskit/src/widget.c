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

#include "widget.h"

typedef struct {
	_toast_cb callback;
	void* data;
	Evas_Object *win;
} _toast_cb_s;

static void __win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

static void __toast_timeout_cb(void *data, Evas_Object *obj, void *event_info)
{
	_toast_cb_s* cb_data = (_toast_cb_s*)data;

        evas_object_del(cb_data->win);
	(cb_data->callback)(cb_data->data, obj);
	free(cb_data);

        return;
}

Evas_Object *_create_win(const char *package)
{
	Evas_Object *win;

	elm_app_base_scale_set(1.8);

	win  = elm_win_add(NULL, package, ELM_WIN_BASIC);
	elm_win_conformant_set(win, EINA_TRUE);
	elm_win_autodel_set(win, EINA_TRUE);
	elm_win_alpha_set(win, EINA_TRUE);

	evas_object_smart_callback_add(win, "delete,request", __win_delete_request_cb, NULL);

	eext_object_event_callback_add(win, EEXT_CALLBACK_BACK, __win_delete_request_cb, NULL);

	return win;
}

Evas_Object *_create_conformant(Evas_Object *parent)
{
	Evas_Object *conform = elm_conformant_add(parent);

	evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(parent, conform);

	evas_object_show(conform);

	return conform;
}

Evas_Object *_create_layout(Evas_Object *parent, char *file, const char *group)
{
	Evas_Object *layout = elm_layout_add(parent);

	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	if (file == NULL)
		elm_layout_theme_set(layout, "layout", "application", "default");
	else
		elm_layout_file_set(layout, file, group);

	evas_object_show(layout);

	return layout;
}

void _create_toast(char* text, _toast_cb callback, void* user_data)
{
	Evas_Object* win, *conform, *popup;

	win = _create_win("toast");
	conform = _create_conformant(win);
	popup = elm_popup_add(conform);
	elm_object_style_set(popup, "toast");
	elm_object_text_set(popup, text);

	_toast_cb_s* cb_data = malloc(sizeof(_toast_cb_s));
	cb_data->callback = callback;
	cb_data->data = user_data;
	cb_data->win = win;

	elm_popup_timeout_set(popup, 3.0);
	evas_object_smart_callback_add(popup, "timeout", __toast_timeout_cb, cb_data);

	evas_object_show(popup);
	evas_object_show(win);
}

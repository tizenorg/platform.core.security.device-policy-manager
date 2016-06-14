/*
 * Tizen Zone Keyguard application
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
#include <efl_util.h>
#include "widget.h"

static void __win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

Evas_Object *_create_win(const char *package)
{
	Evas_Object *win = elm_win_add(NULL, package, ELM_WIN_NOTIFICATION);
	efl_util_set_notification_window_level(win, EFL_UTIL_NOTIFICATION_LEVEL_DEFAULT);
	eext_win_keygrab_set(win, "XF86Home");

	elm_win_conformant_set(win, EINA_TRUE);

	evas_object_smart_callback_add(win, "delete,request", __win_delete_request_cb, NULL);

	elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_TRANSPARENT);

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

Evas_Object *_create_entry(Evas_Object *parent)
{
	Evas_Object *entry = elm_entry_add(parent);

	evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_entry_password_set(entry, EINA_TRUE);

	evas_object_show(entry);
	return entry;
}

static void __popup_timeout_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(obj);
}

static void __block_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(obj);
}

Evas_Object *_create_popup(Evas_Object *parent, const char *text, char *style)
{
	Evas_Object *popup = elm_popup_add(parent);

	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_style_set(popup, style);
	elm_object_text_set(popup, text);

	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, parent);
	if (!strcmp(style, "toast")) {
		elm_popup_timeout_set(popup, 3.0);
		evas_object_smart_callback_add(popup, "timeout", __popup_timeout_cb, NULL);
		evas_object_smart_callback_add(popup, "block,clicked", __block_clicked_cb, NULL);
	}
	evas_object_show(popup);
	return popup;
}
/*Evas_Object *_create_button(Evas_Object *parent, const char *text, const char *style)
{
	Evas_Object *btn = elm_button_add(parent);

	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(btn, text);

	if (style != NULL)
		elm_object_style_set(btn, style);

	evas_object_show(btn);

	return btn;
}

Evas_Object *_create_textblock(Evas_Object *parent, const char *text, char *style)
{
	Evas_Object *txt = evas_object_textblock_add(parent);
	Evas_Textblock_Style *text_st = NULL;

	if (style != NULL) {
		text_st = evas_textblock_style_new();
		evas_textblock_style_set(text_st, style);
		evas_object_textblock_style_set(txt, text_st);
	}

	evas_object_textblock_text_markup_set(txt, text);
	evas_object_show(txt);

	if (text_st != NULL)
		evas_textblock_style_free(text_st);

	return txt;
}*/

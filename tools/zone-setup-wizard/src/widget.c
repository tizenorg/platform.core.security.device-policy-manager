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

static void __win_delete_request_cb(void *data , Evas_Object *obj , void *event_info)
{
	ui_app_exit();
}

Evas_Object *_create_win(const char *package)
{
	Evas_Object *win = elm_win_util_standard_add(package, package);
	elm_win_conformant_set(win, EINA_TRUE);
	elm_win_autodel_set(win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(win, (const int *)(&rots), 4);
	}

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

Evas_Object *_create_button(Evas_Object *parent, const char *text, const char *style)
{
	Evas_Object *btn = elm_button_add(parent);

	evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(btn, text);

	if (style != NULL)
		elm_object_style_set(btn, style);

	evas_object_show(btn);

	return btn;
}

Evas_Object *_create_textblock(Evas_Object *parent, const char *text, Evas_Textblock_Style *style)
{
	Evas_Object *txt = evas_object_textblock_add(parent);

	if (style != NULL)
		evas_object_textblock_style_set(txt, style);
	evas_object_textblock_text_markup_set(txt, text);
	evas_object_show(txt);

	return txt;
}

Evas_Object *_create_progressbar(Evas_Object *parent, const char *style)
{
	Evas_Object *progressbar = elm_progressbar_add(parent);

	elm_object_style_set(progressbar, style);
	evas_object_size_hint_align_set(progressbar, EVAS_HINT_FILL, 0.5);
	evas_object_size_hint_weight_set(progressbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_progressbar_pulse(progressbar, EINA_TRUE);
	elm_progressbar_value_set(progressbar, 0.0);
	evas_object_show(progressbar);

	return progressbar;
}

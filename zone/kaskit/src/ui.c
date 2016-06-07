/*
 * Tizen Krate launcher application
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
#include "kaskit.h"
#include "widget.h"
#include "conf.h"

uidata_s ud = {0, };

static Eina_Bool __hardware_key_cb(void *data, int type, void *event)
{
	Evas_Event_Key_Down *ev = event;

	if (!strcmp(ev->keyname, "XF86Back")) {
		ui_app_exit();
	}

	return EINA_TRUE;
}

static void __block_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
	ui_app_exit();
}

static void __app_icon_up_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	_icon_clicked_cb(data);
	free(data);
}

static void __set_kaskit_layout(const char *name)
{
	ud.panel = _create_layout(ud.layout, ud.edj_path, "popup_layout");

	elm_object_part_text_set(ud.panel, "popup_title", name);
	elm_object_part_content_set(ud.layout, "popup_window", ud.panel);

	ud.scroller = elm_scroller_add(ud.panel);
	elm_scroller_bounce_set(ud.scroller, EINA_FALSE, EINA_TRUE);
	elm_scroller_policy_set(ud.scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_ON);
	elm_object_part_content_set(ud.panel, "popup_content", ud.scroller);

	ud.app_view = elm_table_add(ud.scroller);
	elm_table_homogeneous_set(ud.app_view, EINA_TRUE);
	elm_object_content_set(ud.scroller, ud.app_view);

	return;
}

static char* __get_res_path(const char* file)
{
	char *res_path = NULL;
	char edj_path[PATH_MAX] = "\0";

	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
	}
	snprintf(edj_path, PATH_MAX, "%s%s", res_path, file);

	free(res_path);

	return strdup(edj_path);
}

void _create_kaskit_window(const char *krate_name)
{
	ud.edj_path = __get_res_path(PACKAGE ".edj");
	ud.win = _create_win(PACKAGE);
	ud.conform = _create_conformant(ud.win);
	ud.layout = _create_layout(ud.conform, ud.edj_path, "main_window");
	elm_object_content_set(ud.conform, ud.layout);

	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, __hardware_key_cb, NULL);
	elm_object_signal_callback_add(ud.layout, "bg_clicked", "layout", __block_clicked_cb, NULL);

	__set_kaskit_layout(krate_name);

	evas_object_show(ud.win);
	return;
}

void _create_app_icon(char* pkg_id, char* app_id, char* label, char* icon)
{
	static int index = 0;
	char string[1024] = {0, };
	Evas_Object *icon_layout;
	Evas_Object *icon_image;

	icon_layout = _create_layout(ud.app_view, ud.edj_path, "icon");
	evas_object_size_hint_weight_set(icon_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(icon_layout, 0.0, 0.0);

	if (!ecore_file_can_read(icon)) {
		free(icon);		
		icon = __get_res_path("images/default_app_icon.png");
		dlog_print(DLOG_ERROR, LOG_TAG, "default icons : %s %s %s", label, icon, app_id);
	}

	icon_image = elm_image_add(icon_layout);
	elm_image_file_set(icon_image, icon, NULL);
	evas_object_size_hint_min_set(icon_image, ICON_IMG_SIZE, ICON_IMG_SIZE);
	evas_object_size_hint_max_set(icon_image, ICON_IMG_SIZE, ICON_IMG_SIZE);

        snprintf(string, sizeof(string), "<font_size=%d><color=#%s><shadow_color=#%s>%s</shadow_color></color></font_size>",
		ICON_TXT_SIZE_NORMAL, ICON_TXT_COLOR, ICON_TXT_SHADOW_COLOR,
		label);
	elm_object_part_text_set(icon_layout, "icon_name", string);

	evas_object_show(icon_image);
	evas_object_show(icon_layout);

	elm_object_part_content_set(icon_layout, "icon_content", icon_image);

	evas_object_event_callback_add(icon_layout, EVAS_CALLBACK_MOUSE_UP, __app_icon_up_cb, strdup(app_id));

	elm_table_pack(ud.app_view, icon_layout, index % 3, index / 3, 1, 1);

	index++;

	dlog_print(DLOG_ERROR, LOG_TAG, "show icons : %s %s %s", label, icon, app_id);
}

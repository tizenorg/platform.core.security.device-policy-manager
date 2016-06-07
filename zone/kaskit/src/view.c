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

#define RESOLUTION_SCALE_W	1.0
#define RESOLUTION_SCALE_H	1.0

#define ICON_LAYOUT_W		((720) * (RESOLUTION_SCALE_W) / 4)
#define ICON_LAYOUT_H		((1060) * (RESOLUTION_SCALE_H) / 5)

#define ICON_IMG_SIZE 		((118) * (RESOLUTION_SCALE_W))
#define ICON_TXT_SIZE_NORMAL	((int)((28) * (RESOLUTION_SCALE_H)))
#define ICON_TXT_COLOR		"FFFFFFFF"
#define ICON_TXT_SHADOW_COLOR	"88888888"

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
	return;
}

static void __set_kaskit_layout(const char *name)
{
	ud.panel = _create_layout(ud.layout, ud.edj_path, "kaskit_layout");

	elm_object_part_text_set(ud.panel, "kaskit_title", name);
	elm_object_part_content_set(ud.layout, "kaskit_window", ud.panel);
*
	ud.scroller = elm_scroller_add(ud.panel);
	elm_scroller_content_min_limit(ud.scroller, EINA_FALSE, EINA_FALSE);
	elm_scroller_bounce_set(ud.scroller, EINA_FALSE, EINA_TRUE);
	elm_scroller_policy_set(ud.scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
	elm_scroller_page_scroll_limit_set(ud.scroller, 1, 1);
	evas_object_size_hint_weight_set(ud.scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	elm_scroller_loop_set(ud.scroller, EINA_TRUE, EINA_FALSE);
//	elm_scroller_page_size_set(ud.scroller, APPS_VIEW_W, APPS_VIEW_H);
//	evas_object_resize(ud.scroller, APPS_VIEW_W , APPS_VIEW_H);

	ud.box = elm_box_add(ud.scroller);
	elm_box_horizontal_set(ud.box, EINA_TRUE);
	elm_box_align_set(ud.box, 0.5, 0.5);
	evas_object_show(ud.box);

	elm_object_content_set(ud.scroller, ud.box);

	elm_object_part_content_set(ud.panel, "kaskit_content", ud.scroller);

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

static char* __get_edj_path()
{
	if (ud.edj_path != NULL) {
		return ud.edj_path;
	}

	ud.edj_path = __get_res_path(PACKAGE ".edj");

	return ud.edj_path;
}

void _create_kaskit_window(const char *krate_name)
{
	ud.win = _create_win(PACKAGE);
	ud.conform = _create_conformant(ud.win);
	ud.layout = _create_layout(ud.conform, __get_edj_path(), "main_window");
	elm_object_content_set(ud.conform, ud.layout);

	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, __hardware_key_cb, NULL);
	elm_object_signal_callback_add(ud.layout, "bg_clicked", "layout", __block_clicked_cb, NULL);

	__set_kaskit_layout(krate_name);

	evas_object_show(ud.win);
	return;
}

void _create_app_icon(char* pkg_id, char* app_id, char* label, char* icon)
{
	char style_string[1024] = {0, };
	char *default_icon;
	Evas_Object *icon_layout;
	Evas_Object *icon_image;
	Evas_Object *rect;

	icon_layout = _create_layout(ud.box, __get_edj_path(), "icon");
	evas_object_size_hint_weight_set(icon_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	rect = evas_object_rectangle_add(evas_object_evas_get(icon_layout));
	evas_object_color_set(rect, 0, 0, 0, 0);
	evas_object_size_hint_min_set(rect, ICON_LAYOUT_W, ICON_LAYOUT_H);
	evas_object_size_hint_max_set(rect, ICON_LAYOUT_W, ICON_LAYOUT_H);
	elm_object_part_content_set(icon_layout, "size_setter", rect);

	icon_image = elm_image_add(icon_layout);
	if (ecore_file_can_read(icon)) {
	        elm_image_file_set(icon_image, icon, NULL);
	} else {
		default_icon = __get_res_path(PACKAGE ".edj");
		elm_image_file_set(icon_image, default_icon, NULL);
		free(default_icon);		
	}

	evas_object_size_hint_min_set(icon_image, ICON_IMG_SIZE, ICON_IMG_SIZE);
	evas_object_size_hint_max_set(icon_image, ICON_IMG_SIZE, ICON_IMG_SIZE);
	elm_object_part_content_set(icon_layout, "icon_content", icon_image);

        snprintf(style_string, sizeof(style_string), "<font_size=%d><color=#%s><shadow_color=#%s>%s</shadow_color></color></font_size>",
		ICON_TXT_SIZE_NORMAL, ICON_TXT_COLOR, ICON_TXT_SHADOW_COLOR,
		label);
	elm_object_part_text_set(icon_layout, "icon_name", style_string);

	evas_object_show(icon_image);
	evas_object_show(icon_layout);

	dlog_print(DLOG_ERROR, LOG_TAG, "show icons : %s %s %s", label, icon, app_id);
}

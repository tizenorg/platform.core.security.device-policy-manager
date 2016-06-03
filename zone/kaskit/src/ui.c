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
#include "kaskit.h"
#include "widget.h"

typedef struct {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *layout;
	char *edj_path;
} uidata_s;

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
	Evas_Object *panel = _create_layout(ud.layout, ud.edj_path, "kaskit_layout");

	elm_object_part_text_set(panel, "folder_unname", name);
	elm_object_part_content_set(ud.layout, "kaskit_window", panel);

	return;
}

void _create_kaskit_window(const char *krate_name)
{
	char *res_path = NULL;
	char edj_path[PATH_MAX] = "\0";

	/* Get EDJ path */
	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
	}
	snprintf(edj_path, PATH_MAX, "%s%s", res_path, "org.tizen.kaskit.edj");

	ud.edj_path = strdup(edj_path);
	free(res_path);

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

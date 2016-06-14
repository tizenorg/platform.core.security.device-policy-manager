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
#include "keyguard.h"
#include "widget.h"

typedef struct {
	Evas_Object *win;
	Evas_Object *layout;
	char *edj_path;
} uidata_s;

uidata_s ud = {0, };

static void __change_info_text(const char *text)
{
	elm_object_part_text_set(ud.layout, "entry_info_text", text);
}

static void __entry_change_cb(void *data, Evas_Object *obj, void *event_info)
{
	char text[PATH_MAX];
	int left_attempts = 10; /* [TBD] get left attempts number */

	snprintf(text, PATH_MAX, "%d attempts left", left_attempts);
	__change_info_text(text);
}

static void __entry_activate_cb(void *data, Evas_Object *obj, void *event_info)
{
	char password[10] = "asdf"; /* [TBD] get krate password from dpm */
	const char *input = elm_object_text_get(obj);

	if (!strcmp(password, input)) {
		ui_app_exit();
	} else {
		elm_entry_input_panel_hide(obj);
		__change_info_text("Incorrect Password. Try again.");
	}
}

static void __text_clicked_cb(void *data, Evas_Object *obj, const char *emission, const char *source)
{
	Evas_Object *popup = _create_popup(ud.win, "Reset unlock method", "Contact your administrator to reset your Krate unlock method.", "default");
	evas_object_show(popup);
}

void _create_keyguard_window(appdata_s *ad)
{
	Evas_Object *conform;
	Evas_Object *entry;

	char edj_path[PATH_MAX] = "\0";
	char *res_path = NULL;

	/* Get EDJ path */
	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
	}
	snprintf(edj_path, PATH_MAX, "%s%s", res_path, "org.tizen.keyguard.edj");

	ud.edj_path = strdup(edj_path);
	free(res_path);

	/* Create main UI widget */
	ud.win = _create_win(PACKAGE);
	conform = _create_conformant(ud.win);
	ud.layout = _create_layout(conform, ud.edj_path, "base_layout");

	entry = _create_entry(ud.layout);
	elm_object_part_content_set(ud.layout, "entry", entry);
	elm_entry_single_line_set(entry, EINA_TRUE);

	evas_object_smart_callback_add(entry, "activated", __entry_activate_cb, NULL);
	evas_object_smart_callback_add(entry, "press", __entry_change_cb, NULL);

	elm_object_signal_callback_add(ud.layout, "text_clicked", "layout", __text_clicked_cb, NULL);

	elm_object_content_set(conform, ud.layout);

	evas_object_show(ud.win);
	return;
}

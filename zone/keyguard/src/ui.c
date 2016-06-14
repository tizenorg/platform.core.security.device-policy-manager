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

#define PASSWORD_SUCCESS 1
#define PASSWORD_FAIL 0

typedef struct {
	Evas_Object *win;
	char *edj_path;
	int status;
} uidata_s;

uidata_s ud = {0, };

void _popup_delete_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	if (ud.status == PASSWORD_SUCCESS)
		ui_app_exit();

	return;
}

void _entry_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *popup;
	char password[10] = "asdf"; /* [TBD] get krate password from dpm */
	const char *input = elm_object_text_get(obj);

	if (!strcmp(password, input)) {
		ud.status = PASSWORD_SUCCESS;
		popup = _create_popup(ud.win, "success", "toast");
	} else {
		ud.status = PASSWORD_FAIL;
		popup = _create_popup(ud.win, "Password is wrong.", "toast");
	}

	evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, _popup_delete_cb, NULL);
}

void _create_keyguard_window(appdata_s *ad)
{
	Evas_Object *conform, *layout;
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
	layout = _create_layout(conform, ud.edj_path, "base_layout");

	entry = _create_entry(layout);
	elm_object_part_content_set(layout, "entry", entry);
	elm_entry_single_line_set(entry, EINA_TRUE);
	evas_object_smart_callback_add(entry, "activated", _entry_changed_cb, NULL);

	//info_label = _create_label("Tap here when you forgot password.");

	elm_object_content_set(conform, layout);

	evas_object_show(ud.win);
	return;
}

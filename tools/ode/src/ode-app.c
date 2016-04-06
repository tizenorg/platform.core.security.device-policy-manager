/*
 * Tizen ODE application
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: SeolHeui Kim <s414.kim@samsung.com>
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

static void __win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void __ug_layout_cb(ui_gadget_h ug, enum ug_mode mode, void *priv)
{
	Evas_Object *ug_layout;
	ug_layout = (Evas_Object *) ug_get_layout(ug);
	if (ug_layout == NULL)
		return;

	switch (mode) {
	case UG_MODE_FULLVIEW:
		evas_object_size_hint_weight_set(ug_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_show(ug_layout);
		break;
	default:
		break;
	}

	return;
}

static void __ug_destroy_cb(ui_gadget_h ug, void *priv)
{
	ug_destroy(ug);
	ui_app_exit();
}

static int __create_ug(const char *ug_name)
{
	struct ug_cbs cbs;

	cbs.layout_cb = __ug_layout_cb;
	cbs.destroy_cb = __ug_destroy_cb;
	cbs.priv = NULL;

	if (ug_name == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "ug_name is NULL");
		return -1;
	}

	if (ug_create(NULL, ug_name, UG_MODE_FULLVIEW, NULL, &cbs) == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to create ug");
		return -1;
	}

	return 0;
}

static void __create_ui_layout(void)
{
	Evas_Object *win, *conform, *layout;

	win = elm_win_add(NULL, "ode-app", ELM_WIN_BASIC);
	elm_win_conformant_set(win, EINA_TRUE);
	elm_win_autodel_set(win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(win, "delete, request", __win_delete_request_cb, NULL);

	elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_OPAQUE);

	conform = elm_conformant_add(win);
	evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, conform);
	evas_object_show(conform);

	layout = elm_layout_add(conform);
	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(layout, "layout", "application", "default");
	elm_object_content_set(conform, layout);
	evas_object_show(layout);

	evas_object_show(win);

	UG_INIT_EFL(win, UG_OPT_INDICATOR_ENABLE);

	return;
}

static bool __app_create(void *data)
{
	return true;
}

static void __app_control(app_control_h app_control, void *data)
{
	char *ug_type = (char *) data;
	int ret = 0;

	ret = app_control_get_extra_data(app_control, "viewtype", &ug_type);
	if (ret != APP_CONTROL_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get bundle data. error:%d", ret);

	elm_app_base_scale_set(1.8);

	__create_ui_layout();

	if (!strcmp(ug_type, "encryption"))
		ret = __create_ug(ENCRYPTION_UG);
	else if (!strcmp(ug_type, "decryption"))
		ret = __create_ug(DECRYPTION_UG);

	if (ret != 0)
		ui_app_exit();

	return;
}

static void __app_pause(void *data)
{
	return;
}

static void __app_resume(void *data)
{
	return;
}

static void __app_terminate(void *data)
{
	return;
}

int main(int argc, char *argv[])
{
	char ug_name[1024] = "\0";
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = { 0, };

	event_callback.create = __app_create;
	event_callback.terminate = __app_terminate;
	event_callback.pause = __app_pause;
	event_callback.resume = __app_resume;
	event_callback.app_control = __app_control;

	ret = ui_app_main(argc, argv, &event_callback, ug_name);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);

	return 0;
}

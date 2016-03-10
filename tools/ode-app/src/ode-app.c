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

static void win_delete_request_cb(void* data, Evas_Object* obj, void* event_info)
{
	ui_app_exit();
}

static void ug_layout_cb(ui_gadget_h ug, enum ug_mode mode, void* priv)
{
	Evas_Object* ug_layout;
	ug_layout = (Evas_Object*) ug_get_layout(ug);
	if (ug_layout == NULL) {
		return ;
	}

	switch (mode) {
	case UG_MODE_FULLVIEW:
		evas_object_size_hint_weight_set(ug_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_show(ug_layout);
		break;
	default:
		break;
	}

}

static void ug_destroy_cb(ui_gadget_h ug, void* priv)
{
	appdata_s* ad = (appdata_s*)priv;
	if (ad->ug) {
		ug_destroy(ad->ug);
		ad->ug = NULL;
	}
	ui_app_exit();
}

static void get_ug(void* data, const char* opt)
{
	appdata_s* ad = (appdata_s*)data;

	UG_INIT_EFL(ad->win, UG_OPT_INDICATOR_ENABLE);
	struct ug_cbs cbs;
	cbs.layout_cb = ug_layout_cb;
	cbs.destroy_cb = ug_destroy_cb;
	cbs.priv = (void*)ad;

	/* [TBD] change UG name */
	if (!strcmp(opt, "encryption")) {
		ad->ug = ug_create(NULL, "lockscreen-options", UG_MODE_FULLVIEW, NULL, &cbs);
	} else {
		ad->ug = ug_create(NULL, "setting-privacy-efl", UG_MODE_FULLVIEW, NULL, &cbs);
	}
}

static void app_start(void* data)
{
	appdata_s* ad = (appdata_s*)data;

	ad->conform = elm_conformant_add(ad->win);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	ad->layout = elm_layout_add(ad->conform);
	evas_object_size_hint_weight_set(ad->layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(ad->layout, "layout", "application", "default");
	elm_object_content_set(ad->conform, ad->layout);
	evas_object_show(ad->layout);

	get_ug(ad, ad->encryption);

	return ;
}

static bool app_create(void* data)
{
	appdata_s* ad = (appdata_s*)data;

	char* opt = NULL;
	bundle* bd = bundle_create();
	bundle_get_str(bd, "viewtype", &opt);
	if (opt == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get bundle data");
	}

	ad->encryption = opt;

	elm_app_base_scale_set(1.8);

	ad->win = elm_win_add(NULL, "ug-caller", ELM_WIN_BASIC);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int*)(&rots), 4);
	}
	evas_object_smart_callback_add(ad->win, "delete, request", win_delete_request_cb, NULL);

	evas_object_show(ad->win);

	app_start(ad);

	return true;
}

static void
app_control(app_control_h app_control, void* data)
{
	/* Handle the launch request. */
}

static void
app_pause(void* data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void* data)
{
	/* Take necessary actions when application becomes visible. */
}

static void app_terminate(void* data)
{
	appdata_s* ad = (appdata_s*)data;
	if (ad->win) {
		evas_object_del(ad->win);
		ad->win = NULL;
	}

	if (ad->layout) {
		evas_object_del(ad->layout);
	}

	if (ad->conform) {
		evas_object_del(ad->conform);
	}

	if (ad->ug) {
		ug_destroy(ad->ug);
	}
}

int main(int argc, char* argv[])
{
	appdata_s ad = {0, };
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main() is failed. err = %d", ret);
	}

	return 0;
}

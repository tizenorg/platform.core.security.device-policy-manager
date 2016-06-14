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

static Evas_Object* create_mainview(appdata_s* ad)
{
	Evas_Object* base_layout = NULL;
	Evas_Object* navi_bar = NULL;

	base_layout = dpm_encryption_create_layout(ad->conform, NULL, NULL);

	navi_bar = dpm_encryption_create_navigation(base_layout);
	elm_naviframe_prev_btn_auto_pushed_set(navi_bar, EINA_TRUE);
	ad->navi_bar = navi_bar;

	switch (ad->view_type) {
	case ENCRYPT_DEVICE:
		dpm_encryption_create_view(ad);
		break;
	case DECRYPT_DEVICE:
		dpm_decryption_create_view(ad);
		break;
	case ENCRYPT_SD_CARD:
		dpm_encryption_sd_card_create_view(ad);
		break;
	case DECRYPT_SD_CARD:
		dpm_decryption_sd_card_create_view(ad);
		break;
	}
	return base_layout;
}

static void win_delete_request_cb(void* data, Evas_Object* obj, void* event_info)
{
	ui_app_exit();
}

static Eina_Bool _home_key_cb(void *data, int type, void *event)
{
	Evas_Event_Key_Down *ev = event;
	if (!strcmp(ev->keyname, "XF86Home"))
		ui_app_exit();
	return EINA_TRUE;
}

static int create_base_gui(appdata_s* ad)
{
	dlog_print(DLOG_DEBUG, LOG_TAG, "### create base gui start  ###");
	char* res_path = NULL;
	char edj_path[PATH_MAX] = "\0";

	/* get EDJ path */
	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
		return 1;
	}
	snprintf(edj_path, PATH_MAX, "%s%s", res_path, "org.tizen.ode.edj");
	ad->edj_path = strdup(edj_path);
	free(res_path);

	/* Window */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_conformant_set(ad->win, EINA_TRUE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);

	/* Conformant */
	ad->conform = elm_conformant_add(ad->win);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	/* Indicator */
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_TRANSPARENT);

	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, _home_key_cb, NULL);

	ad->base = create_mainview(ad);
	elm_object_content_set(ad->conform, ad->base);
	evas_object_show(ad->win);

	return 0;
}

static bool app_create(void* data)
{
	return true;
}

static void app_control(app_control_h app_control, void* data)
{
	/* Handle the launch request. */
	dlog_print(DLOG_ERROR, LOG_TAG, "### app create start ###");
	appdata_s* ad = &global_ad;
	int ret;
	char *viewtype = NULL;

	ret = app_control_get_extra_data(app_control, "viewtype", &viewtype);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get viewtype");
		ui_app_exit();
	}

	dlog_print(DLOG_DEBUG, LOG_TAG, "viewtype is %s", viewtype);
	if (!strcmp(viewtype, "ENCRYPT_DEVICE"))
		ad->view_type = ENCRYPT_DEVICE;
	else if (!strcmp(viewtype, "DECRYPT_DEVICE"))
		ad->view_type = DECRYPT_DEVICE;
	else if (!strcmp(viewtype, "ENCRYPT_SD_CARD"))
		ad->view_type = ENCRYPT_SD_CARD;
	else if (!strcmp(viewtype, "DECRYPT_SD_CARD"))
		ad->view_type = DECRYPT_SD_CARD;

	elm_app_base_scale_set(1.8);

	if (create_base_gui(ad) != 0) {
		dlog_print(DLOG_ERROR, LOG_TAG, "### create base gui fail ###");
		return;
	}
}

static void app_pause(void* data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void app_resume(void* data)
{
	/* Take necessary actions when application becomes visible. */
}

static void app_terminate(void* data)
{
	/* unregister vconf notify callback */
	int ret;

	ret = vconf_ignore_key_changed(VCONFKEY_SETAPPL_SCREEN_LOCK_TYPE_INT, locktype_status_changed_cb);
	if (ret != 0)
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to unregister lockscreen type callback");

	ret = vconf_ignore_key_changed(VCONFKEY_SYSMAN_MMC_STATUS, sdcard_status_changed_cb);
	if (ret != 0)
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to unregister sdcard callback");

	/* unregister runtime info callback */
	ret = runtime_info_unset_changed_cb(RUNTIME_INFO_KEY_CHARGER_CONNECTED);
	if (ret != 0)
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to unregister runtime info callback");

	/* unregister battery percent callback */
	ret = device_remove_callback(DEVICE_CALLBACK_BATTERY_CAPACITY, battery_changed_cb);
	if (ret != 0)
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to unregister battery capacity callback");
}

static void ui_app_lang_changed(app_event_info_h event_info, void* user_data)
{
	/* APP_EVENT_LANGUAGE_CHANGED */
	char* locale = NULL;

	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);

	return;
}

static void ui_app_orient_changed(app_event_info_h event_info, void* user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED */
	return;
}

static void ui_app_region_changed(app_event_info_h event_info, void* user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED */
}

static void ui_app_low_battery(app_event_info_h event_info, void* user_data)
{
	/*APP_EVENT_LOW_BATTERY */
}

static void ui_app_low_memory(app_event_info_h event_info, void* user_data)
{
	/*APP_EVENT_LOW_MEMORY */
}

int main(int argc, char* argv[])
{
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = { 0, };
	app_event_handler_h handlers[5] = { NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &global_ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &global_ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &global_ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &global_ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &global_ad);
	ui_app_remove_event_handler(handlers[APP_EVENT_LOW_MEMORY]);

	ret = ui_app_main(argc, argv, &event_callback, &global_ad);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main is failed. err = %d", ret);
	return ret;

}

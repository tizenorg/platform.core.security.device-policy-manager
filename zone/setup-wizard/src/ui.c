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
#include <notification.h>
#include "zone-setup.h"
#include "widget.h"

static void __create_welcome_view(appdata_s *ad);
static void __create_setup_view(appdata_s *ad);

typedef struct {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *nf;
	Evas_Object *timer;
	char *edj_path;
} uidata_s;

uidata_s ud = {0, };

static int __set_notification(notification_h noti_handle, app_control_h app_control)
{
	int ret = 0;

	ret = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_TITLE, IDS_DPM_NOTI_CREATE_ZONE, NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	if (ret != NOTIFICATION_ERROR_NONE)
		return -1;

	ret = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_CONTENT, IDS_DPM_NOTI_BODY_CREATE_ZONE, NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	if (ret != NOTIFICATION_ERROR_NONE)
		return -1;

	ret = notification_set_display_applist(noti_handle, NOTIFICATION_DISPLAY_APP_ALL);
	if (ret != NOTIFICATION_ERROR_NONE)
		return -1;

	ret = notification_set_image(noti_handle, NOTIFICATION_IMAGE_TYPE_THUMBNAIL, DPM_SYSPOPUP_ICON_PATH);
	if (ret != NOTIFICATION_ERROR_NONE)
		return -1;

	ret = notification_set_launch_option(noti_handle, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, app_control);
	if (ret != NOTIFICATION_ERROR_NONE)
		return -1;

	return ret;
}

static void __create_notification(app_control_h app_control)
{
	notification_h noti_handle = NULL;
	int ret = 0;

	noti_handle = notification_create(NOTIFICATION_TYPE_NOTI);

	ret = __set_notification(noti_handle, app_control);
	if (ret != NOTIFICATION_ERROR_NONE) {
		notification_free(noti_handle);
		app_control_destroy(app_control);
		return;
	}

	notification_post(noti_handle);
	notification_free(noti_handle);
	app_control_destroy(app_control);

	return;
}

static Eina_Bool __naviframe_pop_cb(void *data, Elm_Object_Item *it)
{
	app_control_h svc = (app_control_h) data;

	__create_notification(svc);
	ui_app_exit();

	return EINA_FALSE;
}

static void __prev_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	app_control_h svc = (app_control_h) data;

	__create_notification(svc);
	ui_app_exit();

	return;
}

static void __next_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;

	if (_send_zone_provision_data(ad->zone_name, ad->provision_path) != 0)
		ui_app_exit();

	__create_setup_view(ad);
	return;
}

static Eina_Bool __home_key_cb(void *data, int type, void *event)
{
	app_control_h app_control = (app_control_h) data;
	Evas_Event_Key_Down *ev = event;

	if (!strcmp(ev->keyname, "XF86Home")) {
		__create_notification(app_control);
		ui_app_exit();
	}

	return EINA_TRUE;
}

static void __set_two_btn_bottom_layout(Evas_Object *layout, appdata_s *ad, const char *prev_btn_text, const char *next_btn_text)
{
	Evas_Object *bottom_layout;
	Evas_Object *prev_btn, *next_btn;

	bottom_layout = _create_layout(layout, ud.edj_path, "two_button_layout");

	prev_btn = _create_button(bottom_layout, prev_btn_text, "bottom");
	elm_object_part_content_set(bottom_layout, "prev_button", prev_btn);
	evas_object_smart_callback_add(prev_btn, "clicked", __prev_btn_cb, ad->app_control);

	next_btn = _create_button(bottom_layout, next_btn_text, "bottom");
	elm_object_part_content_set(bottom_layout, "next_button", next_btn);
	evas_object_smart_callback_add(next_btn, "clicked", __next_btn_cb, ad);

	elm_object_part_content_set(layout, "bottom_layout", bottom_layout);
	return;
}

static Eina_Bool __progressbar_timer_cb(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	if (ad->create_done) {
		ecore_timer_del(ud.timer);
		ui_app_exit();
		return ECORE_CALLBACK_CANCEL;
	}

	return ECORE_CALLBACK_RENEW;
}

void _create_base_window(appdata_s *ad)
{
	Evas_Object *layout;

	char edj_path[PATH_MAX] = "\0";
	char *res_path = NULL;

	/* Initialize data */
	ad->create_done = false;

	/* Get EDJ path */
	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
	}
	snprintf(edj_path, PATH_MAX, "%s%s", res_path, "org.tizen.zone-setup-wizard.edj");

	ud.edj_path = strdup(edj_path);
	free(res_path);

	/* Create main UI widget */
	ud.win = _create_win(PACKAGE);
	ud.conform = _create_conformant(ud.win);
	layout = _create_layout(ud.conform, NULL, NULL);
	elm_object_content_set(ud.conform, layout);
	ud.nf = elm_naviframe_add(layout);

	eext_win_keygrab_set(ud.win, "XF86HOME");
	ecore_event_handler_add(ECORE_EVENT_KEY_DOWN, __home_key_cb, ad->app_control);


	__create_welcome_view(ad);

	elm_object_part_content_set(layout, "elm.swallow.content", ud.nf);
	eext_object_event_callback_add(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);

	evas_object_show(ud.win);
	return;
}

static void __create_welcome_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Evas_Object *layout, *welcome_layout;
	Evas_Object *text;
	Evas_Textblock_Style *text_st;

	elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");
	welcome_layout = _create_layout(layout, ud.edj_path, "welcome_layout");

	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, WELCOME_TEXT_STYLE);
	text = _create_textblock(welcome_layout, WELCOME_INFO_MESSAGE, text_st);
	elm_object_part_content_set(welcome_layout, "content_text", text);
	evas_textblock_style_free(text_st);

	elm_object_part_content_set(layout, "content_layout", welcome_layout);

	__set_two_btn_bottom_layout(layout, ad, "Cancel", "Set up");

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, ad->app_control);

	return;
}

static void __create_setup_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Evas_Object *setup_layout;
	Evas_Object *progressbar, *text;
	Evas_Textblock_Style *text_st;

	eext_object_event_callback_del(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb);

	setup_layout = _create_layout(ud.nf, ud.edj_path, "setup_layout");

	progressbar = _create_progressbar(setup_layout, "pending");
	elm_object_part_content_set(setup_layout, "progressbar", progressbar);

	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, SETUP_TEXT_STYLE);
	text = _create_textblock(setup_layout, SETUP_INFO_MESSAGE, text_st);
	elm_object_part_content_set(setup_layout, "content_text", text);
	evas_textblock_style_free(text_st);

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, setup_layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, NULL);

	/* set progressbar timer callback */
	ud.timer = ecore_timer_add(0.1, __progressbar_timer_cb, ad);

	return;
}

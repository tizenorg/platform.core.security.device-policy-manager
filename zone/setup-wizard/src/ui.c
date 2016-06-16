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

#define GROUPINDEX_UNLOCK_METHOD 0
#define GROUPINDEX_SECURITY_OPTIONS 1

#define UNLOCKMETHOD_SIMPLE_PASSWORD 0
#define UNLOCKMETHOD_PASSWORD 1

static void __create_welcome_view(appdata_s *ad);
static void __create_setup_view(appdata_s *ad);
static void __create_security_view(appdata_s *ad);

typedef struct {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *nf;
	Evas_Object *timer;
	char *edj_path;
	Evas_Object *radio_main;
	int unlock_method;
} uidata_s;

uidata_s ud = {0, };

static int __set_notification(notification_h noti_handle, app_control_h app_control)
{
	int ret = 0;
	char *mode = NULL;
	char *noti_text[2][2] = {
		{NOTI_CREATE_ZONE, NOTI_BODY_CREATE_ZONE},
		{NOTI_REMOVE_ZONE, NOTI_BODY_REMOVE_ZONE}
	};
	char **text = NULL;

	if (app_control_get_extra_data(app_control, "mode", &mode) != APP_CONTROL_ERROR_NONE)
		return -1;

	if (!strcmp(mode, "create"))
		text = noti_text[0];
	else
		text = noti_text[1];

	ret = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_TITLE, text[0], NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	if (ret != NOTIFICATION_ERROR_NONE)
		return -1;

	ret = notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_CONTENT, text[1], NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
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

	if (!strcmp(ad->mode, "create")) {
		if (_send_zone_create_request(ad) != 0)
			ui_app_exit();
	} else if (!strcmp(ad->mode, "remove")) {
		if (_send_zone_remove_request(ad) != 0)
			ui_app_exit();
	}

	__create_setup_view(ad);
	return;
}

static void __security_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	/* [TBD] send unlock method type to authfw : ud.unlock_method */
	ui_app_exit();
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

static void __set_one_btn_bottom_layout(Evas_Object *layout, appdata_s *ad, const char *btn_text)
{
	Evas_Object *bottom_layout;
	Evas_Object *btn;

	bottom_layout = _create_layout(layout, ud.edj_path, "one_button_layout");
	btn = _create_button(bottom_layout, btn_text, "bottom");
	evas_object_smart_callback_add(btn, "clicked", __security_btn_cb, NULL);

	elm_object_part_content_set(bottom_layout, "button", btn);
	elm_object_part_content_set(layout, "bottom_layout", bottom_layout);

	return;
}

static Eina_Bool __progressbar_timer_cb(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	if (ad->request_done) {
		ecore_timer_del(ud.timer);
		if (!strcmp(ad->mode, "create"))
			__create_security_view(ad);
		else
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
	ad->request_done = false;
	ud.unlock_method = 0;

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
	Evas_Object *title, *content;

	char *welcome_text[2][4] = {
		{WELCOME_MESSAGE_TITLE, WELCOME_MESSAGE_CONTENT, CANCEL_BUTTON, SETUP_BUTTON},
		{DELETE_MESSAGE_TITLE, DELETE_MESSAGE_CONTENT, CANCEL_BUTTON, REMOVE_BUTTON}
	};
	char **text = NULL;

	if (!strcmp(ad->mode, "create"))
		text = welcome_text[0];
	else
		text = welcome_text[1];

	elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");
	welcome_layout = _create_layout(layout, ud.edj_path, "welcome_layout");

	title = _create_textblock(welcome_layout, text[0], SUB_TITLE_STYLE_B);
	content = _create_textblock(welcome_layout, text[1], SUB_CONTENT_STYLE_B);

	elm_object_part_content_set(welcome_layout, "message_title", title);
	elm_object_part_content_set(welcome_layout, "message_content", content);

	elm_object_part_content_set(layout, "content_layout", welcome_layout);

	__set_two_btn_bottom_layout(layout, ad, text[2], text[3]);

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, ad->app_control);

	return;
}

static void __create_setup_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Evas_Object *setup_layout;
	Evas_Object *progressbar;
	Evas_Object *title, *content;

	char *setup_text[2][2] = {
		{SETUP_MESSAGE_TITLE, SETUP_MESSAGE_CONTENT},
		{DELETE_ONGOING_TITLE, DELETE_ONGOING_CONTENT}
	};
	char **text = NULL;

	if (!strcmp(ad->mode, "create"))
		text = setup_text[0];
	else
		text = setup_text[1];

	eext_object_event_callback_del(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb);

	setup_layout = _create_layout(ud.nf, ud.edj_path, "setup_layout");

	title = _create_textblock(setup_layout, text[0], SUB_TITLE_STYLE_W);
	content = _create_textblock(setup_layout, text[1], SUB_CONTENT_STYLE_W);

	elm_object_part_content_set(setup_layout, "progressbar_msg", title);

	progressbar = _create_progressbar(setup_layout, "pending");
	elm_object_part_content_set(setup_layout, "progressbar", progressbar);

	elm_object_part_content_set(setup_layout, "content_text", content);

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, setup_layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);

	/* set progressbar timer callback */
	ud.timer = ecore_timer_add(0.1, __progressbar_timer_cb, ad);

	return;
}

static char *__security_multiline_text_get(void *data, Evas_Object *obj, const char *part)
{
	char text[PATH_MAX] = "\0";

	if (!strcmp(part, "elm.text.multiline")) {
		snprintf(text, PATH_MAX, "Select a Krate unlock method and a timeout option.");
		return strdup(text);
	}

	return NULL;
}

static char *__security_group_text_get(void *data, Evas_Object *obj, const char *part)
{
	char *text[] = {"Unlock method", "Security options"};
	int index = (int)data;

	if (!strcmp(part, "elm.text"))
		return strdup(text[index]);

	return NULL;
}

static char *__security_double_label_text_get(void *data, Evas_Object *obj, const char *part)
{
	char text[PATH_MAX] = "\0";
	int timeout = 10; /*[TBD] get value of timeout */

	if (!strcmp(part, "elm.text"))
		snprintf(text, PATH_MAX, "Security Timeout");
	else if (!strcmp(part, "elm.text.sub"))
		snprintf(text, PATH_MAX, "After %d minuates of inactivity", timeout);

	return strdup(text);
}

static char *__security_radio_text_get(void *data, Evas_Object *obj, const char *part)
{
	char *radio_text[] = {"Password", "PIN"};
	int index = (int)data;

	if (!strcmp(part, "elm.text"))
		return strdup(radio_text[index]);

	return NULL;
}

static Evas_Object *__security_radio_content_get(void *data, Evas_Object *obj, const char *part)
{
	Evas_Object *radio;
	int index = (int)data;

	if (!strcmp(part, "elm.swallow.icon")) {
		radio = elm_radio_add(obj);
		elm_radio_state_value_set(radio, index);
		elm_radio_group_add(radio, ud.radio_main);

		if (index == ud.unlock_method)
			elm_radio_value_set(radio, ud.unlock_method);

		evas_object_propagate_events_set(radio, EINA_FALSE);
		evas_object_repeat_events_set(radio, EINA_TRUE);

		return radio;
	}

	return NULL;
}

static void __security_locktype_select_cb(void *data, Evas_Object *obj, void *event_info)
{
	int index = (int)data;

	elm_genlist_item_selected_set((Elm_Object_Item *)event_info, EINA_FALSE);
	elm_radio_value_set(ud.radio_main, index);
	ud.unlock_method = index;
}

static void __create_security_view(appdata_s *ad)
{
	Evas_Object *genlist;
	Elm_Object_Item *nf_it, *gl_item;
	Elm_Genlist_Item_Class *_itc;
	int index = UNLOCKMETHOD_SIMPLE_PASSWORD;
	Evas_Object *layout;

	elm_object_signal_emit(ud.conform, "elm,state,indicator,nooverlap", "elm");

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");

	genlist = elm_genlist_add(layout);
	elm_object_style_set(genlist, "solid/default");

	_itc = _create_genlist_item_class("multiline", __security_multiline_text_get, NULL);
	gl_item = _append_genlist_item(genlist, _itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, -1);
	_itc =  _create_genlist_item_class("group_index", __security_group_text_get, NULL);
	gl_item = _append_genlist_item(genlist, _itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, GROUPINDEX_UNLOCK_METHOD);

	ud.radio_main = elm_radio_add(genlist);
	elm_radio_state_value_set(ud.radio_main, 0);
	elm_radio_value_set(ud.radio_main, 0);

	_itc = _create_genlist_item_class("one_icon", __security_radio_text_get, __security_radio_content_get);
	for (index = 0; index < 2; index++) {
		gl_item = elm_genlist_item_append(genlist, _itc, (void *)index, NULL, ELM_GENLIST_ITEM_NONE, __security_locktype_select_cb, (void *)index);
		if (index == UNLOCKMETHOD_PASSWORD) {
			elm_object_item_disabled_set(gl_item, EINA_TRUE); /* [TBD] enable simple password */
		}
	}

	/* Timeout list group*/
	_itc = _create_genlist_item_class("group_index", __security_group_text_get, NULL);
	gl_item = _append_genlist_item(genlist, _itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, GROUPINDEX_SECURITY_OPTIONS);

	_itc = _create_genlist_item_class("double_label", __security_double_label_text_get, NULL);
	gl_item = _append_genlist_item(genlist, _itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, -1);
	elm_object_item_disabled_set(gl_item, EINA_TRUE); /* [TBD] enable timeout options */

	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);
	elm_layout_content_set(layout, "content_layout", genlist);

	__set_one_btn_bottom_layout(layout, ad, "Done");

	nf_it = elm_naviframe_item_push(ud.nf, "Krate Security", NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_TRUE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, NULL);
	return;
}

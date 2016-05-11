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
#include "zone-setup.h"
#include "widget.h"

#define WELCOME_HEADER "Separated Zone"
#define WELCOME_INFO_MESSAGE "Welcome<br>Use your applications separately<br>with SZ.<br>The folder will be created on your<br>personal home screen."
#define SETUP_HEADER "Separated Zone"
#define SETUP_PROGRESS_MESSAGE "Creating Folder instance..."
#define SETUP_INFO_MESSAGE "The folder will be created on your<br>presonal home screen."

typedef struct {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *nf;
	Evas_Object *popup;
	char *edj_path;
	view_num current_view;
} uidata_s;

uidata_s ud = {0, };

static Eina_Bool __naviframe_pop_cb(void *data, Elm_Object_Item *it)
{
	switch (ud.current_view) {
	case WELCOME_VIEW:
		ui_app_exit();
		return EINA_FALSE;
	case SETUP_COMPLETE_VIEW:
		ud.current_view = WELCOME_VIEW;
		elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");
		return EINA_TRUE;
	default:
		return EINA_FALSE;
	}
}

static void __prev_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	switch (ud.current_view) {
	case WELCOME_VIEW:
		ui_app_exit();
		break;
	default:
		break;
	}
	return ;
}

static void __next_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;

	switch (ud.current_view) {
	case WELCOME_VIEW:
		ud.current_view = SETUP_COMPLETE_VIEW;
		if (_send_zone_provision_data(ad->zone_name, ad->provision_path) != 0)
			ui_app_exit();
		_create_setup_view(ad);
		break;
	case SETUP_COMPLETE_VIEW:
		ui_app_exit();
		break;
	default:
		break;
	}
	return ;
}

/*static void __set_one_btn_bottom_layout(Evas_Object *layout, appdata_s *ad, const char *btn_text)
{
	Evas_Object *bottom_layout;
	Evas_Object *btn;

	bottom_layout = _create_layout(layout, ud.edj_path, "one_button_layout");

	btn = _create_button(bottom_layout, btn_text, "bottom");
	elm_object_part_content_set(bottom_layout, "button", btn);
	evas_object_smart_callback_add(btn, "clicked", __next_btn_cb, ad);

	elm_object_part_content_set(layout, "bottom_layout", bottom_layout);
	return ;
}*/

static void __set_two_btn_bottom_layout(Evas_Object *layout, appdata_s *ad, const char *prev_btn_text, const char *next_btn_text)
{
	Evas_Object *bottom_layout;
	Evas_Object *prev_btn, *next_btn;

	bottom_layout = _create_layout(layout, ud.edj_path, "two_button_layout");

	prev_btn = _create_button(bottom_layout, prev_btn_text, "bottom");
	elm_object_part_content_set(bottom_layout, "prev_button", prev_btn);
	evas_object_smart_callback_add(prev_btn, "clicked", __prev_btn_cb, NULL);

	next_btn = _create_button(bottom_layout, next_btn_text, "bottom");
	elm_object_part_content_set(bottom_layout, "next_button", next_btn);
	evas_object_smart_callback_add(next_btn, "clicked", __next_btn_cb, ad);

	elm_object_part_content_set(layout, "bottom_layout", bottom_layout);
	return ;
}

/*static void __popup_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;

	Evas_Object *timer = evas_object_data_get(obj, "timer");
	ecore_timer_del(timer);

	eext_object_event_callback_add(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	_create_setup_complete_view(ad);
	return ;
}*/

/*static Eina_Bool __progressbar_timer_cb(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	if (ad->create_done) {
		evas_object_data_del(ud.popup, "timer");
		evas_object_del(ud.popup);

		return ECORE_CALLBACK_CANCEL;
	}

	return ECORE_CALLBACK_RENEW;
}*/

void _create_base_window(appdata_s *ad)
{
	Evas_Object *layout;

	char edj_path[PATH_MAX] = "\0";
	char *res_path = NULL;

	/* Initialize data */
	ud.current_view = WELCOME_VIEW;
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

	_create_welcome_view(ad);

	elm_object_part_content_set(layout, "elm.swallow.content", ud.nf);
	eext_object_event_callback_add(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);

	evas_object_show(ud.win);
	return ;
}

void _create_welcome_view(appdata_s *ad)
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

	//__set_one_btn_bottom_layout(layout, ad, "Set up");
	__set_two_btn_bottom_layout(layout, ad, "Cancel", "Set up");

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, NULL);

	return ;
}

void _create_setup_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Evas_Object *layout, *setup_layout;
	Evas_Object *progressbar, *text;
	Evas_Textblock_Style *text_st;

	eext_object_event_callback_del(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb);

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");
	setup_layout = _create_layout(layout, ud.edj_path, "setup_layout");

	progressbar = _create_progressbar(setup_layout, NULL);
	elm_object_part_content_set(setup_layout, "progressbar", progressbar);

	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, SETUP_TEXT_STYLE);
	text = _create_textblock(setup_layout, SETUP_INFO_MESSAGE, text_st);
	elm_object_part_content_set(setup_layout, "content_text", text);
	evas_textblock_style_free(text_st);

	elm_object_part_content_set(layout, "content_layout", setup_layout);

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, NULL);

	return ;
}

/*
void _create_zone_popup(appdata_s *ad)
{
	Evas_Object *layout;
	Evas_Object *popup;
	Evas_Object *progressbar, *timer;

	eext_object_event_callback_del(ud.nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb);
	popup = elm_popup_add(ud.win);

	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_text_set(popup, "title,text", SETUP_TITLE);

	layout = _create_layout(popup, ud.edj_path, "create_popup_layout");
	elm_object_part_text_set(layout, "text", POPUP_CONTENT_TEXT);

	progressbar = _create_progressbar(layout, "process_large");
	elm_object_part_content_set(layout, "processing", progressbar);
	elm_object_part_content_set(popup, "elm.swallow.content", layout);

	evas_object_show(popup);
	ud.popup = popup;

	// set popup timer callback
	timer = ecore_timer_add(0.1, __progressbar_timer_cb, ad);
	evas_object_data_set(popup, "timer", timer);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, __popup_del_cb, ad);

	return ;
}*/

/*void _create_setup_complete_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Evas_Object *layout, *complete_layout;

	elm_object_signal_emit(ud.conform, "elm,state,indicator,nooverlap", "elm");

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");

	complete_layout = _create_layout(layout, ud.edj_path, "setup_complete_layout");
	elm_object_part_text_set(complete_layout, "sub_title_text", SETUP_TITLE);
	elm_object_part_text_set(complete_layout, "content_text", COMPLETE_SUB_TEXT);
	elm_object_part_content_set(layout, "content_layout", complete_layout);

	__set_one_btn_bottom_layout(layout, ad, "Done");

	nf_it = elm_naviframe_item_push(ud.nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __naviframe_pop_cb, NULL);

	return ;
}*/

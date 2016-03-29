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

#define WELCOME_TEXT "Welcome<br>Keep your applications and content safe<br>with ZONE."
#define POPUP_CONTENT "Creating Zone..."
#define SETUP_TITLE "Tizen ZONE"
#define SETUP_SUB_TEXT "The application shortcut will be created<br>on your personal home screen."

static Eina_Bool naviframe_pop_cb(void *data, Elm_Object_Item *it)
{
	appdata_s *ad = (appdata_s *) data;

	switch (ad->vnum) {
	case WELCOME_WIN:
		ui_app_exit();
		return EINA_FALSE;
	case SETUP_WIN:
		ad->vnum = WELCOME_WIN;
		elm_object_signal_emit(ad->main_conform, "elm,state,indicator,overlap", "elm");
		return EINA_TRUE;
	default:
		return EINA_FALSE;
	}
}

static void next_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;

	switch (ad->vnum) {
	case WELCOME_WIN:
		ad->vnum = SETUP_WIN;
		if (create_zone_manifest() != 0)
			ui_app_exit();
		create_zone_popup(ad);
		break;
	case SETUP_WIN:
		ui_app_exit();
		break;
	default:
		break;
	}
	return ;
}

static void set_one_btn_bottom_layout(Evas_Object *layout, appdata_s *ad, const char *btn_text)
{
	Evas_Object *bottom_layout;
	Evas_Object *btn;

	bottom_layout = setup_wizard_create_layout(layout, ad->edj_path, "one_button_layout");

	btn = setup_wizard_create_button(bottom_layout, btn_text, "bottom");
	elm_object_part_content_set(bottom_layout, "button", btn);
	evas_object_smart_callback_add(btn, "clicked", next_btn_cb, ad);

	elm_object_part_content_set(layout, "bottom_layout", bottom_layout);
	return ;
}

static void popup_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;

	Evas_Object *timer = evas_object_data_get(obj, "timer");
	ecore_timer_del(timer);

	eext_object_event_callback_add(ad->nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	create_setup_zone_view(ad);
	return ;
}

static Eina_Bool progressbar_timer_cb(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	if (ad->create_done) {
		evas_object_data_del(ad->popup, "timer");
		evas_object_del(ad->popup);

		return ECORE_CALLBACK_CANCEL;
	}

	return ECORE_CALLBACK_RENEW;
}

void create_welcome_view(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	Elm_Object_Item *nf_it;
	Evas_Object *layout, *welcome_layout;
	Evas_Object *text;
	Evas_Textblock_Style *text_st;

	elm_object_signal_emit(ad->main_conform, "elm,state,indicator,overlap", "elm");

	layout = setup_wizard_create_layout(ad->nf, ad->edj_path, "base_layout");
	welcome_layout = setup_wizard_create_layout(layout, ad->edj_path, "welcome_layout");

	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, SETUP_TEXT_STYLE);
	text = setup_wizard_create_textblock(welcome_layout, WELCOME_TEXT, text_st);
	elm_object_part_content_set(welcome_layout, "content_text", text);
	evas_textblock_style_free(text_st);

	elm_object_part_content_set(layout, "content_layout", welcome_layout);

	set_one_btn_bottom_layout(layout, ad, "Set up");

	nf_it = elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, naviframe_pop_cb, ad->main_win);

	return ;
}

void create_zone_popup(void *data)
{
	appdata_s *ad = (appdata_s *)data;

	Evas_Object *layout;
	Evas_Object *popup;
	Evas_Object *progressbar, *timer;

	eext_object_event_callback_del(ad->nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb);
	popup = elm_popup_add(ad->main_win);

	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_part_text_set(popup, "title,text", SETUP_TITLE);

	layout = setup_wizard_create_layout(popup, ad->edj_path, "create_popup_layout");
	elm_object_part_text_set(layout, "text", POPUP_CONTENT);

	progressbar = setup_wizard_create_progressbar(layout, "process_large");
	elm_object_part_content_set(layout, "processing", progressbar);
	elm_object_part_content_set(popup, "elm.swallow.content", layout);

	evas_object_show(popup);
	ad->popup = popup;

	/* set popup timer callback*/
	timer = ecore_timer_add(0.1, progressbar_timer_cb, ad);
	evas_object_data_set(popup, "timer", timer);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, popup_del_cb, ad);

	return ;
}

void create_setup_zone_view(void *data)
{
	appdata_s *ad = (appdata_s *) data;
	Elm_Object_Item *nf_it;
	Evas_Object *layout, *setup_layout;

	elm_object_signal_emit(ad->main_conform, "elm,state,indicator,nooverlap", "elm");

	layout = setup_wizard_create_layout(ad->nf, ad->edj_path, "base_layout");

	setup_layout = setup_wizard_create_layout(layout, ad->edj_path, "setup_layout");
	elm_object_part_text_set(setup_layout, "sub_title_text", SETUP_TITLE);
	elm_object_part_text_set(setup_layout, "content_text", SETUP_SUB_TEXT);
	elm_object_part_content_set(layout, "content_layout", setup_layout);

	set_one_btn_bottom_layout(layout, ad, "Done");

	nf_it = elm_naviframe_item_push(ad->nf, NULL, NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_FALSE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, naviframe_pop_cb, ad);

	return ;
}

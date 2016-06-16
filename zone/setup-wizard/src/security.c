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

#define GROUPINDEX_UNLOCK_METHOD 0
#define GROUPINDEX_SECURITY_OPTIONS 1

#define UNLOCKMETHOD_PASSWORD 0
#define UNLOCKMETHOD_SIMPLE_PASSWORD 1

extern uidata_s ud;

typedef struct {
	Evas_Object *entry;
	Evas_Object *right_button;
	Evas_Object *radio_main;
	int unlock_method;
} security_info_s;

security_info_s info = {0, };

static void __create_password_set_view(appdata_s *ad);

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
		elm_radio_group_add(radio, info.radio_main);

		if (index == info.unlock_method)
			elm_radio_value_set(radio, info.unlock_method);

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
	elm_radio_value_set(info.radio_main, index);
	info.unlock_method = index;
}

static Eina_Bool __security_pop_cb(void *data, Elm_Object_Item *it)
{
	elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");

	return EINA_TRUE;
}

static void __security_prev_cb(void *data, Evas_Object *obj, void *event_info)
{
	elm_naviframe_item_pop(ud.nf);
}

static void __security_next_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;
	__create_password_set_view(ad);
}

static void __security_setup_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = (appdata_s *)data;
	/* [TBD] send password to authfw :: const char *pwd = elm_object_text_get(info.entry); */

	if (_send_zone_create_request(ad) != 0)
		ui_app_exit();

	elm_object_signal_emit(ud.conform, "elm,state,indicator,overlap", "elm");
	_create_setup_view(ad);
}

static void __entry_changed_cb(void *data, Evas_Object *obj, void *event_info)
{
	if (elm_entry_is_empty(info.entry) == EINA_TRUE)
		elm_object_disabled_set(info.right_button, EINA_TRUE);
	else
		elm_object_disabled_set(info.right_button, EINA_FALSE);
	return;
}

static Evas_Object *__security_entry_content_get(void *data, Evas_Object *obj, const char *part)
{
	if (!strcmp(part, "elm.swallow.content")) {
		Evas_Object *layout;

		layout =  _create_layout(obj, ud.edj_path, "security_layout");
		info.entry = _create_entry(obj);
		elm_object_part_content_set(layout, "entry", info.entry);
		evas_object_smart_callback_add(info.entry, "changed", __entry_changed_cb, NULL);

		return layout;
	}
	return NULL;
}

static void __create_password_set_view(appdata_s *ad)
{
	Elm_Object_Item *nf_it;
	Elm_Genlist_Item_Class *_itc;
	Evas_Object *layout;
	Evas_Object *left_button, *right_button;

	Evas_Object *genlist;

	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");

	genlist = elm_genlist_add(layout);
	elm_object_style_set(genlist, "solid/default");

	_itc = _create_genlist_item_class("full", NULL, __security_entry_content_get);
	_append_genlist_item(genlist, _itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, -1);

	elm_genlist_mode_set(genlist, ELM_LIST_COMPRESS);
	elm_layout_content_set(layout, "content_layout", genlist);

	left_button = _create_button(layout, PREV_BUTTON, __security_prev_cb, NULL);
	right_button = _create_button(layout, SETUP_BUTTON, __security_setup_cb, ad);

	info.right_button = right_button;
	elm_object_disabled_set(info.right_button, EINA_TRUE);

	_create_two_button_layout(layout, left_button, right_button);

	nf_it = elm_naviframe_item_push(ud.nf, "Password Setting", NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_TRUE, EINA_TRUE);
	return;
}

void _create_security_view(appdata_s *ad)
{
	int index = UNLOCKMETHOD_SIMPLE_PASSWORD;
	Evas_Object *genlist;
	Elm_Object_Item *nf_it, *gl_item;
	Elm_Genlist_Item_Class *_itc;
	Evas_Object *layout;
	Evas_Object *left_button, *right_button;

	elm_object_signal_emit(ud.conform, "elm,state,indicator,nooverlap", "elm");
	layout = _create_layout(ud.nf, ud.edj_path, "base_layout");

	genlist = elm_genlist_add(layout);
	elm_object_style_set(genlist, "solid/default");

	_itc = _create_genlist_item_class("multiline", __security_multiline_text_get, NULL);
	gl_item = _append_genlist_item(genlist, _itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, -1);
	_itc =  _create_genlist_item_class("group_index", __security_group_text_get, NULL);
	gl_item = _append_genlist_item(genlist, _itc, ELM_OBJECT_SELECT_MODE_DISPLAY_ONLY, GROUPINDEX_UNLOCK_METHOD);

	info.radio_main = elm_radio_add(genlist);
	elm_radio_state_value_set(info.radio_main, 0);
	elm_radio_value_set(info.radio_main, 0);

	_itc = _create_genlist_item_class("one_icon", __security_radio_text_get, __security_radio_content_get);
	for (index = 0; index < 2; index++) {
		gl_item = elm_genlist_item_append(genlist, _itc, (void *)index, NULL, ELM_GENLIST_ITEM_NONE, __security_locktype_select_cb, (void *)index);
		if (index == UNLOCKMETHOD_SIMPLE_PASSWORD) {
			/* [TBD] enable simple password */
			elm_object_item_disabled_set(gl_item, EINA_TRUE);
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

	left_button = _create_button(layout, PREV_BUTTON, __security_prev_cb, NULL);
	right_button = _create_button(layout, NEXT_BUTTON, __security_next_cb, ad);

	_create_two_button_layout(layout, left_button, right_button);

	nf_it = elm_naviframe_item_push(ud.nf, "Krate Security", NULL, NULL, layout, NULL);
	elm_naviframe_item_title_enabled_set(nf_it, EINA_TRUE, EINA_TRUE);
	elm_naviframe_item_pop_cb_set(nf_it, __security_pop_cb, NULL);
	return;
}

/*
 * Tizen DPM Syspopup
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

#include "dpm-syspopup.h"

static void __win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static Evas_Object *__create_win(const char *pkg_name)
{
	Evas_Object *win = elm_win_add(NULL, pkg_name, ELM_WIN_DIALOG_BASIC);

	elm_win_title_set(win, pkg_name);
	elm_win_borderless_set(win, EINA_TRUE);
	elm_win_alpha_set(win, EINA_TRUE);

	evas_object_smart_callback_add(win, "delete,request", __win_delete_request_cb, NULL);
	evas_object_show(win);

	return win;
}

static void __popup_del_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	Evas_Object *win = (Evas_Object *)data;
	evas_object_del(win);
	ui_app_exit();
}

static void __popup_timeout_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(obj);
	return;
}

static void __block_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
	evas_object_del(obj);
	return;
}

void _create_syspopup(const char *style, const char *id)
{
	Evas_Object *win = NULL;
	Evas_Object *popup = NULL;
	popup_info_s *info = NULL;

	char *lp_policy = NULL;
	char *lp_header = NULL;
	char *lp_body = NULL;

	info = _get_dpm_popup_info(id);
	if (info == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get popup info");
		return;
	}
	lp_policy = dgettext("dpm-syspopup", info->text);

	win = __create_win("dpm-syspopup");

	popup = elm_popup_add(win);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_style_set(popup, style);

	char text[PATH_MAX] = "\0";
	if (!strcmp(style, "default")) {
		lp_header = dgettext("dpm-syspopup", "IDS_DPM_HEADER_PREVENT_TEXT");
		lp_body = dgettext("dpm-syspopup", "IDS_DPM_BODY_PREVENT_TEXT");

		snprintf(text, PATH_MAX, lp_header, lp_policy);
		elm_object_part_text_set(popup, "title,text", text);
		elm_object_item_part_text_translatable_set(popup, "title,text", EINA_TRUE);

		snprintf(text, PATH_MAX, lp_body, lp_policy);
		elm_object_text_set(popup, text);
		elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	} else {
		lp_body = dgettext("dpm-syspopup", "IDS_DPM_TOAST_PREVENT_TEXT");
		snprintf(text, PATH_MAX, lp_body, lp_policy);
		elm_object_text_set(popup, text);
	}

	elm_popup_timeout_set(popup, 3.0);
	evas_object_smart_callback_add(popup, "block,clicked", __block_clicked_cb, NULL);
	evas_object_smart_callback_add(popup, "timeout", __popup_timeout_cb, NULL);

	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, win);
	evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, __popup_del_cb, win);
	evas_object_show(popup);

	return;
}

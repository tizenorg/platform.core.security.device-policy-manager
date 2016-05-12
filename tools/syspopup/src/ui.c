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

void _create_syspopup(const char *id, char *style, const char *status, const char *user_data)
{
	Evas_Object *win = NULL;
	Evas_Object *popup = NULL;
	Evas_Object *left_btn = NULL, *right_btn = NULL;

	popup_info_s *info = NULL;
	int ret = 0;
	char header[PATH_MAX] = "\0";
	char body[PATH_MAX] = "\0";

	info = _get_popup_info(id);
	if (info == NULL)
		return;

	ret = _get_popup_text(id, status, header, body);
	if (ret != 0) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get popup text");
		return;
	}

	win = __create_win("dpm-syspopup");
	popup = elm_popup_add(win);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	if (style != NULL)
		info->style = style;

	elm_object_style_set(popup, info->style);

	if (!strcmp(info->style, "default")) {
		elm_object_part_text_set(popup, "title,text", header);
		elm_object_item_part_text_translatable_set(popup, "title,text", EINA_TRUE);

		elm_object_text_set(popup, body);
		elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);

		if (info->left_btn != NULL) {
			left_btn = elm_button_add(popup);
			elm_object_style_set(left_btn, "popup");
			elm_object_text_set(left_btn, __(info->left_btn));
			elm_object_part_content_set(popup, "button1", left_btn);
			evas_object_data_set(left_btn, "target", popup);
			evas_object_data_set(popup, "target", popup);
			evas_object_smart_callback_add(left_btn, "clicked", info->left_btn_cb, user_data);
			eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, info->left_btn_cb, (void *)user_data);
		} else {
			eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, win);
		}

		if (info->right_btn != NULL) {
			right_btn = elm_button_add(popup);
			elm_object_style_set(right_btn, "popup");
			elm_object_text_set(right_btn, __(info->right_btn));
			elm_object_part_content_set(popup, "button2", right_btn);
			evas_object_data_set(right_btn, "popup", popup);
			evas_object_smart_callback_add(right_btn, "clicked", info->right_btn_cb, user_data);
		}
	} else {
		elm_object_text_set(popup, body);
		elm_popup_timeout_set(popup, 3.0);
		evas_object_smart_callback_add(popup, "timeout", __popup_timeout_cb, NULL);
		evas_object_smart_callback_add(popup, "block,clicked", __block_clicked_cb, NULL);
		eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, eext_popup_back_cb, win);
	}

	evas_object_event_callback_add(popup, EVAS_CALLBACK_DEL, __popup_del_cb, win);
	evas_object_show(popup);

	return;
}

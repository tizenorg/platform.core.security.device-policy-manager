/*
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

#include "ode-app.h"

static void _confirm_btn_clicked_cb(void* data, Evas_Object* obj, void* event_info)
{
	appdata_s* ad = (appdata_s*) data;
	dlog_print(DLOG_DEBUG, LOG_TAG, "_confirm_btn_clicked_cb start");

	switch (ad->view_type) {
	case ENCRYPT_DEVICE:
		dlog_print(DLOG_DEBUG, LOG_TAG, "encrypt device confirm");
		break;
	case DECRYPT_DEVICE:
		dlog_print(DLOG_DEBUG, LOG_TAG, "decrypt device confirm");
		break;
	case ENCRYPT_SD_CARD:
		dlog_print(DLOG_DEBUG, LOG_TAG, "encrypt sd card confrim");
		break;
	case DECRYPT_SD_CARD:
		dlog_print(DLOG_DEBUG, LOG_TAG, "decrypt sd card confrim");
		break;
	}
}

static void set_confirm_next_btn_layout(Evas_Object* layout, appdata_s* ad, const char* btn_text)
{
	Evas_Object* btn_layout;
	Evas_Object* next_btn;

	btn_layout = dpm_encryption_create_layout(layout, ad->edj_path, "next_button_layout");

	next_btn = dpm_encryption_create_button(btn_layout, btn_text, "bottom");
	elm_object_part_content_set(btn_layout, "next_button", next_btn);

	evas_object_smart_callback_add(next_btn, "clicked", _confirm_btn_clicked_cb, ad);

	elm_object_part_content_set(layout, "bottom_layout", btn_layout);
	return;
}

void dpm_encryption_device_confirm_create_view(appdata_s* ad)
{
	Evas_Object* naviframe = ad->navi_bar;
	Evas_Object* base_layout = NULL;
	Evas_Object* confirm_msg1, *confirm_msg2 = NULL;
	Evas_Object* confirm_layout = NULL;
	Evas_Textblock_Style* text_st;

	dlog_print(DLOG_DEBUG, LOG_TAG, "start dpm_encryption_device_confirm_create_view");

	if (naviframe == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "naviframe is null");
		return;
	}

	base_layout = dpm_encryption_create_layout(naviframe, ad->edj_path, "base_layout");
	confirm_layout = dpm_encryption_create_layout(base_layout, ad->edj_path, "confirm_layout");
	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, MSG_TEXT_STYLE);

	confirm_msg1 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_ENCRYPT_DEVICE_MSG_1, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_1", confirm_msg1);
	confirm_msg2 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_ENCRYPT_DEVICE_MSG_2, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_2", confirm_msg2);
	evas_textblock_style_free(text_st);

	elm_object_part_content_set(base_layout, "content_layout", confirm_layout);
	set_confirm_next_btn_layout(base_layout, ad, IDS_ST_BUTTON_ENCRYPT_DEVICE);

	elm_naviframe_item_push(naviframe, IDS_ST_HEADER_CONFIRM_ENCRYPTION, NULL, NULL, base_layout, NULL);
}

void dpm_decryption_device_confirm_create_view(appdata_s* ad)
{
	Evas_Object* naviframe = ad->navi_bar;
	Evas_Object* base_layout = NULL;
	Evas_Object* confirm_msg1, *confirm_msg2 = NULL;
	Evas_Object* confirm_layout = NULL;
	Evas_Textblock_Style* text_st;

	dlog_print(DLOG_DEBUG, LOG_TAG, "start dpm_encryption_device_confirm_create_view");

	if (naviframe == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "naviframe is null");
		return;
	}

	base_layout = dpm_encryption_create_layout(naviframe, ad->edj_path, "base_layout");
	confirm_layout = dpm_encryption_create_layout(base_layout, ad->edj_path, "confirm_layout");

	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, MSG_TEXT_STYLE);

	confirm_msg1 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_DECRYPT_DEVICE_MSG_1, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_1", confirm_msg1);
	confirm_msg2 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_DECRYPT_DEVICE_MSG_2, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_2", confirm_msg2);

	evas_textblock_style_free(text_st);

	elm_object_part_content_set(base_layout, "content_layout", confirm_layout);

	set_confirm_next_btn_layout(base_layout, ad, IDS_ST_BUTTON_DECRYPT_DEVICE);

	elm_naviframe_item_push(naviframe, IDS_ST_HEADER_CONFIRM_DECRYPTION, NULL, NULL, base_layout, NULL);
}

void dpm_encryption_sd_card_confirm_create_view(appdata_s* ad)
{
	Evas_Object* naviframe = ad->navi_bar;
	Evas_Object* base_layout = NULL;
	Evas_Object* confirm_msg1, *confirm_msg2 = NULL;
	Evas_Object* confirm_layout = NULL;
	Evas_Textblock_Style* text_st;

	dlog_print(DLOG_DEBUG, LOG_TAG, "start dpm_encryption_device_confirm_create_view");

	if (naviframe == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "naviframe is null");
		return;
	}

	base_layout = dpm_encryption_create_layout(naviframe, ad->edj_path, "base_layout");
	confirm_layout = dpm_encryption_create_layout(base_layout, ad->edj_path, "confirm_layout");

	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, MSG_TEXT_STYLE);

	confirm_msg1 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_ENCRYPT_SD_CARD_MSG_1, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_1", confirm_msg1);
	confirm_msg2 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_ENCRYPT_SD_CARD_MSG_2, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_2", confirm_msg2);

	evas_textblock_style_free(text_st);

	elm_object_part_content_set(base_layout, "content_layout", confirm_layout);

	set_confirm_next_btn_layout(base_layout, ad, IDS_ST_BUTTON_ENCRYPT_SD_CARD_ABB);

	elm_naviframe_item_push(naviframe, IDS_ST_HEADER_CONFIRM_ENCRYPTION, NULL, NULL, base_layout, NULL);
}

void dpm_decryption_sd_card_confirm_create_view(appdata_s* ad)
{
	Evas_Object* naviframe = ad->navi_bar;
	Evas_Object* base_layout = NULL;
	Evas_Object* confirm_msg1, *confirm_msg2 = NULL;
	Evas_Object* confirm_layout = NULL;
	Evas_Textblock_Style* text_st;

	dlog_print(DLOG_DEBUG, LOG_TAG, "start dpm_encryption_device_confirm_create_view");

	if (naviframe == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "naviframe is null");
		return;
	}

	base_layout = dpm_encryption_create_layout(naviframe, ad->edj_path, "base_layout");
	confirm_layout = dpm_encryption_create_layout(base_layout, ad->edj_path, "confirm_layout");

	text_st = evas_textblock_style_new();
	evas_textblock_style_set(text_st, MSG_TEXT_STYLE);

	confirm_msg1 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_DECRYPT_SD_CARD_MSG_1, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_1", confirm_msg1);
	confirm_msg2 = dpm_encryption_create_textblock(confirm_layout, IDS_ST_BODY_CONFIRM_DECRYPT_SD_CARD_MSG_2, text_st);
	elm_object_part_content_set(confirm_layout, "msg_content_2", confirm_msg2);

	evas_textblock_style_free(text_st);

	elm_object_part_content_set(base_layout, "content_layout", confirm_layout);

	set_confirm_next_btn_layout(base_layout, ad, IDS_ST_BUTTON_DECRYPT_SD_CARD_ABB3);

	elm_naviframe_item_push(naviframe, IDS_ST_HEADER_CONFIRM_DECRYPTION, NULL, NULL, base_layout, NULL);
}

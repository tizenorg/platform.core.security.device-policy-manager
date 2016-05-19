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
#include <dpm/password.h>
#include <notification.h>
#include <vconf.h>
#include <vconf-keys.h>

#include "dpm-syspopup.h"

#define ARRAY_SIZE(_array_) \
	(sizeof(_array_) / sizeof(_array_[0]))

static void __ok_btn_cb(void *data, Evas_Object *obj, void *event_info);
static void __cancel_btn_cb(void *data, Evas_Object *obj, void *event_info);

noti_info_s noti_list[] = {
	{NULL, NULL},
	{"IDS_DPM_NOTI_PASSWORD_ENFORCE_CHANGE", "IDS_DPM_NOTI_BODY_PASSWORD_ENFORCE_CHANGE"},
	{"IDS_DPM_NOTI_CREATE_ZONE", "IDS_DPM_NOTI_BODY_CREATE_ZONE"},
};

popup_info_s popup_list[] = {

	/* ID | TITLE_PREFIX | TITLE_TEXT | CONTENT_TEXT | POPUP_STYLE | LEFT_BUTTON | RIGHT_BUTTON | LEFT_BTN_CB | RIGHT_BTN_CB | NOTI_NUMBER */

	/* Application Policy */
	{"package-installation-mode",   false,  "IDS_DPM_PACKAGE_INSTALLATION",   NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"package-uninstallation-mode", false, "IDS_DPM_PACKAGE_UNINSTALLATION", NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE },

	/* Password Policy */
	{"password-expires",        true, "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_EXPIRES", "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"password-reset",          true, "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_RESET", "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"password-enforce-change", true, "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_ENFORCE_CHANGE", "default", "IDS_DPM_CANCEL", "IDS_DPM_OK", __cancel_btn_cb, __ok_btn_cb, DPM_NOTI_PASSWORD_ENFORCE_CHANGE},

	/* Restriction Policy */
	{"wifi",                    true, "IDS_DPM_WIFI",              NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"wifi-hotspot",            true, "IDS_DPM_WIFI_HOTSPOT",      NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"camera",                  true, "IDS_DPM_CAMERA",            NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"microphone",              true, "IDS_DPM_MICROPHONE",        NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"location",                true, "IDS_DPM_LOCATION",          NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"settings-changes",        true, "IDS_DPM_SETTINGS_CHANGES",  NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"bluetooth",               true, "IDS_DPM_BLUETOOTH",         NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"clipboard",               true, "IDS_DPM_CLIPBOARD",         NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"usb-debugging",           true, "IDS_DPM_USB_DEBUGGING",     NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"nfc",                     true, "IDS_DPM_NFC",               NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"message-sending",         true, "IDS_DPM_MESSAGE_SENDING",   NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"message-receiving",       true, "IDS_DPM_MESSAGE_RECEIVING", NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"browser",                 true, "IDS_DPM_BROWSER",           NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"screen-capture",          true, "IDS_DPM_SCREEN_CAPTURE",    NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},

	/* Storage Policy */
	{"external-storage",   true, "IDS_DPM_EXTERNAL_STORAGE",   NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"storage-decryption", true, "IDS_DPM_STORAGE_DECRYPTION", NULL, "toast", NULL, NULL, NULL, NULL, DPM_NOTI_NONE},
	{"wipe-data",          true, "IDS_DPM_WIPE_DATA",          NULL, "default", "IDS_DPM_CANCEL", "IDS_DPM_OK", NULL, NULL, DPM_NOTI_NONE},

	/* Zone Policy */
	{"zone-create", false, "IDS_DPM_ZONE_CREATE", "IDS_DPM_BODY_ZONE_CREATE", "default", "IDS_DPM_CANCEL", "IDS_DPM_OK", __cancel_btn_cb, __ok_btn_cb, DPM_NOTI_CREATE_ZONE},
};

static void __ok_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *popup = (Evas_Object *)evas_object_data_get(obj, "target");

	/* call application */
	app_control_h app_control = (app_control_h) data;

	app_control_set_launch_mode(app_control, APP_CONTROL_LAUNCH_MODE_GROUP);
	app_control_send_launch_request(app_control, NULL, NULL);
	app_control_destroy(app_control);

	evas_object_del(popup);
	return;
}

static void __cancel_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	app_control_h app_control = (app_control_h) data;
	Evas_Object *popup = (Evas_Object *)evas_object_data_get(obj, "target");
	noti_num_e noti_num = (noti_num_e)evas_object_data_get(obj, "noti-num");

	noti_info_s noti = noti_list[noti_num];

	/* create notification */
	notification_h noti_handle = NULL;
	noti_handle = notification_create(NOTIFICATION_TYPE_NOTI);

	notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_TITLE, __(noti.title), NULL, NOTIFICATION_VARIABLE_TYPE_NONE);
	notification_set_text(noti_handle, NOTIFICATION_TEXT_TYPE_CONTENT, __(noti.content), NULL, NOTIFICATION_VARIABLE_TYPE_NONE);

	notification_set_display_applist(noti_handle, NOTIFICATION_DISPLAY_APP_ALL);
	/* [TBD] set notification icon
	 * notification_set_image(noti_handle, NOTIFICATION_IMAGE_TYPE_ICON, DPM_SYSPOPUP_ICON_PATH); */

	notification_set_launch_option(noti_handle, NOTIFICATION_LAUNCH_OPTION_APP_CONTROL, app_control);
	notification_post(noti_handle);

	notification_free(noti_handle);
	app_control_destroy(app_control);
	evas_object_del(popup);
	return;
}


popup_info_s *_get_popup_info(const char *id)
{
	int i = 0;

	if (id == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "popup_name is NULL");
		return NULL;
	}

	for (i = 0; i < ARRAY_SIZE(popup_list); i++) {
		if (!strcmp(id, popup_list[i].id))
			return &popup_list[i];
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "[%s] popup is not exist", id);
	return NULL;
}

int _get_popup_text(const char *id, const char *status, char *header, char *body)
{
	popup_info_s *info = NULL;
	char *lp_policy = NULL;
	char *lp_header = NULL;
	char *lp_body = NULL;

	info = _get_popup_info(id);
	if (info == NULL)
		return -1;

	lp_policy = __(info->title);

	if (info->content == NULL) {
		if (!strcmp(status, "ongoing"))
			lp_body = __("IDS_DPM_BODY_ONGOING_PREVENT_TEXT");
		else
			lp_body = __("IDS_DPM_BODY_PREVENT_TEXT");

		snprintf(body, PATH_MAX, lp_body, lp_policy);
	} else {
		lp_body = __(info->content);
		snprintf(body, PATH_MAX, "%s", lp_body);
	}

	if (info->title_prefix) {
		lp_header = __("IDS_DPM_HEADER_PREVENT_TEXT");
		snprintf(header, PATH_MAX, lp_header, lp_policy);
	} else {
		snprintf(header, PATH_MAX, "%s", lp_policy);
	}

	return 0;
}

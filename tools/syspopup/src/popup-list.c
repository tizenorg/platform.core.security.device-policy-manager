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

#include "dpm-syspopup.h"

#define ARRAY_SIZE(_array_) \
	(sizeof(_array_) / sizeof(_array_[0]))

popup_info_s popup_list[] = {

	/* ID | TITLE_PREFIX | TITLE_TEXT | CONTENT_TEXT | POPUP_STYLE | LEFT_BUTTON | RIGHT_BUTTON | NOTI_TITLE | NOTI_CONTENT */

	/* Application Policy */
	{"package-installation-mode",   false,  "IDS_DPM_PACKAGE_INSTALLATION",   NULL, "toast", NULL, NULL, NULL, NULL},
	{"package-uninstallation-mode", false, "IDS_DPM_PACKAGE_UNINSTALLATION", NULL, "toast", NULL, NULL, NULL, NULL},

	/* Password Policy */
	{"password-expires",        true, "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_EXPIRES", "toast", NULL, NULL, NULL, NULL},
	{"password-reset",          true, "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_RESET", "toast", NULL, NULL, NULL, NULL},
	{"password-enforce-change", true, "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_ENFORCE_CHANGE", "default", NULL, "IDS_DPM_OK", NULL, NULL},

	/* Restriction Policy */
	{"wifi",                    true, "IDS_DPM_WIFI",              NULL, "toast", NULL, NULL, NULL, NULL},
	{"wifi-hotspot",            true, "IDS_DPM_WIFI_HOTSPOT",      NULL, "toast", NULL, NULL, NULL, NULL},
	{"camera",                  true, "IDS_DPM_CAMERA",            NULL, "toast", NULL, NULL, NULL, NULL},
	{"microphone",              true, "IDS_DPM_MICROPHONE",        NULL, "toast", NULL, NULL, NULL, NULL},
	{"location",                true, "IDS_DPM_LOCATION",          NULL, "toast", NULL, NULL, NULL, NULL},
	{"settings-changes",        true, "IDS_DPM_SETTINGS_CHANGES",  NULL, "toast", NULL, NULL, NULL, NULL},
	{"bluetooth",               true, "IDS_DPM_BLUETOOTH",         NULL, "toast", NULL, NULL, NULL, NULL},
	{"clipboard",               true, "IDS_DPM_CLIPBOARD",         NULL, "toast", NULL, NULL, NULL, NULL},
	{"usb-debugging",           true, "IDS_DPM_USB_DEBUGGING",     NULL, "toast", NULL, NULL, NULL, NULL},
	{"nfc",                     true, "IDS_DPM_NFC",               NULL, "toast", NULL, NULL, NULL, NULL},
	{"message-sending",         true, "IDS_DPM_MESSAGE_SENDING",   NULL, "toast", NULL, NULL, NULL, NULL},
	{"message-receiving",       true, "IDS_DPM_MESSAGE_RECEIVING", NULL, "toast", NULL, NULL, NULL, NULL},
	{"browser",                 true, "IDS_DPM_BROWSER",           NULL, "toast", NULL, NULL, NULL, NULL},
	{"screen-capture",          true, "IDS_DPM_SCREEN_CAPTURE",    NULL, "toast", NULL, NULL, NULL, NULL},

	/* Storage Policy */
	{"external-storage",   true, "IDS_DPM_EXTERNAL_STORAGE",   NULL, "toast", NULL, NULL, NULL, NULL},
	{"storage-decryption", true, "IDS_DPM_STORAGE_DECRYPTION", NULL, "toast", NULL, NULL, NULL, NULL},
	{"wipe-data",          true, "IDS_DPM_WIPE_DATA",          NULL, "default", "IDS_DPM_CANCEL", "IDS_DPM_OK", NULL, NULL},

	/* Zone Policy */
	{"zone-create", false, "IDS_DPM_ZONE_CREATE", "IDS_DPM_BODY_ZONE_CREATE", "default", "IDS_DPM_CANCEL", "IDS_DPM_OK", "IDS_DPM_NOTI_ZONE_CREATE", "IDS_DPM_NOTI_BODY_ZONE_CREATE"},
	{"zone-remove", false, "IDS_DPM_ZONE_REMOVE", "IDS_DPM_BODY_ZONE_REMOVE", "default", "IDS_DPM_CANCEL", "IDS_DPM_OK", "IDS_DPM_NOTI_ZONE_REMOVE", "IDS_DPM_NOTI_BODY_ZONE_REMOVE"},
};

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

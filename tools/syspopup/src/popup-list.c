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

#define ARRAY_SIZE(_array_) \
	(sizeof(_array_) / sizeof(_array_[0]))

popup_info_s popup_list[] = {
	/* Application Policy */
	{"package-installation-mode",   "IDS_DPM_PACKAGE_INSTALLATION",   NULL},
	{"package-uninstallation-mode", "IDS_DPM_PACKAGE_UNINSTALLATION", NULL},

	/* Password Policy */
	{"password-expires",        "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_EXPIRES"},
	{"password-reset",          "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_RESET"},
	{"password-enforce-change", "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_ENFORCE_CHANGE"},
	{"password-delete-pattern", "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_DELETE_PATTERN"},
	{"password-set-pattern",    "IDS_DPM_PASSWORD", "IDS_DPM_BODY_PASSWORD_SET_PATTERN"},

	/* Restriction Policy */
	{"wifi",                    "IDS_DPM_WIFI", NULL},
	{"wifi-hotspot",            "IDS_DPM_WIFI_HOTSPOT", NULL},
	{"camera",                  "IDS_DPM_CAMERA", NULL},
	{"microphone",              "IDS_DPM_MICROPHONE", NULL},
	{"location",                "IDS_DPM_LOCATION", NULL},
	{"settings-changes",        "IDS_DPM_SETTINGS_CHANGES", NULL},
	{"bluetooth",               "IDS_DPM_BLUETOOTH", NULL},
	{"clipboard",               "IDS_DPM_CLIPBOARD", NULL},
	{"usb-debugging",           "IDS_DPM_USB_DEBUGGING", NULL},
	{"nfc",                     "IDS_DPM_NFC", NULL},
	{"message-sending",         "IDS_DPM_MESSAGE_SENDING", NULL},
	{"message-receiving",       "IDS_DPM_MESSAGE_RECEIVING", NULL},
	{"browser",                 "IDS_DPM_BROWSER", NULL},
	{"screen-capture",          "IDS_DPM_SCREEN_CAPTURE", NULL},

	/* Storage Policy */
	{"external-storage",   "IDS_DPM_EXTERNAL_STORAGE", NULL},
	{"storage-decryption", "IDS_DPM_STORAGE_DECRYPTION", NULL},
	{"wipe-data",          "IDS_DPM_WIPE_DATA", NULL},
};

static popup_info_s *_get_syspopup_info(const char *id)
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

int _get_dpm_syspopup_text(const char *id, const char *status, char *header, char *body)
{
	popup_info_s *info = NULL;
	char *lp_policy = NULL;
	char *lp_header = NULL;
	char *lp_body = NULL;

	info = _get_syspopup_info(id);
	if (info == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get popup info");
		return -1;
	}

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

	lp_header = __("IDS_DPM_HEADER_PREVENT_TEXT");
	snprintf(header, PATH_MAX, lp_header, lp_policy);

	return 0;
}

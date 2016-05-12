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
	{"package-installation-mode",   "IDS_DPM_PACKAGE_INSTALLATION"},
	{"package-uninstallation-mode", "IDS_DPM_PACKAGE_UNINSTALLATION"},

	/* Password Policy */
//	{"password-expires",        "IDS_DPM_HEADER_PASSWORD_POLICY"},
//	{"password-reset",          "IDS_DPM_HEADER_PASSWORD_POLICY"},
//	{"password-enforce-change", "IDS_DPM_HEADER_PASSWORD_POLICY"},
//	{"password-delete-pattern", "IDS_DPM_HEADER_PASSWORD_POLICY"},
//	{"password-set-pattern",    "IDS_DPM_HEADER_PASSWORD_POLICY"},

	/* Restriction Policy */
	{"wifi",                    "IDS_DPM_WIFI"},
	{"wifi-hotspot",            "IDS_DPM_WIFI_HOTSPOT"},
	{"camera",                  "IDS_DPM_CAMERA"},
	{"microphone",              "IDS_DPM_MICROPHONE"},
	{"location",                "IDS_DPM_LOCATION"},
	{"settings-changes",        "IDS_DPM_SETTINGS_CHANGES"},
	{"bluetooth",               "IDS_DPM_BLUETOOTH"},
	{"clipboard",               "IDS_DPM_CLIPBOARD"},
	{"usb-debugging",           "IDS_DPM_USB_DEBUGGING"},
	{"nfc",                     "IDS_DPM_NFC"},
	{"message-sending",         "IDS_DPM_MESSAGE_SENDING"},
	{"message-receiving",       "IDS_DPM_MESSAGE_RECEIVING"},
	{"browser",                 "IDS_DPM_BROWSER"},
	{"screen-capture",          "IDS_DPM_SCREEN_CAPTURE"},

	/* Storage Policy */
	{"external-storage",   "IDS_DPM_EXTERNAL_STORAGE"},
	{"storage-decryption", "IDS_DPM_STORAGE_DECRYPTION"},
	{"wipe-data",          "IDS_DPM_WIPE_DATA"},
};

popup_info_s *_get_dpm_popup_info(const char *id)
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

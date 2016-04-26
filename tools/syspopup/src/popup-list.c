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

popup_info_s popup_list[DPM_SYSPOPUP_MAX] = {
	/* Application Policy */
	{"install_mode",   "IDS_DPM_HEADER_APPLICATION_POLICY", "IDS_DPM_BODY_DISALLOW_INSTALL_MODE"},
	{"uninstall_mode", "IDS_DPM_HEADER_APPLICATION_POLICY", "IDS_DPM_BODY_DISALLOW_UNINSTALL_MODE"},

	/* Password Policy */
	{"password_quality",        "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_QUALITY"},
	{"password_expired",        "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_EXPIRED"},
	{"password_reset",          "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_RESET"},
	{"password_enforce_change", "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_ENFORCE_CHANGE"},

	/* Wi-Fi Policy */
	{"wifi",    "IDS_DPM_HEADER_WIFI_POLICY", "IDS_DPM_BODY_DISALLOW_WIFI"},
	{"hotspot", "IDS_DPM_HEADER_WIFI_POLICY", "IDS_DPM_BODY_DISALLOW_HOTSPOT"},

	/* Restriction Policy */
	{"camera",           "IDS_DPM_HEADER_CAMERA_POLICY",   "IDS_DPM_BODY_DISALLOW_CAMERA"},
	{"microphone",       "IDS_DPM_HEADER_RECORD_POLICY",   "IDS_DPM_BODY_DISALLOW_MICROPHONE"},
	{"location",         "IDS_DPM_HEADER_LOCATION_POLICY", "IDS_DPM_BODY_DISALLOW_LOCATION"},
	{"settings_changes", "IDS_DPM_HEADER_SETTING_POLICY",  "IDS_DPM_BODY_DISALLOW_SETTINGS_CHANGES"},

	/* Storage Policy */
	{"external_storage",   "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_DISALLOW_EXTERNAL_STORAGE"},
	{"storage_encryption", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_DISALLOW_STORAGE_ENCRYPTION"},
	{"storage_decryption", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_DISALLOW_STORAGE_DECRYPTION"},
	{"wipe_data",          "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_WIPE_STORAGE_DATA"},

	/* Bluetooth Policy */
	{"bluetooth", "IDS_DPM_HEADER_DISALLOW_BLUETOOTH", "IDS_DPM_BODY_DISALLOW_BLUETOOTH"},

	/* Container Policy */
	{"create_zone", "IDS_DPM_HEADER_CONTAINER_POLICY", "IDS_DPM_BODY_CREATE_CONTAINER"},
	{"remove_zone", "IDS_DPM_HEADER_CONTAINER_POLICY", "IDS_DPM_BODY_REMOVE_CONTAINER"},
};

popup_info_s *_get_dpm_popup_info(const char *popup_name)
{
	int i = 0;

	if (popup_name == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "popup_name is NULL");
		return NULL;
	}

	for (i = 0; i < DPM_SYSPOPUP_MAX; i++) {
		if (!strcmp(popup_name, popup_list[i].name))
			return &popup_list[i];
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "[%s] popup is not exist", popup_name);
	return NULL;
}

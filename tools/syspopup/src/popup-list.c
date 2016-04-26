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
	{"application-installation-mode",   "IDS_DPM_HEADER_APPLICATION_POLICY", "IDS_DPM_BODY_PREVENT_APPLICATION_INSTALLATION_MODE"},
	{"application-uninstallation-mode", "IDS_DPM_HEADER_APPLICATION_POLICY", "IDS_DPM_BODY_PREVENT_APPLICATION_UNINSTALLATION_MODE"},

	/* Password Policy */
	{"password-expires",        "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_EXPIRES"},
	{"password-reset",          "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_RESET"},
	{"password-enforce-change", "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_ENFORCE_CHANGE"},
	{"password-delete-pattern", "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_DELETE_PATTERN"},
	{"password-set-pattern",    "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_SET_PATTERN"},

	/* Wi-Fi Policy */
	{"wifi",         "IDS_DPM_HEADER_WIFI_POLICY", "IDS_DPM_BODY_PREVENT_WIFI"},
	{"wifi-hotspot", "IDS_DPM_HEADER_WIFI_POLICY", "IDS_DPM_BODY_PREVENT_HOTSPOT"},

	/* Restriction Policy */
	{"restriction-camera",           "IDS_DPM_HEADER_CAMERA_POLICY",   "IDS_DPM_BODY_PREVENT_CAMERA"},
	{"restriction-microphone",       "IDS_DPM_HEADER_RECORD_POLICY",   "IDS_DPM_BODY_PREVENT_MICROPHONE"},
	{"restriction-location",         "IDS_DPM_HEADER_LOCATION_POLICY", "IDS_DPM_BODY_PREVENT_LOCATION"},
	{"restriction-settings-changes", "IDS_DPM_HEADER_SETTING_POLICY",  "IDS_DPM_BODY_PREVENT_SETTINGS_CHANGES"},

	/* Storage Policy */
	{"storage-external",   "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_PREVENT_EXTERNAL_STORAGE"},
	{"storage-encryption", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_PREVENT_STORAGE_ENCRYPTION"},
	{"storage-decryption", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_PREVENT_STORAGE_DECRYPTION"},
	{"storage-wipe-data",  "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_WIPE_STORAGE_DATA"},

	/* Bluetooth Policy */
	{"bluetooth", "IDS_DPM_HEADER_PREVENT_BLUETOOTH", "IDS_DPM_BODY_PREVENT_BLUETOOTH"},

	/* Container Policy */
	{"zone-create", "IDS_DPM_HEADER_CONTAINER_POLICY", "IDS_DPM_BODY_CREATE_CONTAINER"},
	{"zone-remove", "IDS_DPM_HEADER_CONTAINER_POLICY", "IDS_DPM_BODY_REMOVE_CONTAINER"},
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

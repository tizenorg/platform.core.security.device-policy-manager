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

static void __terminate_cb (void *user_data)
{
	return;
}

static void __end_key_cb (void *user_data)
{
	return;
}

popup_info_s popup_list[DPM_SYSPOPUP_MAX] = {
	/* Application Policy */
	{"install_mode", "IDS_DPM_HEADER_APPLICATION_POLICY", "IDS_DPM_BODY_DISALLOW_INSTALL_MODE", __terminate_cb, __end_key_cb},
	{"uninstall_mode", "IDS_DPM_HEADER_APPLICATION_POLICY", "IDS_DPM_BODY_DISALLOW_UNINSTALL_MODE", __terminate_cb, __end_key_cb},

	/* Password Policy */
	{"password_expired", "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_EXPIRED", __terminate_cb, __end_key_cb},
	{"password_reset", "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_RESET", __terminate_cb, __end_key_cb},
	{"password_enforce_change", "IDS_DPM_HEADER_PASSWORD_POLICY", "IDS_DPM_BODY_PASSWORD_ENFORCE_CHANGE", __terminate_cb, __end_key_cb},

	/* Wi-Fi Policy */
	{"wifi", "IDS_DPM_HEADER_WIFI_POLICY", "IDS_DPM_BODY_DISALLOW_WIFI", __terminate_cb, __end_key_cb},
	{"hotspot", "IDS_DPM_HEADER_WIFI_POLICY", "IDS_DPM_BODY_DISALLOW_HOTSPOT", __terminate_cb, __end_key_cb},

	/* Restriction Policy */
	{"camera", "IDS_DPM_HEADER_CAMERA_POLICY", "IDS_DPM_BODY_DISALLOW_CAMERA", __terminate_cb, __end_key_cb},
	{"microphone", "IDS_DPM_HEADER_RECORD_POLICY", "IDS_DPM_BODY_DISALLOW_MICROPHONE", __terminate_cb, __end_key_cb},
	{"location", "IDS_DPM_HEADER_LOCATION_POLICY", "IDS_DPM_BODY_DISALLOW_LOCATION", __terminate_cb, __end_key_cb},

	/* Storage Policy */
	{"external_storage", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_DISALLOW_EXTERNAL_STORAGE", __terminate_cb, __end_key_cb},
	{"storage_encryption", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_DISALLOW_STORAGE_ENCRYPTION", __terminate_cb, __end_key_cb},
	{"storage_decryption", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_DISALLOW_STORAGE_DECRYPTION", __terminate_cb, __end_key_cb},
	{"wipe_device", "IDS_DPM_HEADER_STORAGE_POLICY", "IDS_DPM_BODY_WIPE_DEVICE", __terminate_cb, __end_key_cb},

	/* Bluetooth Policy */
	{"bluetooth", "IDS_DPM_HEADER_DISALLOW_BLUETOOTH", "IDS_DPM_BODY_DISALLOW_BLUETOOTH", __terminate_cb, __end_key_cb},

	/* Container Policy */
	{"create_zone", "IDS_DPM_HEADER_CONTAINER_POLICY", "IDS_DPM_BODY_CREATE_CONTAINER", __terminate_cb, __end_key_cb},
	{"remove_zone", "IDS_DPM_HEADER_CONTAINER_POLICY", "IDS_DPM_BODY_REMOVE_CONTAINER", __terminate_cb, __end_key_cb},
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

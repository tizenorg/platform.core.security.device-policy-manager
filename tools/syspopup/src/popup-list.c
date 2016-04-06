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
	{"restriction_camera", "IDS_DPM_POP_RESTRICTION_CAMERA_TITLE", "IDS_DPM_POP_RESTRICTION_CAMERA_CONTENT", __terminate_cb, __end_key_cb},
	{"restriction_location", "IDS_DPM_POP_RESTRICTION_LOCATION_TITLE", "IDS_DPM_POP_RESTRICTION_CONTENT", __terminate_cb, __end_key_cb},
};

popup_info_s *_get_dpm_popup_info(const char *popup_name)
{
	int iterator = 0;

	if (popup_name == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "popup_name is NULL");
		return NULL;
	}

	for (iterator = 0; iterator < DPM_SYSPOPUP_MAX; iterator++) {
		if (!strcmp(popup_name, popup_list[iterator].name))
			return &popup_list[iterator];
	}

	dlog_print(DLOG_ERROR, LOG_TAG, "[%s] popup is not exist", popup_name);
	return NULL;
}

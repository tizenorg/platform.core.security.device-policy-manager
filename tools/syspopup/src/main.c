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

static bool __app_create(void *data)
{
	return true;
}

static void __app_control(app_control_h app_control, void *data)
{
	int ret = 0;
	char *popup_name = NULL;

	ret = app_control_get_extra_data(app_control, "dpm-syspopup", &popup_name);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get bundle data");
		return;
	}

	_create_syspopup(popup_name);

	return;
}

static void __app_terminate(void *data)
{
	return;
}

static void __app_pause(void *data)
{
	return;
}

static void __app_resume(void *data)
{
	return;
}

static void __ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	int ret = 0;
	char *locale = NULL;

	ret = system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	if (ret != SYSTEM_SETTINGS_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get language");
		return;
	}

	elm_language_set(locale);
	free(locale);
	return;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	ui_app_lifecycle_callback_s event_callback = {0, };
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = __app_create;
	event_callback.terminate = __app_terminate;
	event_callback.pause = __app_pause;
	event_callback.resume = __app_resume;
	event_callback.app_control = __app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, __ui_app_lang_changed, NULL);

	ret = ui_app_main(argc, argv, &event_callback, NULL);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);

	return ret;
}

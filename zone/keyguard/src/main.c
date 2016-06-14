/*
 * Tizen Zone Keyguard application
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
#include "keyguard.h"
#include "widget.h"

static bool __app_create(void *data)
{
	_create_keyguard_window(NULL);
	return true;
}

static void __app_pause(void *data)
{
	return;
}

static void __app_resume(void *data)
{
	return;
}

static void __free_data(appdata_s *ad)
{
	free(ad->name);
}

static void __app_terminate(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	__free_data(ad);

	return ;
}

static void __app_control(app_control_h app_control, void *data)
{
	/*appdata_s *ad = (appdata_s *) data;
	char *locktype = NULL;
	int ret = 0;

	ret = app_control_get_extra_data(app_control, "name", &ad->name);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get name");
		ui_app_exit();
	}

	ret = app_control_get_extra_data(app_control, "locktype", &locktype);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get zone name");
		ui_app_exit();
	}

	if (!strcmp(locktype, "simple-password"))
		ad->locktype = SIMPLE_PASSWORD;
	else
		ad->locktype = PASSWORD;

	elm_app_base_scale_set(1.8);
	_create_keyguard_window(ad);*/

	return;
}

int main(int argc, char *argv[])
{
	appdata_s ad = {0, };
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0, };

	event_callback.create = __app_create;
	event_callback.terminate = __app_terminate;
	event_callback.pause = __app_pause;
	event_callback.resume = __app_resume;
	event_callback.app_control = __app_control;

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main is failed. err = %d", ret);

	return ret;
}

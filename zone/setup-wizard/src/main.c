/*
 * Tizen Zone Setup-Wizard application
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
#include "zone-setup.h"
#include "widget.h"

static void __zone_request_done(const char *from, const char *info, void *user_data)
{
	char uri[PATH_MAX];
	appdata_s *ad = (appdata_s *) user_data;
	ad->request_done = true;

	app_control_h app_control;
	app_control_create(&app_control);
	app_control_set_app_id(app_control, "org.tizen.kaskit");
	snprintf(uri, PATH_MAX, "zone://setup/%s", ad->zone_name);
	app_control_set_uri(app_control, uri);
	app_control_send_launch_request(app_control, NULL, NULL);
	app_control_destroy(app_control);

}

static bool __app_create(void *data)
{
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
	free(ad->zone_name);
	free(ad->mode);
}

static void __app_terminate(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	if (ad->zone_manager != NULL) {
		zone_manager_remove_event_cb(ad->zone_manager, ad->zone_event_cb_id);
		zone_manager_destroy(ad->zone_manager);
		ad->zone_manager = NULL;
	}

	__free_data(ad);

	return ;
}

static void __set_zone_callback(appdata_s *ad)
{
	char *cb_event_list[2] = {"created", "removed"};
	char *cb_event = NULL;

	if (zone_manager_create(&ad->zone_manager) != ZONE_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get zone manager handle");
		ui_app_exit();
	}

	if (!strcmp(ad->mode, "create"))
		cb_event = cb_event_list[0];
	else
		cb_event = cb_event_list[1];

	if (zone_manager_add_event_cb(ad->zone_manager, cb_event, __zone_request_done, ad, &ad->zone_event_cb_id) != ZONE_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to add zone signal callback");
		ui_app_exit();
	}

	return;
}

static void __app_control(app_control_h app_control, void *data)
{
	appdata_s *ad = (appdata_s *) data;
	int ret = 0;

	ret = app_control_get_extra_data(app_control, "mode", &ad->mode);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get mode");
		ui_app_exit();
	}

	if (strcmp(ad->mode, "create") && strcmp(ad->mode, "remove")) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Invalid mode");
		ui_app_exit();
	}

	ret = app_control_get_extra_data(app_control, "zone", &ad->zone_name);
	if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get zone name");
		ui_app_exit();
	}

	__set_zone_callback(ad);

	if (app_control_clone(&ad->app_control, app_control) != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to clone app control handler");
		ui_app_exit();
	}

	elm_app_base_scale_set(1.8);
	_create_base_window(ad);

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

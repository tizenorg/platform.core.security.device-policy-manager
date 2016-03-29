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

static void create_base_window(void *data)
{
	appdata_s *ad = (appdata_s *) data;
	Evas_Object *layout;

	char edj_path[PATH_MAX] = "\0";
	char *res_path = NULL;

	/* Initialize appdata */
	ad->vnum = WELCOME_WIN;
	ad->create_done = false;

	/* Get EDJ path */
	res_path = app_get_resource_path();
	if (res_path != NULL) {
		snprintf(edj_path, PATH_MAX, "%s%s", res_path, "org.tizen.zone-setup-wizard.edj");
		dlog_print(DLOG_ERROR, LOG_TAG, "failed get resource path");
		ui_app_exit();
	}
	ad->edj_path = strdup(edj_path);
	free(res_path);

	/* Create main UI widget */
	ad->main_win = setup_wizard_create_win(PACKAGE);
	ad->main_conform = setup_wizard_create_conformant(ad->main_win);
	layout = setup_wizard_create_layout(ad->main_conform, NULL, NULL);
	elm_object_content_set(ad->main_conform, layout);
	ad->nf = elm_naviframe_add(layout);

	create_welcome_view(ad);

	elm_object_part_content_set(layout, "elm.swallow.content", ad->nf);
	eext_object_event_callback_add(ad->nf, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);

	evas_object_show(ad->main_win);
	return ;
}

void create_zone_done(zone_state_e event, const char *name, void *info, void *user_data)
{
	appdata_s *ad = (appdata_s *) user_data;
	if (event == DPM_ZONE_DEFINED)
		ad->create_done = true;
	return ;
}

static bool app_create(void *data)
{
	int ret = true;
	appdata_s *ad = (appdata_s *)data;

	ad->dpm_client = dpm_create_client();
	if (ad->dpm_client == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get dpm client");
		return false;
	}

	if (dpm_subscribe_zone_signal(ad->dpm_client, create_zone_done, ad) != 0) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to set signal callback");
		return false;
	}

	elm_app_base_scale_set(1.8);
	create_base_window(ad);

	return ret;
}

static void app_pause(void *data)
{
	return ;
}

static void app_resume(void *data)
{
	return ;
}

static void app_terminate(void *data)
{
	appdata_s *ad = (appdata_s *) data;

	dpm_unsubscribe_zone_signal(ad->dpm_client, create_zone_done);
	dpm_destroy_client(ad->dpm_client);
	ad->dpm_client = NULL;
	return ;
}

int main(int argc, char *argv[])
{
	appdata_s ad = {0, };
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main is failed. err = %d", ret);

	return ret;
}

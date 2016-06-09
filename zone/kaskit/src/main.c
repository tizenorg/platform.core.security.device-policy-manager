/*
 * Tizen Krate launcher application
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
#include <pthread.h>

#include <zone/zone.h>
#include <zone/app-proxy.h>
#include <zone/package-proxy.h>

#include "kaskit.h"
#include "widget.h"

static zone_package_proxy_h __zone_pkg;
static zone_app_proxy_h __zone_app;
static zone_manager_h __zone_mgr;

static bool __app_terminated = false;

static bool __get_app_info_cb(app_info_h app_h, void* user_data)
{
	char* app_label = NULL, *app_icon = NULL, *app_id, *pkg_id;
	bool nodisplay = false;

        if (__app_terminated)
		return false;

	app_info_is_nodisplay(app_h, &nodisplay);
	if (nodisplay)
		return true;

	app_info_get_app_id(app_h, &app_id);
	app_info_get_label(app_h, &app_label);
	app_info_get_icon(app_h, &app_icon);
	app_info_get_package(app_h, &pkg_id);

	if (user_data == NULL ||  !strncmp(user_data, pkg_id, PATH_MAX)) {
		_create_app_icon(pkg_id, app_id, app_label, app_icon);
	}

	free(app_id);
	free(pkg_id);
	if (app_label != NULL) {
		free(app_label);
	}
	if (app_icon != NULL) {
		free(app_icon);
	}

	return true;
}

void* create_app_thread(void* data) {
	zone_app_proxy_foreach_app_info(__zone_app, __get_app_info_cb, data);
	if (data != NULL) {
		free(data);
	}
	return NULL;
}

void* destroy_app_thread(void* data) {
	_destroy_app_icon(data);
	free(data);
	return NULL;
}

static void __pkg_event_cb(const char* type,
	const char* pkg_id,
	package_manager_event_type_e event_type,
	package_manager_event_state_e event_state, int progress,
	package_manager_error_e error, void* user_data)
{
	pthread_t tid;

	if (event_state == PACKAGE_MANAGER_EVENT_STATE_COMPLETED) {
		if (event_type == PACKAGE_MANAGER_EVENT_TYPE_INSTALL) {
			pthread_create(&tid, NULL, create_app_thread, strdup(pkg_id));
		} else if (event_type == PACKAGE_MANAGER_EVENT_TYPE_UNINSTALL) {
			pthread_create(&tid, NULL, destroy_app_thread, strdup(pkg_id));
		}
	}
}

char* __get_current_zone_name() {
	struct passwd pwd, *result;
	int bufsize;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1) {
		bufsize = 16384;
	}

	char* ret, *buf = malloc(bufsize * sizeof(char));

	getpwuid_r(getuid(), &pwd, buf, bufsize, &result);
	if (result == NULL) {
		ret = NULL;
	} else {
		ret = strdup(result->pw_name);
	}
	free(buf);
	return ret;
}

static void __toast_callback_cb(void *data, Evas_Object *obj)
{
	ui_app_exit();
}

void _icon_clicked_cb(char *app_id)
{
	zone_app_proxy_launch(__zone_app, app_id);
	ui_app_exit();
}

static bool __app_create(void *data)
{
	zone_iterator_h it;
	const char* zone_name;
	char *current_zone_name;
	pthread_t tid;

	current_zone_name = __get_current_zone_name();
	zone_manager_create(&__zone_mgr);
	it = zone_manager_create_zone_iterator(__zone_mgr, ZONE_STATE_RUNNING);
	while (1) {
		zone_iterator_next(it, &zone_name);
		if (zone_name == NULL || strncmp(zone_name, current_zone_name, 16384)) {
			break;
		}

	}

	if (zone_name == NULL) {
		_create_toast("There is no zone", __toast_callback_cb, NULL);
		return true;
	}

	_create_kaskit_window(zone_name);

	zone_app_proxy_create(__zone_mgr, zone_name, &__zone_app);
	zone_package_proxy_create(__zone_mgr, zone_name, &__zone_pkg);

	zone_package_proxy_set_event_status(__zone_pkg,
		PACKAGE_MANAGER_STATUS_TYPE_INSTALL |
		PACKAGE_MANAGER_STATUS_TYPE_UNINSTALL);
	zone_package_proxy_set_event_cb(__zone_pkg, __pkg_event_cb, NULL);

	pthread_create(&tid, NULL, create_app_thread, NULL);

        zone_iterator_destroy(it);
	free(current_zone_name);

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

static void __app_terminate(void *data)
{
	__app_terminated = true;

	zone_package_proxy_destroy(__zone_pkg);
	zone_app_proxy_destroy(__zone_app);
	zone_manager_destroy(__zone_mgr);
}

static void __app_control(app_control_h app_control, void *data)
{
	return;
}

int main(int argc, char *argv[])
{
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0, };

	event_callback.create = __app_create;
	event_callback.terminate = __app_terminate;
	event_callback.pause = __app_pause;
	event_callback.resume = __app_resume;
	event_callback.app_control = __app_control;

	ret = ui_app_main(argc, argv, &event_callback, NULL);
	if (ret != APP_ERROR_NONE)
		dlog_print(DLOG_ERROR, LOG_TAG, "ui_app_main is failed. err = %d", ret);

	return ret;
}

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
#include <zone/zone.h>
#include <zone/app-proxy.h>
#include <zone/package-proxy.h>
#include <shortcut_manager.h>

#include "kaskit.h"
#include "widget.h"

static zone_package_proxy_h __zone_pkg;
static zone_app_proxy_h __zone_app;
static zone_manager_h __zone_mgr;

struct app_icon_s{
	char* id;
	char* label;
	char* icon;
	char* package;
	bool removable;
};

static void* __create_app_icon(void* data)
{
	struct app_icon_s* app = (struct app_icon_s*)data;

	_create_app_icon(app->package, app->id, app->label, app->icon, app->removable);

	return NULL;
}

static bool __pkg_is_removable(const char* pkg_id)
{
	bool removable = false;

	package_info_h pkg_h;

	zone_package_proxy_get_package_info(__zone_pkg, pkg_id, &pkg_h);
	package_info_is_removable_package(pkg_h, &removable);
	package_info_destroy(pkg_h);

	return removable;
}

static bool __get_app_info_cb(app_info_h app_h, void* user_data)
{
	struct app_icon_s app = {NULL, };
	bool nodisplay = true;

	app_info_is_nodisplay(app_h, &nodisplay);
	if (nodisplay)
		return true;

	app_info_get_package(app_h, &app.package);

	if (user_data == NULL ||  !strncmp(user_data, app.package, PATH_MAX)) {
		app_info_get_app_id(app_h, &app.id);
		app_info_get_label(app_h, &app.label);
		app_info_get_icon(app_h, &app.icon);
		app.removable = __pkg_is_removable(app.package);

		ecore_main_loop_thread_safe_call_sync(__create_app_icon, &app);

		free(app.id);
		if (app.label != NULL) {
			free(app.label);
		}
		if (app.icon != NULL) {
			free(app.icon);
		}
	}
	free(app.package);

	return true;
}

static void __create_icon_thread(void* data, Ecore_Thread* thread) {
	zone_app_proxy_foreach_app_info(__zone_app, __get_app_info_cb, data);
	if (data != NULL) {
		free(data);
	}
}

void __pkg_event_cb(const char* type,
	const char* pkg_id,
	package_manager_event_type_e event_type,
	package_manager_event_state_e event_state, int progress,
	package_manager_error_e error, void* user_data)
{
	if (event_state == PACKAGE_MANAGER_EVENT_STATE_COMPLETED) {
		if (event_type == PACKAGE_MANAGER_EVENT_TYPE_INSTALL) {
			ecore_thread_run(__create_icon_thread, NULL, NULL, strdup(pkg_id));
		} else if (event_type == PACKAGE_MANAGER_EVENT_TYPE_UNINSTALL) {
			_destroy_app_icon(pkg_id);
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

void _icon_clicked_cb(const char *app_id)
{
	zone_app_proxy_launch(__zone_app, app_id);
}

void _icon_uninstalled_cb(const char *pkg_id)
{
	zone_package_proxy_uninstall(__zone_pkg, pkg_id);
}

static int __shortcut_result_cb(int ret,  void *data)
{
	dlog_print(DLOG_ERROR, LOG_TAG, "__shortcut_result_cb = %d", ret);
	return 0;
}

static void __add_shortcut(const char *zone_name)
{
	char new_uri[PATH_MAX];

	snprintf(new_uri, sizeof(new_uri), "zone://launch/%s", zone_name);
	shortcut_add_to_home(zone_name, LAUNCH_BY_URI, new_uri, "", 0, __shortcut_result_cb, NULL);
}

static void __show_launcher(const char *zone_name)
{
	_set_kaskit_window_title(zone_name);

	zone_app_proxy_create(__zone_mgr, zone_name, &__zone_app);
	zone_package_proxy_create(__zone_mgr, zone_name, &__zone_pkg);

	zone_package_proxy_set_event_status(__zone_pkg,
		PACKAGE_MANAGER_STATUS_TYPE_INSTALL |
		PACKAGE_MANAGER_STATUS_TYPE_UNINSTALL);
	zone_package_proxy_set_event_cb(__zone_pkg, __pkg_event_cb, NULL);

	ecore_thread_run(__create_icon_thread, NULL, NULL, NULL);
}

static bool __app_create(void *data)
{
	zone_manager_create(&__zone_mgr);

	_create_kaskit_window();

	return true;
}

static void __app_control(app_control_h app_control, void *data)
{
	char* zone_uri, *zone_name = "";
        int ret = 0;

        ret = app_control_get_uri(app_control, &zone_uri);
        if (ret != APP_CONTROL_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "No URI");
                ui_app_exit();
        }

	if (strncmp(zone_uri, "zone://", sizeof("zone://") - 1) != 0) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Mismatched URI");
                ui_app_exit();
	}

	zone_uri = zone_uri + sizeof("zone://") - 1;

	if (strncmp(zone_uri, "setup/", sizeof("setup/") - 1) == 0) {
		zone_name = zone_uri + sizeof("setup/") - 1;
		__add_shortcut(zone_name);
		__show_launcher(zone_name);
	} else if (strncmp(zone_uri, "launch/", sizeof("launch/") - 1) == 0) {
		zone_name = zone_uri + sizeof("launch/") - 1;
		__show_launcher(zone_name);
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "Invalid URI");
                ui_app_exit();
	}
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
	zone_package_proxy_destroy(__zone_pkg);
	zone_app_proxy_destroy(__zone_app);
	zone_manager_destroy(__zone_mgr);
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

/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#include <cstdlib>
#include <cstring>

#include "zone.h"
#include "debug.h"
#include "app-info.h"
#include "app-info-internal.h"

#include "error.h"
#include "auth/user.h"
#include "policy-client.h"
#include "zone/app-manager.hxx"

using namespace DevicePolicyManager;

typedef struct zone_app_manager_s {
    std::unique_ptr<DevicePolicyContext> pContext;
    std::unique_ptr<ZoneAppManager> pManager;
} zone_app_manager_s;

inline ZoneAppManager* getManager(zone_app_manager_h handle) {
    return reinterpret_cast<zone_app_manager_s *>(handle)->pManager.get();
}

static app_info_h make_app_info_h(const ZoneAppManager::AppInfo& info)
{
    if (info.id.empty()) {
        return NULL;
    }

    application_x* app = (application_x*)::malloc(sizeof(application_x));
    pkgmgr_appinfo_x* pkgappinfo = (pkgmgr_appinfo_x*)::malloc(sizeof(pkgmgr_appinfo_x));
    app_info_s* appinfo = (app_info_s*)::malloc(sizeof(struct app_info_s));

    ::memset(appinfo, 0, sizeof(app_info_s));
    appinfo->app_id = ::strdup(info.id.c_str());
    appinfo->pkg_app_info = pkgappinfo;

    ::memset(pkgappinfo, 0, sizeof(pkgmgr_appinfo_x));
    pkgappinfo->package = ::strdup(info.package.c_str());
    pkgappinfo->locale = ::strdup(info.locale.c_str());
    pkgappinfo->app_component = info.componentType;
    pkgappinfo->app_info = app;

    ::memset(app, 0, sizeof(application_x));
    app->appid = ::strdup(info.id.c_str());
    app->nodisplay = ::strdup(info.isNoDisplayed? "true":"false");
    app->taskmanage = ::strdup(info.isTaskManaged? "true":"false");
    
    app->icon = ::g_list_append(NULL, ::strdup(info.icon.c_str()));
    app->label = ::g_list_append(NULL, ::strdup(info.label.c_str()));

    return reinterpret_cast<app_info_h>(appinfo);
} 

int zone_app_manager_create(zone_app_manager_h *manager)
{
    RET_ON_FAILURE(manager, ZONE_ERROR_INVALID_PARAMETER);

    zone_app_manager_s* handle = new zone_app_manager_s;

    handle->pContext.reset(new(std::nothrow) DevicePolicyContext());

    if (handle->pContext->connect() < 0) {
        delete handle;
        return ZONE_ERROR_CONNECTION_REFUSED;
    }

    handle->pManager.reset(handle->pContext->createPolicyInterface<ZoneAppManager>());

    *manager = reinterpret_cast<zone_app_manager_h>(handle);
    return ZONE_ERROR_NONE;
}

int zone_app_manager_destroy(zone_app_manager_h handle)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<zone_app_manager_s *>(handle);
    return ZONE_ERROR_NONE;
}

int zone_app_manager_get_app_info(zone_app_manager_h handle, const char* name, const char* app_id, app_info_h* app_info)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app_id, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app_info, ZONE_ERROR_INVALID_PARAMETER);

    const auto& info = getManager(handle)->get(name, app_id);
    app_info_h ret = make_app_info_h(info);

    if (ret == NULL) {
        return ZONE_ERROR_INVALID_PARAMETER;
    }

    *app_info = ret;

    return ZONE_ERROR_NONE;
}

int zone_app_manager_foreach_app_info(zone_app_manager_h handle, const char* name, app_manager_app_info_cb callback, void *user_data)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, ZONE_ERROR_INVALID_PARAMETER);

    const auto& list = getManager(handle)->getList(name);

    for (const auto& info : list) {
        app_info_h info_h = make_app_info_h(info);
        callback(info_h, user_data);
        app_info_destroy(info_h);
    } 

    return ZONE_ERROR_NONE;
}

int zone_app_manager_launch(zone_app_manager_h handle, const char* name, const char* app_id)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app_id, ZONE_ERROR_INVALID_PARAMETER);

    return getManager(handle)->launch(name, app_id);
}

int zone_app_manager_is_running(zone_app_manager_h handle, const char* name, const char* app_id, int *result)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app_id, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, ZONE_ERROR_INVALID_PARAMETER);

    *result = getManager(handle)->isRunning(name, app_id);
    return ZONE_ERROR_NONE;
}

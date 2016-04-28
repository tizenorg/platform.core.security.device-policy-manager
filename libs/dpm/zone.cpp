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

#include "zone.h"
#include "zone.hxx"

#include "array.h"
#include "debug.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

dpm_zone_policy_h dpm_context_acquire_zone_policy(dpm_context_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    DevicePolicyContext &client = GetDevicePolicyContext(handle);
    return client.createPolicyInterface<ZonePolicy>();
}

int dpm_context_release_zone_policy(dpm_context_h context, dpm_zone_policy_h handle)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);

    delete &GetPolicyInterface<ZonePolicy>(handle);
    return 0;
}

int dpm_zone_create(dpm_zone_policy_h handle, const char* name, const char* pkgname)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgname, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.createZone(name, pkgname);
}

int dpm_zone_destroy(dpm_zone_policy_h handle, const char* name)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.removeZone(name);
}

typedef runtime::Array<std::string> dpm_zone_iterator;

dpm_zone_iterator_h dpm_zone_create_iterator(dpm_zone_policy_h handle)
{
    RET_ON_FAILURE(handle, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);

    return reinterpret_cast<dpm_zone_iterator_h>(new dpm_zone_iterator(zone.getZoneList()));
}

int dpm_zone_iterator_next(dpm_zone_iterator_h iter, const char** result)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    dpm_zone_iterator* it = reinterpret_cast<dpm_zone_iterator*>(iter);

    if (it->isEnd())
        *result = NULL;
    else
        *result = it->next().c_str();

    return 0;
}

int dpm_zone_destroy_iterator(dpm_zone_iterator_h iter)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<dpm_zone_iterator*>(iter);

    return 0;
}

int dpm_zone_foreach_name(dpm_zone_policy_h handle,
                          dpm_zone_foreach_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    std::vector<std::string> list = zone.getZoneList();
    for (std::vector<std::string>::iterator it = list.begin();
         it != list.end(); it++) {
        callback((*it).c_str(), user_data);
    }

    return 0;
}

int dpm_zone_get_state(dpm_zone_policy_h handle, const char* name, dpm_zone_state_e *state)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);

    //ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);

    /* TODO : should implement */

    return DPM_ERROR_INVALID_PARAMETER;
}

dpm_zone_pkg_h dpm_zone_acquire_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgid)
{
    RET_ON_FAILURE(handle, NULL);
    RET_ON_FAILURE(name, NULL);
    RET_ON_FAILURE(pkgid, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::PkgInfo* pPkg =  new ZonePolicy::PkgInfo();
    *pPkg = std::move(zone.getPkg(name, pkgid));

    return reinterpret_cast<dpm_zone_pkg_h>(pPkg);
}

int dpm_zone_pkg_release(dpm_zone_pkg_h pkg)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);

    return 0;
}

typedef runtime::Array<ZonePolicy::PkgInfo> dpm_zone_pkg_iterator;

dpm_zone_iterator_h dpm_zone_pkg_create_iterator(dpm_zone_policy_h handle, const char* name)
{
    RET_ON_FAILURE(handle, NULL);
    RET_ON_FAILURE(name, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);

    return reinterpret_cast<dpm_zone_pkg_iterator_h>(new dpm_zone_pkg_iterator(zone.getPkgList(name)));
}

int dpm_zone_pkg_iterator_next(dpm_zone_pkg_iterator_h iter, dpm_zone_pkg_h* result)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    dpm_zone_pkg_iterator* it = reinterpret_cast<dpm_zone_pkg_iterator*>(iter);

    if (it->isEnd())
        *result = NULL;
    else
        *result = reinterpret_cast<dpm_zone_pkg_h>(&it->next());

    return 0;
}

int dpm_zone_pkg_destroy_iterator(dpm_zone_pkg_iterator_h iter)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<dpm_zone_pkg_iterator*>(iter);

    return 0;
}

int dpm_zone_foreach_pkg(dpm_zone_policy_h handle, const char* name,
                         dpm_zone_pkg_foreach_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    std::vector<ZonePolicy::PkgInfo> list = zone.getPkgList(name);
    for (std::vector<ZonePolicy::PkgInfo>::iterator it = list.begin();
         it != list.end(); it++) {
        callback(reinterpret_cast<dpm_zone_pkg_h>(&(*it)), user_data);
    }

    return 0;
}

int dpm_zone_pkg_get_id(dpm_zone_pkg_h pkg, const char **id)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(id, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *id = ::strdup(pPkg->id.c_str());

    return 0;
}

int dpm_zone_pkg_get_type(dpm_zone_pkg_h pkg, const char **type)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(type, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *type = ::strdup(pPkg->type.c_str());

    return 0;
}

int dpm_zone_pkg_get_icon(dpm_zone_pkg_h pkg, const char **icon)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(icon, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *icon = ::strdup(pPkg->icon.c_str());

    return 0;
}

int dpm_zone_pkg_get_label(dpm_zone_pkg_h pkg, const char **label)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(label, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *label = ::strdup(pPkg->label.c_str());

    return 0;
}

int dpm_zone_pkg_get_version(dpm_zone_pkg_h pkg, const char **version)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(version, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *version = ::strdup(pPkg->version.c_str());

    return 0;
}

int dpm_zone_pkg_is_system(dpm_zone_pkg_h pkg, int* result)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *result = pPkg->isSystem;

    return 0;
}

int dpm_zone_pkg_is_removable(dpm_zone_pkg_h pkg, int* result)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *result = pPkg->isRemovable;

    return 0;
}

int dpm_zone_pkg_is_preload(dpm_zone_pkg_h pkg, int* result)
{
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    *result = pPkg->isPreload;

    return 0;
}

int dpm_zone_install_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgpath)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgpath, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.installPkg(name, pkgpath);
}

int dpm_zone_uninstall_pkg(dpm_zone_policy_h handle, dpm_zone_pkg_h pkg)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    return zone.uninstallPkg(pPkg->zone, pPkg->id);
}

dpm_zone_app_h dpm_zone_acquire_app(dpm_zone_policy_h handle, dpm_zone_pkg_h pkg, const char* appid)
{
    RET_ON_FAILURE(handle, NULL);
    RET_ON_FAILURE(pkg, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    ZonePolicy::AppInfo* pApp = new ZonePolicy::AppInfo();
    *pApp = std::move(zone.getApp(pPkg->zone, pPkg->id));

    return reinterpret_cast<dpm_zone_app_h>(pApp);
}

int dpm_zone_app_release(dpm_zone_app_h app)
{
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<ZonePolicy::AppInfo*>(app);

    return 0;
}

typedef runtime::Array<ZonePolicy::AppInfo> dpm_zone_uiapp_iterator;

dpm_zone_iterator_h dpm_zone_uiapp_create_iterator(dpm_zone_policy_h handle, dpm_zone_pkg_h pkg)
{
    RET_ON_FAILURE(handle, NULL);
    RET_ON_FAILURE(pkg, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);

    return reinterpret_cast<dpm_zone_uiapp_iterator_h>(new dpm_zone_uiapp_iterator(zone.getUIAppList(pPkg->zone, pPkg->id)));
}

int dpm_zone_uiapp_iterator_next(dpm_zone_uiapp_iterator_h iter, dpm_zone_app_h* result)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    dpm_zone_uiapp_iterator* it = reinterpret_cast<dpm_zone_uiapp_iterator*>(iter);

    if (it->isEnd())
        *result = NULL;
    else
        *result = reinterpret_cast<dpm_zone_app_h>(&it->next());

    return 0;
}

int dpm_zone_uiapp_destroy_iterator(dpm_zone_uiapp_iterator_h iter)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<dpm_zone_uiapp_iterator*>(iter);

    return 0;
}

int dpm_zone_foreach_uiapp(dpm_zone_policy_h handle, dpm_zone_pkg_h pkg,
                           dpm_zone_app_foreach_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkg, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::PkgInfo* pPkg = reinterpret_cast<ZonePolicy::PkgInfo*>(pkg);
    std::vector<ZonePolicy::AppInfo> list = zone.getUIAppList(pPkg->zone, pPkg->id);
    for (std::vector<ZonePolicy::AppInfo>::iterator it = list.begin();
         it != list.end(); it++) {
        callback(reinterpret_cast<dpm_zone_app_h>(&(*it)), user_data);
    }

    return 0;
}

int dpm_zone_app_get_id(dpm_zone_app_h app, const char **id)
{
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(id, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    *id = ::strdup(pApp->id.c_str());

    return 0;
}

int dpm_zone_app_get_icon(dpm_zone_app_h app, const char **icon)
{
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(icon, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    *icon = ::strdup(pApp->icon.c_str());

    return 0;
}

int dpm_zone_app_get_label(dpm_zone_app_h app, const char **label)
{
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(label, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    *label = ::strdup(pApp->label.c_str());

    return 0;
}

int dpm_zone_app_is_nodisplayed(dpm_zone_app_h app, int* result)
{
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    *result = pApp->isNoDisplayed;

    return 0;
}

int dpm_zone_app_is_taskmanaged(dpm_zone_app_h app, int* result)
{
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    *result = pApp->isTaskManaged;

    return 0;
}

int dpm_zone_launch_app(dpm_zone_policy_h handle, dpm_zone_app_h app)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    return zone.launchApp(pApp->zone, pApp->id);
}

int dpm_zone_resume_app(dpm_zone_policy_h handle, dpm_zone_app_h app)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    return zone.resumeApp(pApp->zone, pApp->id);
}

int dpm_zone_terminate_app(dpm_zone_policy_h handle, dpm_zone_app_h app)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    return zone.terminateApp(pApp->zone, pApp->id);
}

int dpm_zone_is_app_running(dpm_zone_policy_h handle, dpm_zone_app_h app, int* result)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(app, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    ZonePolicy::AppInfo* pApp = reinterpret_cast<ZonePolicy::AppInfo*>(app);
    *result = zone.isRunningApp(pApp->zone, pApp->id);

    return 0;
}

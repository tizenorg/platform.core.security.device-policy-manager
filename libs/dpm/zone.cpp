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

    *result = reinterpret_cast<dpm_zone_iterator*>(iter)->next()->c_str();

    RET_ON_FAILURE(*result, DPM_ERROR_INVALID_PARAMETER);

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

typedef runtime::Array<std::string> dpm_zone_pkg_iterator;

dpm_zone_iterator_h dpm_zone_pkg_create_iterator(dpm_zone_policy_h handle, const char* name)
{
    RET_ON_FAILURE(handle, NULL);
    RET_ON_FAILURE(name, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);

    return reinterpret_cast<dpm_zone_pkg_iterator_h>(new dpm_zone_pkg_iterator(zone.getPackageList(name)));
}

int dpm_zone_pkg_iterator_next(dpm_zone_pkg_iterator_h iter, const char** result)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    *result = reinterpret_cast<dpm_zone_pkg_iterator*>(iter)->next()->c_str();

    RET_ON_FAILURE(*result, DPM_ERROR_INVALID_PARAMETER);

    return 0;
}

int dpm_zone_pkg_destroy_iterator(dpm_zone_pkg_iterator_h iter)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<dpm_zone_pkg_iterator*>(iter);

    return 0;
}

int dpm_zone_foreach_pkg_id(dpm_zone_policy_h handle, const char* name,
                            dpm_zone_pkg_foreach_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    std::vector<std::string> list = zone.getPackageList(name);
    for (std::vector<std::string>::iterator it = list.begin();
         it != list.end(); it++) {
        callback((*it).c_str(), user_data);
    }

    return 0;
}

int dpm_zone_get_pkg_type(dpm_zone_policy_h handle, const char* name, const char* pkgid, const char **type)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(type, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *type = ::strdup(zone.getPackageType(name, pkgid).c_str());

    return 0;
}

int dpm_zone_get_pkg_icon(dpm_zone_policy_h handle, const char* name, const char* pkgid, const char **icon)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(icon, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *icon = ::strdup(zone.getPackageIcon(name, pkgid).c_str());

    return 0;
}

int dpm_zone_get_pkg_label(dpm_zone_policy_h handle, const char* name, const char* pkgid, const char **label)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(label, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *label = ::strdup(zone.getPackageLabel(name, pkgid).c_str());

    return 0;
}

int dpm_zone_get_pkg_version(dpm_zone_policy_h handle, const char* name, const char* pkgid, const char **version)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(version, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *version = ::strdup(zone.getPackageType(name, pkgid).c_str());

    return 0;
}

int dpm_zone_is_system_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgid, int *result)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *result = zone.isSystemPackage(name, pkgid);

    return 0;
}

int dpm_zone_is_removable_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgid, int *result)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *result = zone.isRemovablePackage(name, pkgid);

    return 0;
}

int dpm_zone_is_preload_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgid, int *result)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *result = zone.isPreloadPackage(name, pkgid);

    return 0;
}

int dpm_zone_install_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgpath)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgpath, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.installPackage(name, pkgpath);
}

int dpm_zone_uninstall_pkg(dpm_zone_policy_h handle, const char* name, const char* pkgid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(pkgid, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.uninstallPackage(name, pkgid);
}

typedef runtime::Array<std::string> dpm_zone_uiapp_iterator;

dpm_zone_iterator_h dpm_zone_uiapp_create_iterator(dpm_zone_policy_h handle, const char* name, const char* pkgid)
{
    RET_ON_FAILURE(handle, NULL);
    RET_ON_FAILURE(name, NULL);
    RET_ON_FAILURE(pkgid, NULL);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);

    return reinterpret_cast<dpm_zone_uiapp_iterator_h>(new dpm_zone_uiapp_iterator(zone.getUIAppList(name, pkgid)));
}

int dpm_zone_uiapp_iterator_next(dpm_zone_uiapp_iterator_h iter, const char** result)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    *result = reinterpret_cast<dpm_zone_uiapp_iterator*>(iter)->next()->c_str();

    RET_ON_FAILURE(*result, DPM_ERROR_INVALID_PARAMETER);

    return 0;
}

int dpm_zone_uiapp_destroy_iterator(dpm_zone_uiapp_iterator_h iter)
{
    RET_ON_FAILURE(iter, DPM_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<dpm_zone_uiapp_iterator*>(iter);

    return 0;
}

int dpm_zone_foreach_uiapp_id(dpm_zone_policy_h handle,
                              const char* name, const char* pkgid,
                              dpm_zone_pkg_foreach_cb callback, void* user_data)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    std::vector<std::string> list = zone.getUIAppList(name, pkgid);
    for (std::vector<std::string>::iterator it = list.begin();
         it != list.end(); it++) {
        callback((*it).c_str(), user_data);
    }

    return 0;
}

int dpm_zone_get_app_icon(dpm_zone_policy_h handle, const char* name, const char* appid, const char **icon)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(icon, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *icon = ::strdup(zone.getAppIcon(name, appid).c_str());

    return 0;
}

int dpm_zone_get_app_label(dpm_zone_policy_h handle, const char* name, const char* appid, const char **label)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(label, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *label = ::strdup(zone.getAppLabel(name, appid).c_str());

    return 0;
}

int dpm_zone_is_nodisplayed_app(dpm_zone_policy_h handle, const char* name, const char* appid, int *result)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *result = zone.isNoDisplayedApp(name, appid);

    return 0;
}

int dpm_zone_is_taskmanaged_app(dpm_zone_policy_h handle, const char* name, const char* appid, int *result)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *result = zone.isTaskManagedApp(name, appid);

    return 0;
}

int dpm_zone_launch_app(dpm_zone_policy_h handle, const char* name, const char* appid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.launchApp(name, appid);
}

int dpm_zone_resume_app(dpm_zone_policy_h handle, const char* name, const char* appid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.resumeApp(name, appid);
}

int dpm_zone_terminate_app(dpm_zone_policy_h handle, const char* name, const char* appid)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    return zone.terminateApp(name, appid);
}

int dpm_zone_is_app_running(dpm_zone_policy_h handle, const char* name, const char* appid, int *result)
{
    RET_ON_FAILURE(handle, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(appid, DPM_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(result, DPM_ERROR_INVALID_PARAMETER);

    ZonePolicy& zone = GetPolicyInterface<ZonePolicy>(handle);
    *result = zone.isRunningApp(name, appid);

    return 0;
}

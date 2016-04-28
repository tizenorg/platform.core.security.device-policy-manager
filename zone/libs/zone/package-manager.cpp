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
#include "package-manager.h"
#include "package-info-internal.h"

#include "error.h"
#include "auth/user.h"
#include "policy-client.h"
#include "zone/package-manager.hxx"

using namespace DevicePolicyManager;

typedef struct zone_package_manager_s {
    std::unique_ptr<DevicePolicyContext> pContext;
    std::unique_ptr<ZonePackageManager> pManager;
} zone_package_manager_s;

inline ZonePackageManager* getManager(zone_package_manager_h handle) {
    return reinterpret_cast<zone_package_manager_s *>(handle)->pManager.get();
}

static package_info_h make_package_info_h(const ZonePackageManager::PkgInfo& info)
{
    uid_t zone_uid;

    if (info.id.empty()) {
        return NULL;
    }

    try {
        runtime::User pkgOwner(info.zone);
        zone_uid = pkgOwner.getUid();
    } catch (runtime::Exception &e) {
        return NULL;
    }   

    package_x* package = (package_x*)::calloc(1, sizeof(package_x));
    pkgmgr_pkginfo_x* pkginfo = (pkgmgr_pkginfo_x*)::calloc(1, sizeof(pkgmgr_pkginfo_x));
    package_info_s* packageinfo = (package_info_s*)::calloc(1, sizeof(package_info_s));

    packageinfo->package = ::strdup(info.id.c_str());
    packageinfo->pkgmgr_pkginfo = pkginfo;

    pkginfo->uid = zone_uid;
    pkginfo->locale = ::strdup(info.locale.c_str());
    pkginfo->pkg_info = package;

    package->for_all_users = ::strdup("false");
    package->package = ::strdup(info.id.c_str());
    package->version = ::strdup(info.version.c_str());
    package->removable = ::strdup(info.isRemovable? "true":"false");
    package->preload = ::strdup(info.isPreload? "true":"false");
    package->system = ::strdup(info.isSystem? "true":"false");
    package->type = ::strdup(info.type.c_str());
    package->mainapp_id = ::strdup(info.mainAppId.c_str());
    package->api_version = ::strdup(info.apiVersion.c_str());

    icon_x* icon = (icon_x*)::calloc(1, sizeof(icon_x));
    icon->text = ::strdup(info.icon.c_str());
    icon->lang = ::strdup(info.locale.c_str());
    package->icon = ::g_list_append(NULL, icon);

    label_x* label = (label_x*)::calloc(1, sizeof(label_x));
    label->text = ::strdup(info.label.c_str());
    label->lang = ::strdup(info.locale.c_str());
    package->label = ::g_list_append(NULL, label);

    description_x* desc = (description_x*)::calloc(1, sizeof(description_x));
    desc->text = ::strdup(info.description.c_str());
    desc->lang = ::strdup(info.locale.c_str());
    package->description = ::g_list_append(NULL, desc);

    author_x* author = (author_x*)::calloc(1, sizeof(author_x));
    author->text = ::strdup(info.author.name.c_str());
    author->email = ::strdup(info.author.email.c_str());
    author->href = ::strdup(info.author.href.c_str());
    author->lang = ::strdup(info.locale.c_str());
    package->author = ::g_list_append(NULL, author);

    return reinterpret_cast<package_info_h>(packageinfo);
} 

int zone_package_manager_create(zone_package_manager_h *manager)
{
    RET_ON_FAILURE(manager, ZONE_ERROR_INVALID_PARAMETER);

    zone_package_manager_s* handle = new zone_package_manager_s;

    handle->pContext.reset(new(std::nothrow) DevicePolicyContext());

    if (handle->pContext->connect() < 0) {
        delete handle;
        return ZONE_ERROR_CONNECTION_REFUSED;
    }

    handle->pManager.reset(handle->pContext->createPolicyInterface<ZonePackageManager>());

    *manager = reinterpret_cast<zone_package_manager_h>(handle);
    return ZONE_ERROR_NONE;
}

int zone_package_manager_destroy(zone_package_manager_h handle)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);

    delete reinterpret_cast<zone_package_manager_s *>(handle);
    return ZONE_ERROR_NONE;
}

int zone_package_manager_get_package_info(zone_package_manager_h handle, const char* name, const char* package_id, package_info_h* package_info)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(package_id, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(package_info, ZONE_ERROR_INVALID_PARAMETER);

    const auto& info = getManager(handle)->get(name, package_id);
    package_info_h ret = make_package_info_h(info);

    if (ret == NULL) {
        return ZONE_ERROR_INVALID_PARAMETER;
    }

    *package_info = ret;

    return ZONE_ERROR_NONE;
}

int zone_package_manager_foreach_package_info(zone_package_manager_h handle, const char* name, package_manager_package_info_cb callback, void *user_data)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(callback, ZONE_ERROR_INVALID_PARAMETER);

    auto manager = getManager(handle);
    const auto& list = manager->getIdList(name);
    for (const auto& pkgid : list) {
        package_info_h info_h = make_package_info_h(manager->get(name, pkgid));
        int ret = callback(info_h, user_data);
        package_info_destroy(info_h);
        if (!ret) {
            continue;
        }
    } 

    return ZONE_ERROR_NONE;
}

int zone_package_manager_install(zone_package_manager_h handle, const char* name, const char* package_path)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(package_path, ZONE_ERROR_INVALID_PARAMETER);

    return getManager(handle)->install(name, package_path);
}

int zone_package_manager_uninstall(zone_package_manager_h handle, const char* name, const char* package_id)
{
    RET_ON_FAILURE(handle, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(name, ZONE_ERROR_INVALID_PARAMETER);
    RET_ON_FAILURE(package_id, ZONE_ERROR_INVALID_PARAMETER);

    return getManager(handle)->uninstall(name, package_id);
}

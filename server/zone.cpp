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
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include <tzplatform_config.h>

#include "zone.hxx"

#include "error.h"
#include "smack.h"
#include "process.h"
#include "packman.h"
#include "launchpad.h"
#include "filesystem.h"
#include "auth/user.h"
#include "auth/group.h"
#include "xml/parser.h"
#include "xml/document.h"
#include "audit/logger.h"
#include "dbus/connection.h"

#define ZONE_UID_MIN       60001
#define ZONE_UID_MAX       65000

namespace DevicePolicyManager {

ZonePolicy::ZonePolicy(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (int)(ZonePolicy::createZone)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::removeZone)(std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::lockZone)(std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::unlockZone)(std::string));
    context.registerNonparametricMethod(this, (std::vector<std::string>)(ZonePolicy::getZoneList)());
    context.registerParametricMethod(this, (int)(ZonePolicy::getZoneState)(std::string));

    context.createNotification("ZonePolicy::created");
    context.createNotification("ZonePolicy::removed");
}

ZonePolicy::~ZonePolicy()
{
    PackageManager& packageManager = PackageManager::instance();
    packageManager.unsetEventCallback();
}

int ZonePolicy::createZone(const std::string& name, const std::string& setupWizAppid)
{
    std::vector<std::string> userData = {"app-id", setupWizAppid, "zone", name};

    try {
        Bundle bundle;
        bundle.add("id", "zone-create");
        bundle.add("user-data", userData);

        Launchpad launchpad(context.getPeerUid());
        launchpad.launch("org.tizen.dpm-syspopup", bundle);
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

int ZonePolicy::removeZone(const std::string& name)
{

    return 0;
}

int ZonePolicy::lockZone(const std::string& name)
{
    return 0;
}

int ZonePolicy::unlockZone(const std::string& name)
{

    return 0;
}

std::vector<std::string> ZonePolicy::getZoneList()
{
    return std::vector<std::string>();
}

int ZonePolicy::getZoneState(const std::string& name)
{
    return 0;
}

ZonePolicy zonePolicy(Server::instance());

} // namespace DevicePolicyManager

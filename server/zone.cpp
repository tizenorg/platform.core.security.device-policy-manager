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
#include <regex>
#include <algorithm>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>

#include <tzplatform_config.h>

#include "zone.hxx"
#include "zone/zone.hxx"

#include "error.h"
#include "launchpad.h"
#include "filesystem.h"
#include "auth/user.h"
#include "audit/logger.h"

#define NAME_PATTERN "^[A-Za-z_][A-Za-z0-9_.-]*"

namespace DevicePolicyManager {

namespace {

namespace {
std::regex zoneNamePattern(NAME_PATTERN);
}

bool isAllowedName(const std::string& name) {
    if (!std::regex_match(name, zoneNamePattern)) {
        return false;
    }

    bool exists = false;
    try {
        runtime::User user(name);
        exists = true;
    } catch (runtime::Exception& e) {}

    return !exists;
}

}

ZonePolicy::ZonePolicy(PolicyControlContext& ctx)
    : context(ctx)
{
    context.registerParametricMethod(this, (int)(ZonePolicy::createZone)(std::string, std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::removeZone)(std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::lockZone)(std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::unlockZone)(std::string));
    context.registerParametricMethod(this, (int)(ZonePolicy::getZoneState)(std::string));
    context.registerParametricMethod(this, (std::vector<std::string>)(ZonePolicy::getZoneList)(int));

    context.createNotification("ZonePolicy::created");
    context.createNotification("ZonePolicy::removed");
}

ZonePolicy::~ZonePolicy()
{
}


int ZonePolicy::createZone(const std::string& name, const std::string& setupWizAppid)
{
    if (!std::regex_match(name, zoneNamePattern)) {
        return -1;
    }

    if (!isAllowedName(name)) {
        return -1;
    }

    try {
        std::vector<std::string> data = {"app-id", "org.tizen.zone-setup-wizard",
                                         "mode", "create",
                                         "zone", name};
        Bundle bundle;
        bundle.add("id", "zone-create");
        bundle.add("user-data", data);

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
    if (getZoneState(name) == 0) {
        return -1;
    }

    try {
        std::vector<std::string> data = {"app-id", "org.tizen.zone-setup-wizard",
                                         "mode", "remove",
                                         "zone", name};
        Bundle bundle;
        bundle.add("id", "zone-remove");
        bundle.add("user-data", data);

        Launchpad launchpad(context.getPeerUid());
        launchpad.launch("org.tizen.dpm-syspopup", bundle);
    } catch (runtime::Exception& e) {
        ERROR(e.what());
        return -1;
    }

    return 0;
}

/* [TBD] remove dependency with zoneManager like this */
extern ZoneManager zoneManager;

int ZonePolicy::lockZone(const std::string& name)
{
    return zoneManager.lockZone(name);
}

int ZonePolicy::unlockZone(const std::string& name)
{
    return zoneManager.unlockZone(name);
}

int ZonePolicy::getZoneState(const std::string& name)
{
    return zoneManager.getZoneState(name);
}

std::vector<std::string> ZonePolicy::getZoneList(int state)
{
    return zoneManager.getZoneList(state);
}

ZonePolicy zonePolicy(Server::instance());

} // namespace DevicePolicyManager

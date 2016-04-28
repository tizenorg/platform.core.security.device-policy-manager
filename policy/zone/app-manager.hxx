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

#ifndef __ZONE_APP_MANAGER__
#define __ZONE_APP_MANAGER__

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager {

/**
 * This class provides APIs to create/remove and manage names.
 */

class ZoneAppManager {
public:
    ZoneAppManager(PolicyControlContext& ctxt);
    ~ZoneAppManager();

    //application information
    struct AppInfo {
        std::string zone, id, locale, package;
        std::string icon, label;
        int componentType;
        bool isNoDisplayed, isTaskManaged;

        REFLECTABLE(zone, id, locale, package,
                    icon, label,
                    componentType,
                    isNoDisplayed, isTaskManaged);
    };

    AppInfo get(const std::string& name, const std::string& appid);
    std::vector<AppInfo> getList(const std::string& name);

    //app control for homescreen
    int launch(const std::string& name, const std::string& appid);

    //app control for task manager
    int resume(const std::string& name, const std::string& appid);
    int terminate(const std::string& name, const std::string& appid);
    bool isRunning(const std::string& name, const std::string& appid);

private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __ZONE_APP_MANAGER__

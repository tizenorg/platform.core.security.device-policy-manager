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

#ifndef __ZONE_PACKAGE_MANAGER__
#define __ZONE_PACKAGE_MANAGER__

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager {

/**
 * This class provides APIs to control packages in the zones.
 */

class ZonePackageManager {
public:
    ZonePackageManager(PolicyControlContext& ctxt);
    ~ZonePackageManager();

    //package information
    struct PkgInfo {
        std::string zone, id;
        std::string type, icon, label, version;
        bool isSystem, isRemovable, isPreload;

        REFLECTABLE(zone, id,
                    type, icon, label, version,
                    isSystem, isRemovable, isPreload);
    };

    PkgInfo get(const std::string& name, const std::string& pkgid);
    std::vector<PkgInfo> getList(const std::string& name);

    //package manager request
    int install(const std::string& name, const std::string& pkgpath);
    int uninstall(const std::string& name, const std::string& pkgid);

private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __ZONE_PACKAGE_MANAGER__

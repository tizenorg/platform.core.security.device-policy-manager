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
#include "zone/package-manager.hxx"

namespace DevicePolicyManager {

ZonePackageManager::ZonePackageManager(PolicyControlContext& ctx)
    : context(ctx)
{
}

ZonePackageManager::~ZonePackageManager()
{
}

ZonePackageManager::PkgInfo ZonePackageManager::get(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<ZonePackageManager::PkgInfo>("ZonePackageManager::get", name, pkgid);
    } catch (runtime::Exception& e) {}
    return ZonePackageManager::PkgInfo();
}

std::vector<std::string> ZonePackageManager::getIdList(const std::string& zone)
{
    try {
        return context->methodCall<std::vector<std::string>>("ZonePackageManager::getIdList", zone);
    } catch (runtime::Exception& e) {}
    return std::vector<std::string>();;
}

int ZonePackageManager::install(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePackageManager::install", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePackageManager::uninstall(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePackageManager::uninstall", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

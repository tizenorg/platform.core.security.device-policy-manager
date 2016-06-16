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
#include "zone/package-proxy.hxx"

namespace DevicePolicyManager {

ZonePackageProxy::ZonePackageProxy(PolicyControlContext& ctx)
    : context(ctx)
{
}

ZonePackageProxy::~ZonePackageProxy()
{
}

ZonePackageProxy::PackageInfo ZonePackageProxy::getPackageInfo(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<ZonePackageProxy::PackageInfo>("ZonePackageProxy::getPackageInfo", name, pkgid);
    } catch (runtime::Exception& e) {}
    return ZonePackageProxy::PackageInfo();
}

int ZonePackageProxy::createIterator(const std::string& zone)
{
    try {
        return context->methodCall<int>("ZonePackageProxy::createIterator", zone);
    } catch (runtime::Exception& e) {}
    return -1;
}

ZonePackageProxy::PackageInfo ZonePackageProxy::getIteratorValue(int iterator)
{
    try {
        return context->methodCall<ZonePackageProxy::PackageInfo>("ZonePackageProxy::getIteratorValue", iterator);
    } catch (runtime::Exception& e) {}
    return ZonePackageProxy::PackageInfo();
}

bool ZonePackageProxy::nextIterator(int iterator)
{
    try {
        return context->methodCall<bool>("ZonePackageProxy::nextIterator", iterator);
    } catch (runtime::Exception& e) {}
    return false;
}

int ZonePackageProxy::destroyIterator(int iterator)
{
    try {
        return context->methodCall<int>("ZonePackageProxy::destroyIterator", iterator);
    } catch (runtime::Exception& e) {}
    return -1;
}

int ZonePackageProxy::install(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePackageProxy::install", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ZonePackageProxy::uninstall(const std::string& name, const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ZonePackageProxy::uninstall", name, pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

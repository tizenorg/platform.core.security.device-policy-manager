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

#include "policy-client.h"

#include "application.hxx"

namespace DevicePolicyManager {

ApplicationPolicy::ApplicationPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
}

ApplicationPolicy::~ApplicationPolicy()
{
}


int ApplicationPolicy::setApplicationPolicyInstallationMode(const bool mode)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::setApplicationPolicyInstallatioMode", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool ApplicationPolicy::getApplicationPolicyInstallationMode()
{
    try {
        return context->methodCall<int>("ApplicationPolicy::getApplicationPolicyInstallationMode");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::setApplicationPolicyUninstallationMode(const bool mode)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::setApplicationPolicyUninstallationMode", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool ApplicationPolicy::getApplicationPolicyUninstallationMode()
{
    try {
        return context->methodCall<int>("ApplicationPolicy::getApplicationPolicyUninstallationMode");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::setApplicationPolicyState(const std::string& appid, const int state)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::setApplicationPolicyState", appid, state);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::getApplicationPolicyState(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::getApplicationPolicyState", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> ApplicationPolicy::getInstalledPackageList()
{
    try {
        return context->methodCall<std::vector<std::string>>("ApplicationPolicy::getInstalledPackageList");
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}

bool ApplicationPolicy::isApplicationPolicyRunning(const std::string& appid)
{
    try {
        return context->methodCall<bool>("ApplicationPolicy::isApplicationPolicyRunning", appid);
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool ApplicationPolicy::isApplicationPolicyInstalled(const std::string& appid)
{
    try {
        return context->methodCall<bool>("ApplicationPolicy::isApplicationPolicyInstalled", appid);
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool ApplicationPolicy::isPackageInstalled(const std::string& pkgid)
{
    try {
        return context->methodCall<bool>("ApplicationPolicy::isPackageInstalled", pkgid);
    } catch (runtime::Exception& e) {
        return false;
    }
}

int ApplicationPolicy::installPackage(const std::string& pkgpath)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::installPackage", pkgpath);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::uninstallPackage(const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::uninstallPackage", pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::disableApplicationPolicy(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::disableApplicationPolicy", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::enableApplicationPolicy(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::enableApplicationPolicy", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::startApplicationPolicy(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::startApplicationPolicy", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::stopApplicationPolicy(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::stopApplicationPolicy", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::wipeApplicationPolicyData(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::wipeApplicationPolicyData", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::addPackageToBlacklist(const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::addPackageToBlacklist", pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int ApplicationPolicy::removePackageFromBlacklist(const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::removePackageFromBlacklist", pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int ApplicationPolicy::checkPackageIsBlacklisted(const std::string& pkgid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::checkPackageIsBlacklisted", pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

} // namespace DevicePolicyManager

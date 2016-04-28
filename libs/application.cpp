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


int ApplicationPolicy::setApplicationInstallationMode(const bool mode)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::setApplicationInstallatioMode", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool ApplicationPolicy::getApplicationInstallationMode()
{
    try {
        return context->methodCall<int>("ApplicationPolicy::getApplicationInstallationMode");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::setApplicationUninstallationMode(const bool mode)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::setApplicationUninstallationMode", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool ApplicationPolicy::getApplicationUninstallationMode()
{
    try {
        return context->methodCall<int>("ApplicationPolicy::getApplicationUninstallationMode");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::setApplicationState(const std::string& appid, const int state)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::setApplicationState", appid, state);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::getApplicationState(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::getApplicationState", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::disableApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::disableApplication", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::enableApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::enableApplication", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::wipeApplicationData(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::wipeApplicationData", appid);
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

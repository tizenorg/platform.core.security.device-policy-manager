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

int ApplicationPolicy::setApplicationState(const std::string& appid, int state)
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

int ApplicationPolicy::startApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::startApplication", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::stopApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::stopApplication", appid);
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

int ApplicationPolicy::setModeRestriction(int mode)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::setModeRestriction", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::unsetModeRestriction(int mode)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::unsetModeRestriction", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::getModeRestriction()
{
    try {
        return context->methodCall<int>("ApplicationPolicy::getModeRestriction");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::addPrivilegeToBlacklist(int type, const std::string& privilege)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::addPrivilegeToBlacklist", type, privilege);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::removePrivilegeFromBlacklist(int type, const std::string& privilege)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::removePrivilegeFromBlacklist", type, privilege);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int ApplicationPolicy::checkPrivilegeIsBlacklisted(int type, const std::string& privilege)
{
    try {
        return context->methodCall<int>("ApplicationPolicy::checkPrivilegeIsBlacklisted", type, privilege);
    } catch (runtime::Exception& e) {
        return -1;
    }
}
} // namespace DevicePolicyManager

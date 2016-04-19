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

Application::Application(PolicyControlContext& ctxt) :
    context(ctxt)
{
}

Application::~Application()
{
}


int Application::setApplicationInstallationMode(const bool mode)
{
    try {
        return context->methodCall<int>("Application::setApplicationInstallatioMode", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Application::getApplicationInstallationMode()
{
    try {
        return context->methodCall<int>("Application::getApplicationInstallationMode");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::setApplicationUninstallationMode(const bool mode)
{
    try {
        return context->methodCall<int>("Application::setApplicationUninstallationMode", mode);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Application::getApplicationUninstallationMode()
{
    try {
        return context->methodCall<int>("Application::getApplicationUninstallationMode");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::setApplicationState(const std::string& appid, const int state)
{
    try {
        return context->methodCall<int>("Application::setApplicationState", appid, state);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::getApplicationState(const std::string& appid)
{
    try {
        return context->methodCall<int>("Application::getApplicationState", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

std::vector<std::string> Application::getInstalledPackageList()
{
    try {
        return context->methodCall<std::vector<std::string>>("Application::getInstalledPackageList");
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}

bool Application::isApplicationRunning(const std::string& appid)
{
    try {
        return context->methodCall<bool>("Application::isApplicationRunning", appid);
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool Application::isApplicationInstalled(const std::string& appid)
{
    try {
        return context->methodCall<bool>("Application::isApplicationInstalled", appid);
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool Application::isPackageInstalled(const std::string& pkgid)
{
    try {
        return context->methodCall<bool>("Application::isPackageInstalled", pkgid);
    } catch (runtime::Exception& e) {
        return false;
    }
}

int Application::installPackage(const std::string& pkgpath)
{
    try {
        return context->methodCall<int>("Application::installPackage", pkgpath);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::uninstallPackage(const std::string& pkgid)
{
    try {
        return context->methodCall<int>("Application::uninstallPackage", pkgid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::disableApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("Application::disableApplication", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::enableApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("Application::enableApplication", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::startApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("Application::startApplication", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::stopApplication(const std::string& appid)
{
    try {
        return context->methodCall<int>("Application::stopApplication", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Application::wipeApplicationData(const std::string& appid)
{
    try {
        return context->methodCall<int>("Application::wipeApplicationData", appid);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

} // namespace DevicePolicyManager

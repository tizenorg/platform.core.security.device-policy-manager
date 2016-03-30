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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include "application.hxx"

#include "packman.h"
#include "launchpad.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

Application::Application(PolicyControlContext& ctxt) :
    context(ctxt)
{
    rmi::Service& manager = context.getServiceManager();

    manager.registerNonparametricMethod(this, (bool)(Application::getApplicationInstallationMode)());
    manager.registerNonparametricMethod(this, (bool)(Application::getApplicationUninstallationMode)());
    manager.registerNonparametricMethod(this, (std::vector<std::string>)(Application::getInstalledPackageList)());

    manager.registerParametricMethod(this, (int)(Application::setApplicationInstallationMode)(bool));
    manager.registerParametricMethod(this, (int)(Application::setApplicationUninstallationMode)(bool));
    manager.registerParametricMethod(this, (bool)(Application::isPackageInstalled)(std::string));
    manager.registerParametricMethod(this, (bool)(Application::isApplicationInstalled)(std::string));
    manager.registerParametricMethod(this, (bool)(Application::isApplicationRunning)(std::string));
    manager.registerParametricMethod(this, (int)(Application::installPackage)(std::string));
    manager.registerParametricMethod(this, (int)(Application::uninstallPackage)(std::string));
    manager.registerParametricMethod(this, (int)(Application::disableApplication)(std::string));
    manager.registerParametricMethod(this, (int)(Application::enableApplication)(std::string));
    manager.registerParametricMethod(this, (int)(Application::getApplicationState)(std::string));
    manager.registerParametricMethod(this, (int)(Application::setApplicationState)(std::string, int));
    manager.registerParametricMethod(this, (bool)(Application::isApplicationInstallationEnabled)(std::string));
    manager.registerParametricMethod(this, (bool)(Application::isApplicationUninstallationEnabled)(std::string));
    manager.registerParametricMethod(this, (int)(Application::startApplication)(std::string));
    manager.registerParametricMethod(this, (int)(Application::stopApplication)(std::string));
    manager.registerParametricMethod(this, (int)(Application::wipeApplicationData)(std::string));
}

Application::~Application()
{
}

int Application::setApplicationInstallationMode(const bool mode)
{
    return 0;
}

bool Application::getApplicationInstallationMode()
{
    return true;
}

int Application::setApplicationUninstallationMode(const bool mode)
{
    return 0;
}

bool Application::getApplicationUninstallationMode()
{
    return true;
}

std::vector<std::string> Application::getInstalledPackageList()
{
    try {
        return PackageManager::getInstalledPackageList(context.getServiceManager().getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the devioce");
        return std::vector<std::string>();
    }
}

bool Application::isApplicationInstallationEnabled(const std::string& appid)
{
        return true;
}

bool Application::isApplicationUninstallationEnabled(const std::string& appid)
{
    return true;
}

bool Application::isApplicationInstalled(const std::string& appid)
{
    try {
        AppInfo appInfo(appid, context.getServiceManager().getPeerUid());

        return true;
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool Application::isApplicationRunning(const std::string& appid)
{
    Launchpad launchpad(context.getServiceManager().getPeerUid());
    return launchpad.instantiated(appid);
}

bool Application::isPackageInstalled(const std::string& pkgid)
{
    try {
        PackageInfo pkgInfo(pkgid);

        return true;
    } catch (runtime::Exception& e) {
        return false;
    }
}


int Application::installPackage(const std::string& pkgpath)
{
    try {
        PackageManager::installPackage(pkgpath, context.getServiceManager().getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception in Package Id");
        return -1;
    }

    return 0;
}

int Application::uninstallPackage(const std::string& pkgid)
{
    try {
        PackageManager::uninstallPackage(pkgid, context.getServiceManager().getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception in Package Id");
        return -1;
    }

    return 0;
}

int Application::disableApplication(const std::string& appid)
{
    try {
        Launchpad launchpad(context.getServiceManager().getPeerUid());
        if (launchpad.instantiated(appid)) {
            launchpad.terminate(appid);
            // Notify user that the app hass terminated due to the policy
        }

        PackageManager::deactivatePackage(appid, context.getServiceManager().getPeerUid());
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int Application::enableApplication(const std::string& appid)
{
    try {
        PackageManager::activatePackage(appid, context.getServiceManager().getPeerUid());
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int Application::getApplicationState(const std::string& appid)
{
    return true;
}

int Application::setApplicationState(const std::string& appid, const int state)
{
    return true;
}

int Application::startApplication(const std::string& appid)
{
    Launchpad launchpad(context.getServiceManager().getPeerUid());
    if (launchpad.launch(appid) < 0) {
        ERROR("Failed to start device encryption");
        return -1;
    }

    return 0;
}

int Application::stopApplication(const std::string& appid)
{
    Launchpad launchpad(context.getServiceManager().getPeerUid());
    if (launchpad.instantiated(appid)) {
        launchpad.terminate(appid);
    }

    return 0;
}

int Application::wipeApplicationData(const std::string& appid)
{
    try {
        PackageManager::wipePackageData(appid, context.getServiceManager().getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception in Package Id");
        return -1;
    }

    return 0;
}
Application applicationPolicy(Server::instance());

} // namespace DevicePolicyManager

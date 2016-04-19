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
    context.registerNonparametricMethod(this, (bool)(Application::getApplicationInstallationMode)());
    context.registerNonparametricMethod(this, (bool)(Application::getApplicationUninstallationMode)());
    context.registerNonparametricMethod(this, (std::vector<std::string>)(Application::getInstalledPackageList)());

    context.registerParametricMethod(this, (int)(Application::setApplicationInstallationMode)(bool));
    context.registerParametricMethod(this, (int)(Application::setApplicationUninstallationMode)(bool));
    context.registerParametricMethod(this, (bool)(Application::isPackageInstalled)(std::string));
    context.registerParametricMethod(this, (bool)(Application::isApplicationInstalled)(std::string));
    context.registerParametricMethod(this, (bool)(Application::isApplicationRunning)(std::string));
    context.registerParametricMethod(this, (int)(Application::installPackage)(std::string));
    context.registerParametricMethod(this, (int)(Application::uninstallPackage)(std::string));
    context.registerParametricMethod(this, (int)(Application::disableApplication)(std::string));
    context.registerParametricMethod(this, (int)(Application::enableApplication)(std::string));
    context.registerParametricMethod(this, (int)(Application::getApplicationState)(std::string));
    context.registerParametricMethod(this, (int)(Application::setApplicationState)(std::string, int));
    context.registerParametricMethod(this, (int)(Application::startApplication)(std::string));
    context.registerParametricMethod(this, (int)(Application::stopApplication)(std::string));
    context.registerParametricMethod(this, (int)(Application::wipeApplicationData)(std::string));
    context.registerParametricMethod(this, (int)(Application::addPackageToBlacklist)(std::string));
    context.registerParametricMethod(this, (int)(Application::removePackageFromBlacklist)(std::string));
    context.registerParametricMethod(this, (int)(Application::checkPackageIsBlacklisted)(std::string));
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
        PackageManager& packman = PackageManager::instance();
        return packman.getInstalledPackageList(context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the devioce");
        return std::vector<std::string>();
    }
}

bool Application::isApplicationInstalled(const std::string& appid)
{
    try {
        AppInfo appInfo(appid, context.getPeerUid());

        return true;
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool Application::isApplicationRunning(const std::string& appid)
{
    Launchpad launchpad(context.getPeerUid());
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
        PackageManager& packman = PackageManager::instance();
        packman.installPackage(pkgpath, context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception in Package Id");
        return -1;
    }

    return 0;
}

int Application::uninstallPackage(const std::string& pkgid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        packman.uninstallPackage(pkgid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception in Package Id");
        return -1;
    }

    return 0;
}

int Application::disableApplication(const std::string& appid)
{
    try {
        Launchpad launchpad(context.getPeerUid());
        if (launchpad.instantiated(appid)) {
            launchpad.terminate(appid);
            // Notify user that the app hass terminated due to the policy
        }
        PackageManager& packman = PackageManager::instance();
        packman.deactivatePackage(appid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int Application::enableApplication(const std::string& appid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        packman.activatePackage(appid, context.getPeerUid());
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
    Launchpad launchpad(context.getPeerUid());
    if (launchpad.launch(appid) < 0) {
        ERROR("Failed to start device encryption");
        return -1;
    }

    return 0;
}

int Application::stopApplication(const std::string& appid)
{
    Launchpad launchpad(context.getPeerUid());
    if (launchpad.instantiated(appid)) {
        launchpad.terminate(appid);
    }

    return 0;
}

int Application::wipeApplicationData(const std::string& appid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        packman.wipePackageData(appid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception on wiping package data");
        return -1;
    }

    return 0;
}


int Application::addPackageToBlacklist(const std::string& pkgid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        packman.addPackageToBlacklist(pkgid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception on adding package to blacklist: " + pkgid);
        return -1;
    }

    return 0;
}

int Application::removePackageFromBlacklist(const std::string& pkgid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        packman.removePackageFromBlacklist(pkgid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception on removing package to blacklist: " + pkgid);
        return -1;
    }

    return 0;
}

int Application::checkPackageIsBlacklisted(const std::string& pkgid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        return packman.checkPackageIsBlacklisted(pkgid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception on checking package package blacklist: " + pkgid);
        return -1;
    }
}

Application applicationPolicy(Server::instance());

} // namespace DevicePolicyManager

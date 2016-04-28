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

#include "policy-helper.h"
#include "packman.h"
#include "launchpad.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

ApplicationPolicy::ApplicationPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
    context.registerNonparametricMethod(this, (bool)(ApplicationPolicy::getApplicationInstallationMode)());
    context.registerNonparametricMethod(this, (bool)(ApplicationPolicy::getApplicationUninstallationMode)());
    context.registerNonparametricMethod(this, (std::vector<std::string>)(ApplicationPolicy::getInstalledPackageList)());

    context.registerParametricMethod(this, (int)(ApplicationPolicy::setApplicationInstallationMode)(bool));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::setApplicationUninstallationMode)(bool));
    context.registerParametricMethod(this, (bool)(ApplicationPolicy::isPackageInstalled)(std::string));
    context.registerParametricMethod(this, (bool)(ApplicationPolicy::isApplicationInstalled)(std::string));
    context.registerParametricMethod(this, (bool)(ApplicationPolicy::isApplicationRunning)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::installPackage)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::uninstallPackage)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::disableApplication)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::enableApplication)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::getApplicationState)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::setApplicationState)(std::string, int));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::startApplication)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::stopApplication)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::wipeApplicationData)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::addPackageToBlacklist)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::removePackageFromBlacklist)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::checkPackageIsBlacklisted)(std::string));

    context.createNotification("package-installation-mode");
    context.createNotification("package-uninstallation-mode");
}

ApplicationPolicy::~ApplicationPolicy()
{
}

int ApplicationPolicy::setApplicationInstallationMode(const bool mode)
{
    SetPolicyAllowed(context, "package-installation-mode", mode);

    return 0;
}

bool ApplicationPolicy::getApplicationInstallationMode()
{
    return IsPolicyAllowed(context, "package-installation-mode");
}

int ApplicationPolicy::setApplicationUninstallationMode(const bool mode)
{
    SetPolicyAllowed(context, "package-uninstallation-mode", mode);
    return 0;
}

bool ApplicationPolicy::getApplicationUninstallationMode()
{
    return IsPolicyAllowed(context, "package-uninstallation-mode");
}

std::vector<std::string> ApplicationPolicy::getInstalledPackageList()
{
    try {
        PackageManager& packman = PackageManager::instance();
        return packman.getPackageList(context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the devioce");
    }
    return std::vector<std::string>();
}

bool ApplicationPolicy::isApplicationInstalled(const std::string& appid)
{
    try {
        AppInfo appInfo(appid, context.getPeerUid());

        return true;
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool ApplicationPolicy::isApplicationRunning(const std::string& appid)
{
    Launchpad launchpad(context.getPeerUid());
    return launchpad.isRunning(appid);
}

bool ApplicationPolicy::isPackageInstalled(const std::string& pkgid)
{
    try {
        PackageInfo pkgInfo(pkgid);

        return true;
    } catch (runtime::Exception& e) {
        return false;
    }
}


int ApplicationPolicy::installPackage(const std::string& pkgpath)
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

int ApplicationPolicy::uninstallPackage(const std::string& pkgid)
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

int ApplicationPolicy::disableApplication(const std::string& appid)
{
    try {
        Launchpad launchpad(context.getPeerUid());
        if (launchpad.isRunning(appid)) {
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

int ApplicationPolicy::enableApplication(const std::string& appid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        packman.activatePackage(appid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int ApplicationPolicy::getApplicationState(const std::string& appid)
{
    return true;
}

int ApplicationPolicy::setApplicationState(const std::string& appid, const int state)
{
    return true;
}

int ApplicationPolicy::startApplication(const std::string& appid)
{
    try {
        Launchpad launchpad(context.getPeerUid());
        launchpad.launch(appid);
     } catch (runtime::Exception& e) {
        ERROR("Failed to start device encryption");
        return -1;
    }

    return 0;
}

int ApplicationPolicy::stopApplication(const std::string& appid)
{
    Launchpad launchpad(context.getPeerUid());
    if (launchpad.isRunning(appid)) {
        launchpad.terminate(appid);
    }

    return 0;
}

int ApplicationPolicy::wipeApplicationData(const std::string& appid)
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


int ApplicationPolicy::addPackageToBlacklist(const std::string& pkgid)
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

int ApplicationPolicy::removePackageFromBlacklist(const std::string& pkgid)
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

int ApplicationPolicy::checkPackageIsBlacklisted(const std::string& pkgid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        return packman.checkPackageIsBlacklisted(pkgid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Exception on checking package package blacklist: " + pkgid);
        return -1;
    }
}

ApplicationPolicy applicationPolicy(Server::instance());

} // namespace DevicePolicyManager

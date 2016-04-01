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

ApplicationPolicy::ApplicationPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
    context.registerNonparametricMethod(this, (bool)(ApplicationPolicy::getApplicationPolicyInstallationMode)());
    context.registerNonparametricMethod(this, (bool)(ApplicationPolicy::getApplicationPolicyUninstallationMode)());
    context.registerNonparametricMethod(this, (std::vector<std::string>)(ApplicationPolicy::getInstalledPackageList)());

    context.registerParametricMethod(this, (int)(ApplicationPolicy::setApplicationPolicyInstallationMode)(bool));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::setApplicationPolicyUninstallationMode)(bool));
    context.registerParametricMethod(this, (bool)(ApplicationPolicy::isPackageInstalled)(std::string));
    context.registerParametricMethod(this, (bool)(ApplicationPolicy::isApplicationPolicyInstalled)(std::string));
    context.registerParametricMethod(this, (bool)(ApplicationPolicy::isApplicationPolicyRunning)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::installPackage)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::uninstallPackage)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::disableApplicationPolicy)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::enableApplicationPolicy)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::getApplicationPolicyState)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::setApplicationPolicyState)(std::string, int));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::startApplicationPolicy)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::stopApplicationPolicy)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::wipeApplicationPolicyData)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::addPackageToBlacklist)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::removePackageFromBlacklist)(std::string));
    context.registerParametricMethod(this, (int)(ApplicationPolicy::checkPackageIsBlacklisted)(std::string));
}

ApplicationPolicy::~ApplicationPolicy()
{
}

int ApplicationPolicy::setApplicationPolicyInstallationMode(const bool mode)
{
    return 0;
}

bool ApplicationPolicy::getApplicationPolicyInstallationMode()
{
    return true;
}

int ApplicationPolicy::setApplicationPolicyUninstallationMode(const bool mode)
{
    return 0;
}

bool ApplicationPolicy::getApplicationPolicyUninstallationMode()
{
    return true;
}

std::vector<std::string> ApplicationPolicy::getInstalledPackageList()
{
    try {
        PackageManager& packman = PackageManager::instance();
        return packman.getInstalledPackageList(context.getPeerUid());
    } catch (runtime::Exception& e) {
        ERROR("Failed to retrieve package info installed in the devioce");
        return std::vector<std::string>();
    }
}

bool ApplicationPolicy::isApplicationPolicyInstalled(const std::string& appid)
{
    try {
        AppInfo appInfo(appid, context.getPeerUid());

        return true;
    } catch (runtime::Exception& e) {
        return false;
    }
}

bool ApplicationPolicy::isApplicationPolicyRunning(const std::string& appid)
{
    Launchpad launchpad(context.getPeerUid());
    return launchpad.instantiated(appid);
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

int ApplicationPolicy::disableApplicationPolicy(const std::string& appid)
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

int ApplicationPolicy::enableApplicationPolicy(const std::string& appid)
{
    try {
        PackageManager& packman = PackageManager::instance();
        packman.activatePackage(appid, context.getPeerUid());
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int ApplicationPolicy::getApplicationPolicyState(const std::string& appid)
{
    return true;
}

int ApplicationPolicy::setApplicationPolicyState(const std::string& appid, const int state)
{
    return true;
}

int ApplicationPolicy::startApplicationPolicy(const std::string& appid)
{
    Launchpad launchpad(context.getPeerUid());
    if (launchpad.launch(appid) < 0) {
        ERROR("Failed to start device encryption");
        return -1;
    }

    return 0;
}

int ApplicationPolicy::stopApplicationPolicy(const std::string& appid)
{
    Launchpad launchpad(context.getPeerUid());
    if (launchpad.instantiated(appid)) {
        launchpad.terminate(appid);
    }

    return 0;
}

int ApplicationPolicy::wipeApplicationPolicyData(const std::string& appid)
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

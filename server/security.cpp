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
#include <sys/reboot.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

#include <aul.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <dd-deviced.h>
#include <dd-control.h>

#include "security.hxx"

#include "syspopup.h"
#include "launchpad.h"
#include "process.h"
#include "filesystem.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

namespace {

const std::string APPID_DEVICE_ENCRYPTION = "org.tizen.ode";
const std::string APPID_LOCKSCREEN = "org.tizen.lockscreen";

const std::string PROG_POWEROFF = "/usr/sbin/poweroff";

} // namespace

SecurityPolicy::SecurityPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
    ctxt.registerNonparametricMethod(this, (int)(SecurityPolicy::lockoutDevice));
    ctxt.registerNonparametricMethod(this, (int)(SecurityPolicy::lockoutScreen));
    ctxt.registerNonparametricMethod(this, (int)(SecurityPolicy::reboot));
    ctxt.registerNonparametricMethod(this, (int)(SecurityPolicy::powerOffDevice));
    ctxt.registerNonparametricMethod(this, (bool)(SecurityPolicy::isInternalStorageEncrypted));
    ctxt.registerNonparametricMethod(this, (bool)(SecurityPolicy::isExternalStorageEncrypted));
    ctxt.registerParametricMethod(this, (int)(SecurityPolicy::setInternalStorageEncryption)(bool));
    ctxt.registerParametricMethod(this, (int)(SecurityPolicy::setExternalStorageEncryption)(bool));
    ctxt.registerParametricMethod(this, (std::vector<std::string>)(SecurityPolicy::getFileNamesOnDevice)(std::string));
    ctxt.registerParametricMethod(this, (std::vector<std::string>)(SecurityPolicy::getFileNamesWithAttributes)(std::string));
}

SecurityPolicy::~SecurityPolicy()
{
}

int SecurityPolicy::lockoutDevice()
{
    return 0;
}

int SecurityPolicy::lockoutScreen()
{
    try {
        Launchpad launchpad(context.getPeerUid());
        launchpad.launch(APPID_LOCKSCREEN);
   } catch (runtime::Exception &e) {
        ERROR("Failed to launch lockscreen: " + APPID_LOCKSCREEN);
        return -1;
    }

    return 0;
}

int SecurityPolicy::reboot()
{
    if (::reboot(RB_AUTOBOOT) < 0) {
        ERROR("Failed to reboot device");
        return -1;
    }

    return 0;
}

int SecurityPolicy::powerOffDevice()
{
    int ret = 0;

    if (::deviced_call_predef_action("poweroff", 0) < 0) {
        WARN("Poweroff failed in deviced... Start poweroff fallback");
        runtime::Process proc(PROG_POWEROFF);
        ret = proc.execute();
        if (ret != 0) {
            ERROR("Failed to turn off device power");
            ret = -1;
        }
    }

    return ret;
}

int SecurityPolicy::setInternalStorageEncryption(const bool encrypt)
{
    try {
        Bundle bundle;
        bundle.add("viewtype", encrypt ? "ENCRYPT_DEVICE" : "DECRYPT_DEVICE");

        Launchpad launchpad(context.getPeerUid());
        launchpad.launch(APPID_DEVICE_ENCRYPTION, bundle);
    } catch (runtime::Exception& e) {
        ERROR("Failed to start device encryption");
        return -1;
    }

    return 0;
}

bool SecurityPolicy::isInternalStorageEncrypted()
{
    INFO("Not implemented yet");
    return false;
}

int SecurityPolicy::setExternalStorageEncryption(const bool encrypt)
{
    int status;

    if (::vconf_get_int(VCONFKEY_SYSMAN_MMC_STATUS, &status) != 0) {
        ERROR("Failed to read VCONFKEY_SYSMAN_MMC_STATUS");
        return -1;
    }

    if (status != VCONFKEY_SYSMAN_MMC_MOUNTED) {
        WARN("MMC was not mounted");
        return -1;
    }

    int wasEncrypted;
    if (::vconf_get_bool(VCONFKEY_SETAPPL_MMC_ENCRYPTION_STATUS_BOOL, &wasEncrypted) != 0) {
        ERROR("Failed to read VCONFKEY_SETAPPL_MMC_ENCRYPTION_STATUS_BOOL");
        return -1;
    }

    try {
        Bundle bundle;
        bundle.add("viewtype", encrypt ? "ENCRYPT_SD_CARD" : "DECRYPT_SD_CARD");

        Launchpad launchpad(context.getPeerUid());
        launchpad.launch(APPID_DEVICE_ENCRYPTION, bundle);
    } catch (runtime::Exception& e) {
        ERROR("Failed to start sd card encryption");
        return -1;
    }

    return 0;
}

bool SecurityPolicy::isExternalStorageEncrypted()
{
    return false;
}

std::vector<std::string> SecurityPolicy::getFileNamesOnDevice(const std::string& path)
{
    std::vector<std::string> files;

    try {
        runtime::DirectoryIterator iter(path), end;
        while (iter != end) {
            files.push_back(iter->getPath());
            ++iter;
        }
    } catch (runtime::Exception& e) {
        ERROR("Error on directory iteration");
        // Return empty list
        return std::vector<std::string>();
    }

    return files;
}

std::vector<std::string> SecurityPolicy::getFileNamesWithAttributes(const std::string& path)
{
    std::vector<std::string> files;

    try {
        runtime::DirectoryIterator iter(path), end;
        while (iter != end) {
            files.push_back(iter->getPath());
            ++iter;
        }
    } catch (runtime::Exception& e) {
        ERROR("Error on directory iteration");
        // Return empty list
        return std::vector<std::string>();
    }

    return files;
}

SecurityPolicy securityPolicy(Server::instance());

} // namespace DevicePolicyManager

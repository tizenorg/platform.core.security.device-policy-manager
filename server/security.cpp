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
#include <bundle.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <dd-deviced.h>
#include <dd-control.h>
#include <syspopup_caller.h>

#include "security.hxx"

#include "process.h"
#include "filesystem.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

namespace {

const std::string APPID_DEVICE_ENCRYPTION = "org.tizen.ode";
const std::string APPID_LOCKSCREEN = "org.tizen.lockscreen";

const std::string PROG_POWEROFF = "/usr/sbin/poweroff";
const std::string PROG_FACTORY_RESET = "/usr/bin/factory-reset";

void WipeExternalMemoryCallback(int ret, void *user_data)
{
    mmc_contents *mmc_data = static_cast<mmc_contents*>(user_data);
    delete mmc_data;
}

int LaunchMMCSysPopup(const std::string& prop)
{
    bundle *b = ::bundle_create();
    if (b == NULL) {
        ERROR("Failed to create bundle: out of memory");
        return -1;
    }

    ::bundle_add_str(b, "_SYSPOPUP_CONTENT_", prop.c_str());
    int ret = ::syspopup_launch((char *)"mmc-syspopup", b);
    ::bundle_free(b);

    if (ret < 0) {
        ret = -1;
    }

    return ret;
}

} // namespace

Security::Security(PolicyControlContext& ctxt) :
    context(ctxt)
{
    rmi::Service& manager = context.getServiceManager();

    manager.registerNonparametricMethod(this, (int)(Security::lockoutDevice));
    manager.registerNonparametricMethod(this, (int)(Security::lockoutScreen));
    manager.registerNonparametricMethod(this, (int)(Security::reboot));
    manager.registerNonparametricMethod(this, (int)(Security::powerOffDevice));
    manager.registerNonparametricMethod(this, (bool)(Security::isInternalStorageEncrypted));
    manager.registerNonparametricMethod(this, (bool)(Security::isExternalStorageEncrypted));
    manager.registerParametricMethod(this, (int)(Security::wipeData)(int));
    manager.registerParametricMethod(this, (int)(Security::setInternalStorageEncryption)(bool));
    manager.registerParametricMethod(this, (int)(Security::setExternalStorageEncryption)(bool));
    manager.registerParametricMethod(this, (std::vector<std::string>)(Security::getFileNamesOnDevice)(std::string));
    manager.registerParametricMethod(this, (std::vector<std::string>)(Security::getFileNamesWithAttributes)(std::string));
}

Security::~Security()
{
}

int Security::lockoutDevice()
{
    return 0;
}

int Security::lockoutScreen()
{
    if (::aul_launch_app(APPID_LOCKSCREEN.c_str(), nullptr) < 0) {
        ERROR("Failed to launch lockscreen: " + APPID_LOCKSCREEN);
        return -1;
    }

    return 0;
}

int Security::wipeData(const int id)
{
    int ret = 0;
    if (id & WIPE_INTERNAL_MEMORY) {
        runtime::Process proc(PROG_FACTORY_RESET);
        if (proc.execute() != 0) {
            ERROR("Failed to launch factory-reset");
            ret = -1;
        }
    }

    if (id & WIPE_EXTERNAL_MEMORY) {
        int status;
        if (::vconf_get_int(VCONFKEY_SYSMAN_MMC_STATUS, &status) != 0) {
            ERROR("Failed to get mmc status");
            return -1;
        }

        if (status != VCONFKEY_SYSMAN_MMC_MOUNTED) {
            ERROR("MMC is not working");
            return -1;
        }

        mmc_contents *mmc_data = new(std::nothrow) mmc_contents();
        if (mmc_data == nullptr) {
            ERROR("Failed to construct mmc_contents: out of memory");
            return -1;
        }

        mmc_data->mmc_cb = WipeExternalMemoryCallback;
        mmc_data->user_data = nullptr;

        if (::deviced_request_format_mmc(mmc_data) < 0) {
            ERROR("Failed to format MMC");
            delete mmc_data;
            return -1;
        }
    }

    return ret;
}

int Security::reboot()
{
    if (::reboot(RB_AUTOBOOT) < 0) {
        ERROR("Failed to reboot device");
        return -1;
    }

    return 0;
}

int Security::powerOffDevice()
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

int Security::setInternalStorageEncryption(const bool encrypt)
{
    bundle *b = ::bundle_create();
    if (b == nullptr) {
        ERROR("Failed to create bundle: out of memory");
        return -1;
    }

    ::bundle_add_str(b, "viewtype", "encryption");
    int ret = ::aul_launch_app(APPID_DEVICE_ENCRYPTION.c_str(), b);
    ::bundle_free(b);

    if (ret < 0) {
        ERROR("Failed to launch syspopup for device encryption");
        return -1;
    }

    return 0;
}

bool Security::isInternalStorageEncrypted()
{
    INFO("Not implemented yet");
    return false;
}

int Security::setExternalStorageEncryption(const bool encrypt)
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

    std::string prop = encrypt ? "odeencrypt" : "odedecrypt";
    if (LaunchMMCSysPopup(prop) < 0) {
        ERROR("Failed to launch syspop for external storage decryption");
        return -1;
    }

    return 0;
}

bool Security::isExternalStorageEncrypted()
{
    return false;
}

std::vector<std::string> Security::getFileNamesOnDevice(const std::string& path)
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

std::vector<std::string> Security::getFileNamesWithAttributes(const std::string& path)
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

Security securityPolicy(Server::instance());

} // namespace DevicePolicyManager

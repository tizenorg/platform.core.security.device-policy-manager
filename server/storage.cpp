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
 #include <vconf.h>
 #include <vconf-keys.h>
 #include <dd-deviced.h>
 #include <dd-control.h>

#include "storage.hxx"

#include "policy-helper.h"

#include "exception.h"
#include "process.h"
#include "filesystem.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

namespace {

const std::string PROG_FACTORY_RESET = "/usr/bin/factory-reset";

void WipeExternalMemoryCallback(int ret, void *user_data)
{
    std::cout << "WipeExternalMemoryCallback was called" << std::endl;
}

} // namespace

StoragePolicy::StoragePolicy(PolicyControlContext& ctx) :
    context(ctx)
{
	context.registerParametricMethod(this, (int)(StoragePolicy::wipeData)(int));
}

StoragePolicy::~StoragePolicy()
{
}

int StoragePolicy::wipeData(int id)
{
    int ret = 0;
    if (id & WIPE_INTERNAL_STORAGE) {
        runtime::Process proc(PROG_FACTORY_RESET);
        if (proc.execute() != 0) {
            ERROR("Failed to launch factory-reset");
            ret = -1;
        }
    }

    if (id & WIPE_EXTERNAL_STORAGE) {
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

StoragePolicy storagePolicy(Server::instance());

} //namespace DevicePolicyManager

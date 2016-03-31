/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include <sys/stat.h>
#include <sys/mount.h>
#include <vconf.h>
#include <vconf-keys.h>
#include <dd-deviced.h>
#include <dd-control.h>
#include <glib.h>

#include "misc.hxx"
#include "audit/logger.h"

#define SETTING_MEMORY_STATUS_MMC_PATH		"/opt/storage/sdcard"
#define SETTING_MEMORY_STATUS_MMC_PARENT_PATH	"/opt/storage/sdcard/.."

#define DPM_LOCATION_DISABLED 0
#define DPM_MMC_MOUNTED 1
#define DPM_MMC_UMOUNTED 0

namespace DevicePolicyManager {
namespace {
static int checkMMCStatus(void)
{
	int ret = 0;
	struct stat parent_stat, mount_stat;

	ret = ::stat(SETTING_MEMORY_STATUS_MMC_PATH, &mount_stat);
	if (ret != 0) {
		ERROR("failed to get stat : /opt/storage/sdcard");
		return -1;
	}

	ret = ::stat(SETTING_MEMORY_STATUS_MMC_PARENT_PATH, &parent_stat);
	if (ret != 0) {
		ERROR("failed to get stat : /opt/stroage/sdcard/..");
		return -1;
	}

	if (mount_stat.st_dev == parent_stat.st_dev)
		return 0;
	else
		return 1;
}

static void callbackMMCResult(int result, void *p_userdata)
{
	return ;
}

static gboolean callbackMMCUmountTimer(void *data)
{
	struct mmc_contents *lp_mmc = NULL;
	int vconf_status = -1, mmc_status = -1;
	int ret = 0;

	ret = ::vconf_get_int(VCONFKEY_SYSMAN_MMC_STATUS, &vconf_status);
	if (ret != 0) {
		ERROR("failed to get vconf mmc status");
		return FALSE;
	}

	if (vconf_status == VCONFKEY_SYSMAN_MMC_REMOVED || vconf_status == VCONFKEY_SYSMAN_MMC_INSERTED_NOT_MOUNTED) {
		ERROR("MMC device is ejected or not mounted");
		return FALSE;
	}

	mmc_status = checkMMCStatus();
	if (mmc_status == DPM_MMC_MOUNTED) {
		lp_mmc = (struct mmc_contents *)g_malloc(sizeof(struct mmc_contents));
		lp_mmc->mmc_cb = callbackMMCResult;
		ret = ::deviced_request_unmount_mmc(lp_mmc, MNT_FORCE);
		if (ret == -1) {
			ERROR("failed to request to umount");
			return FALSE;
		}
		g_free(lp_mmc);
	} else if (mmc_status == DPM_MMC_UMOUNTED) {
		WARN("MMC was umounted");
		return FALSE;
	} else {
		ERROR("failed to check status of mmc");
		return FALSE;
	}

	return TRUE;
}

static gboolean callbackMMCMountTimer(void *data)
{
	struct mmc_contents *lp_mmc = NULL;
	int vconf_status = -1, mmc_status = -1;
	int ret = 0;

	ret = ::vconf_get_int(VCONFKEY_SYSMAN_MMC_STATUS, &vconf_status);
	if (ret != 0) {
		ERROR("failed to get vconf mmc status");
		return FALSE;
	}

	if (vconf_status != VCONFKEY_SYSMAN_MMC_INSERTED_NOT_MOUNTED) {
		ERROR("MMC was mounted");
		return FALSE;
	}

	mmc_status = checkMMCStatus();
	if (mmc_status == DPM_MMC_UMOUNTED) {
		lp_mmc = (struct mmc_contents *)g_malloc(sizeof(struct mmc_contents));
		lp_mmc->mmc_cb = callbackMMCResult;
		ret = ::deviced_request_mount_mmc(lp_mmc);
		if (ret == -1) {
			ERROR("failed to request to mount");
			return FALSE;
		}
		g_free(lp_mmc);
	} else if (mmc_status == DPM_MMC_MOUNTED) {
		WARN("MMC was mounted");
		return FALSE;
	} else {
		ERROR("failed to check status of mmc");
		return FALSE;
	}

	return TRUE;
}
} // namespace

Misc::Misc(PolicyControlContext &ctxt)
	: context(ctxt)
{
	rmi::Service &manager = context.getServiceManager();

	manager.registerParametricMethod(this, (int)(Misc::setAllowCamera)(bool));
	manager.registerNonparametricMethod(this, (bool)(Misc::getAllowCamera));
	manager.registerParametricMethod(this, (int)(Misc::setAllowMicrophone)(bool));
	manager.registerNonparametricMethod(this, (bool)(Misc::getAllowMicrophone));
	manager.registerParametricMethod(this, (int)(Misc::setAllowLocation)(bool));
	manager.registerNonparametricMethod(this, (bool)(Misc::getAllowLocation));
	manager.registerParametricMethod(this, (int)(Misc::setAllowSDCard)(bool));
	manager.registerNonparametricMethod(this, (bool)(Misc::getAllowSDCard));
}

Misc::~Misc()
{
}

int Misc::setAllowCamera(const bool enable)
{
	return 0;
}

bool Misc::getAllowCamera()
{
	bool ret = true;
	return ret;
}

int Misc::setAllowMicrophone(const bool enable)
{
	return 0;
}

bool Misc::getAllowMicrophone()
{
	bool ret = true;
	return ret;
}

int Misc::setAllowLocation(const bool enable)
{
	int ret = 0;
	int status = -1;

	ret = ::vconf_get_int(VCONFKEY_LOCATION_USE_MY_LOCATION, &status);
	if (ret != 0) {
		ERROR("failed to get the status of location");
		return ret;
	}

	if (status != DPM_LOCATION_DISABLED) {
		ret = ::vconf_set_int(VCONFKEY_LOCATION_USE_MY_LOCATION, DPM_LOCATION_DISABLED);
		if (ret != 0)
			ERROR("failed to set key(VCONFKEY_LOACTION_USE_MY_LOACATION)");

		ret = ::vconf_set_int(VCONFKEY_LOCATION_ENABLED, DPM_LOCATION_DISABLED);
		if (ret != 0)
			ERROR("failed to set key(VCONFKEY_LOCATION_ENABLED)");

		ret = ::vconf_set_int(VCONFKEY_LOCATION_NETWORK_ENABLED, DPM_LOCATION_DISABLED);
		if (ret != 0)
			ERROR("failed to set key(VCONFKEY_LOCATION_NETWORK_ENABLED)");
	}

	return ret;
}

bool Misc::getAllowLocation()
{
	bool ret = true;
	return ret;
}

int Misc::setAllowSDCard(const bool enable)
{
	int ret = 0;

	if (enable == false)
		::g_timeout_add(5000, callbackMMCUmountTimer, NULL);
	else if (enable == true)
		::g_timeout_add(5000, callbackMMCMountTimer, NULL);

	ret = ::deviced_mmc_control(enable);
	if (ret != 0)
		ERROR("failed to control MMC device");

	return ret;
}

bool Misc::getAllowSDCard()
{
	int ret = true;
	return ret;
}
} // namespace DevicePolicyManager

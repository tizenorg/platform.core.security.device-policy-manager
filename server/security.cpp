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

#include "privilege.h"
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
	ctxt.registerNonparametricMethod(this, DPM_PRIVILEGE_LOCK, (int)(SecurityPolicy::lockoutScreen));
	ctxt.registerNonparametricMethod(this, "", (int)(SecurityPolicy::isInternalStorageEncrypted));
	ctxt.registerNonparametricMethod(this, "", (int)(SecurityPolicy::isExternalStorageEncrypted));
	ctxt.registerParametricMethod(this, DPM_PRIVILEGE_SECURITY, (int)(SecurityPolicy::setInternalStorageEncryption)(bool));
	ctxt.registerParametricMethod(this, DPM_PRIVILEGE_SECURITY, (int)(SecurityPolicy::setExternalStorageEncryption)(bool));
}

SecurityPolicy::~SecurityPolicy()
{
}

int SecurityPolicy::lockoutScreen()
{
	try {
		Launchpad launchpad(context.getPeerUid());
		if (launchpad.isRunning(APPID_LOCKSCREEN)) {
			launchpad.resume(APPID_LOCKSCREEN);
			return 0;
		}

		launchpad.launch(APPID_LOCKSCREEN);
   } catch (runtime::Exception &e) {
		ERROR("Failed to launch lockscreen: " + APPID_LOCKSCREEN);
		return -1;
	}

	return 0;
}

int SecurityPolicy::setInternalStorageEncryption(bool encrypt)
{
	std::string policy = context.getPolicy("internal-storage-encryption");
	if ((encrypt == true) && (policy == "encrypted")) {
		return 0;
	} else if ((encrypt == false) && (policy == "decrypted")) {
		return 0;
	}

	try {
		Bundle bundle;
		bundle.add("viewtype", encrypt ? "ENCRYPT_DEVICE" : "DECRYPT_DEVICE");

		Launchpad launchpad(context.getPeerUid());
		if (launchpad.isRunning(APPID_DEVICE_ENCRYPTION)) {
			launchpad.resume(APPID_DEVICE_ENCRYPTION);
			return 0;
		}

		launchpad.launch(APPID_DEVICE_ENCRYPTION, bundle);
	} catch (runtime::Exception& e) {
		ERROR("Failed to start device encryption");
		return -1;
	}

	return 0;
}

int SecurityPolicy::isInternalStorageEncrypted()
{
	std::string policy = context.getPolicy("internal-storage-encryption");
	if (policy == "encrypted") {
		return true;
	}

	return false;
}

int SecurityPolicy::setExternalStorageEncryption(bool encrypt)
{
	std::string policy = context.getPolicy("external-storage-encryption");
	if ((encrypt == true) && (policy == "encrypted")) {
		return 0;
	} else if ((encrypt == false) && (policy == "decrypted")) {
		return 0;
	}

	try {
		Bundle bundle;
		bundle.add("viewtype", encrypt ? "ENCRYPT_SD_CARD" : "DECRYPT_SD_CARD");

		Launchpad launchpad(context.getPeerUid());
		if (launchpad.isRunning(APPID_DEVICE_ENCRYPTION)) {
			launchpad.resume(APPID_DEVICE_ENCRYPTION);
			return 0;
		}

		launchpad.launch(APPID_DEVICE_ENCRYPTION, bundle);
	} catch (runtime::Exception& e) {
		ERROR("Failed to start sd card encryption");
		return -1;
	}

	return 0;
}

int SecurityPolicy::isExternalStorageEncrypted()
{
	std::string policy = context.getPolicy("external-storage-encryption");
	if (policy == "encrypted") {
		return true;
	}

	return false;
}

SecurityPolicy securityPolicy(Server::instance());

} // namespace DevicePolicyManager

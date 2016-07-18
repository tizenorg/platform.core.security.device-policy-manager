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

#include <stdio.h>
#include <stdlib.h>		// atoi
#include <unistd.h>		// getopt
#include <pwd.h>		// struct pwd

#include <memory>

#include <dpm/administration.h>

static void printUsage()
{
	printf("Usage: dpm-admin-cli [option] [package name] -u [user name]\n");
	printf("Options:\n");
	printf("  -r: *Mandatory* registration admin client with package name and uid\n");
	printf("  -d: *Mandatory* deregistration admin client with package name and uid\n");
	printf("  -u: *Mandatory* user name of admin client\n");
	printf("  -h: print usage\n");
	printf("\n");
}

static int registAdminClient(const char* pkgName, const int uid)
{
	if (pkgName == NULL)
		return -1;

	int ret;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return -1;
	}

	ret = 0;
	if (dpm_admin_register_client(handle, pkgName, uid) != DPM_ERROR_NONE) {
		printf("Failed to register admin client. (name:%s, uid:%d)\n", pkgName, uid);
		ret = -1;
	}

	dpm_manager_destroy(handle);

	return ret;
}

static int deregistAdminClient(const char* pkgName, const int uid)
{
	if (pkgName == NULL)
		return -1;

	int ret;
	device_policy_manager_h handle;

	handle = dpm_manager_create();
	if (handle == NULL) {
		printf("Failed to create client handle\n");
		return -1;
	}

	ret = 0;
	if (dpm_admin_deregister_client(handle, pkgName, uid) != DPM_ERROR_NONE) {
		printf("Failed to deregister admin client. (name:%s, uid:%d)\n", pkgName, uid);
		ret = -1;
	}

	dpm_manager_destroy(handle);

	return ret;
}

static int getUID(const char* userName)
{
	struct passwd pwd, *result;
	int bufsize;

	if (userName == NULL)
		return -1;

	bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
	if (bufsize == -1)
		bufsize = 16384;

	std::unique_ptr<char[]> buf(new char[bufsize]);
	::getpwnam_r(userName, &pwd, buf.get(), bufsize, &result);
	if (result == NULL) {
		printf("User %s doesn't exist\n", userName);
		return -1;
	}

	return (int)result->pw_uid;
}

enum OptVal
{
	DPM_ADMIN_CLI_REGISTER,
	DPM_ADMIN_CLI_DEREGISTER,
	DPM_ADMIN_CLI_NULL,
};

int main(int argc, char *argv[])
{
	int dpmOpt = 0;
	OptVal optVal = DPM_ADMIN_CLI_NULL;
	int uid = 0;
	char* userName = 0;
	char* pkgName = NULL;

	if (argc < 5) {
		printUsage();
		return -1;
	}

	opterr = 0;

	while ((dpmOpt = getopt(argc, argv, "r:d:u:h")) != -1) {
		switch(dpmOpt) {
		case 'r':
			optVal = DPM_ADMIN_CLI_REGISTER;
			pkgName = optarg;
			break;
		case 'd':
			optVal = DPM_ADMIN_CLI_DEREGISTER;
			pkgName = optarg;
			break;
		case 'u':
			userName = optarg;
			break;
		case 'h':
			printUsage();
			break;
		default:
			printUsage();
			return -1;
			break;
		}
	}

	uid = getUID(userName);
	if (uid == -1) {
		printUsage();
		return -1;
	}

	if (optVal == DPM_ADMIN_CLI_REGISTER)
		return registAdminClient(pkgName, uid);
	else if (optVal == DPM_ADMIN_CLI_DEREGISTER)
		return deregistAdminClient(pkgName, uid);
	else {
		printUsage();
		return -1;
	}

	return 0;
}
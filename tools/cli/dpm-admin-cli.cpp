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

#include <dpm/administration.h>

static void printUsage()
{
	printf("Usage: dpm-admin-cli [option] [package name] -u [uid]\n");
	printf("Options:\n");
	printf("  -r: *Mandatory* registration admin client with package name and uid\n");
	printf("  -d: *Mandatory* deregistration admin client with package name and uid\n");
	printf("  -u: *Mandatory* uid of admin client\n");
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
			uid = atoi(optarg);
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

	// Our uid always shouldn't be 0.
	// Also, this exception can handle the wrong input value.
	if (uid == 0) {
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
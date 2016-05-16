/*
 * Tizen Zone Setup-Wizard application
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "zone-setup.h"

#define TARGET_ZONE "workspace"
#define PROVISION_DATA "/manifest.xml"
#define PROVISION_COMPLETE "/.completed"

static char *__get_zone_metadata(void)
{
	FILE *fp = NULL;
	char *res_path = NULL;
	char *metadata = NULL;
	char metadata_path[PATH_MAX] = "\0";
	long fsize = 0;
	int ret = -1;

	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get resource path");
		return NULL;
	}
	snprintf(metadata_path, PATH_MAX, "%s%s", res_path, "data/ZoneManifest.xml");
	free(res_path);

	fp = fopen(metadata_path, "r");
	if (fp != NULL) {
		if (fseek(fp, 0, SEEK_END) == -1)
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to fseek");

		fsize = ftell(fp);
		if (fsize == -1) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to get file size");
			fclose(fp);
			return NULL;
		}

		metadata = malloc(fsize + 1);
		if (metadata == NULL) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to allocate memory");
			fclose(fp);
			return NULL;
		}
		memset(metadata, 0, fsize + 1);
		if (fseek(fp, 0, SEEK_SET) == -1) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to fseek");
			fclose(fp);
			free(metadata);
			return NULL;
		}

		ret = fread(metadata, fsize, 1, fp);
		if (ret < 0) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to read metadata file");
			fclose(fp);
			free(metadata);
			return NULL;
		}
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to open metadata file");
		return NULL;
	}

	fclose(fp);

	return metadata;
}

static void __create_zone_manifest(char *metadata, char *manifest, const char *zone_name)
{
	while (1) {
		int index = 0;
		int len = 0;
		char tmp[PATH_MAX] = "\0";
		char *found = strstr(metadata, TARGET_ZONE);

		if (!found) {
			strncpy(manifest, metadata, strlen(metadata)+1);
			break;
		}

		index = found - metadata;
		strncpy(tmp, metadata, index);
		strncat(tmp, zone_name, strlen(zone_name));
		len = strlen(found)-strlen(TARGET_ZONE);
		if (len > 0) {
			strncat(tmp, metadata+index+strlen(TARGET_ZONE), len);
		}

		strncpy(manifest, tmp, strlen(tmp)+1);
		strncpy(metadata, manifest, strlen(manifest)+1);
	}

	return ;
}

int _send_zone_provision_data(const char *zone_name, const char *target_path)
{
	FILE *fp = NULL;
	char *metadata = NULL;
	char manifest[PATH_MAX] = "\0";
	char data_path[PATH_MAX] = "\0";

	metadata = __get_zone_metadata();
	if (metadata == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get metadata");
		return -1;
	}

	__create_zone_manifest(metadata, manifest, zone_name);
	free(metadata);

	snprintf(data_path, strlen(target_path)+strlen(PROVISION_DATA)+1, "%s%s", target_path, PROVISION_DATA);
	fp = fopen(data_path, "w");
	if (fp != NULL) {
		fwrite(manifest, 1, strlen(manifest), fp);
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to create provisioning file");
		return -1;
	}
	fclose(fp);

	snprintf(data_path, strlen(target_path)+strlen(PROVISION_COMPLETE)+1, "%s%s", target_path, PROVISION_COMPLETE);
	fp = fopen(data_path, "w");
	if (fp == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to touch completed file");
		return -1;
	}

	fclose(fp);
	return 0;
}

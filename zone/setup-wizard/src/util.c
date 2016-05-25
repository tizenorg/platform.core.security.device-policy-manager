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

#define ZONE_METADATA_PATH "data/ZoneManifest.xml"

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
	snprintf(metadata_path, PATH_MAX, "%s%s", res_path, ZONE_METADATA_PATH);
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

int _send_zone_create_request(void)
{
	char *metadata = NULL;

	metadata = __get_zone_metadata();
	if (metadata == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "Failed to get zone metadata");
		return -1;
	}

	/* [TBD] */

	return 0;
}

int _send_zone_remove_request(void)
{
	/* [TBD] */

	return 0;
}

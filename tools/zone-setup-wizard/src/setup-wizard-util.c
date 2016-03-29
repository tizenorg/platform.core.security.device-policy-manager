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

static char *get_zone_metadata(void)
{
	FILE *fp = NULL;
	char *res_path = NULL;
	char *metadata = NULL;
	char metadata_path[1024] = "\0";
	long fsize = 0;
	char buf[1024] = "\0";

	res_path = app_get_resource_path();
	if (res_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get resource path");
		return NULL;
	}
	snprintf(metadata_path, 1024, "%s%s", res_path, "data/.sample-BundleManifest.xml");

	fp = fopen(metadata_path, "r");
	if (fp != NULL) {
		if (fseek(fp, 0, SEEK_END) == -1) dlog_print(DLOG_ERROR, LOG_TAG, "failed to fseek");
		fsize = ftell(fp);
		if (fsize == -1) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to get file size");
			return NULL;
		}

		metadata = malloc(fsize + 1);
		if (metadata == NULL) {
			dlog_print(DLOG_ERROR, LOG_TAG, "failed to allocate memory");
			return NULL;
		}
		memset(metadata, 0, fsize + 1);
		if (fseek(fp, 0, SEEK_SET) == -1) dlog_print(DLOG_ERROR, LOG_TAG, "failed to fseek");

		while (fgets(buf, 1024, fp) != NULL) strncat(metadata, buf, strlen(buf));
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to open metadata file");
		return NULL;
	}

	fclose(fp);
	free(res_path);

	return metadata;
}

int create_zone_manifest(void)
{
	FILE *fp = NULL;
	char *metadata = NULL;
	char *data_path = NULL;
	char manifest_path[1024] = "\0";

	metadata = get_zone_metadata();
	if (metadata == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get metadata");
		return -1;
	}

	data_path = app_get_data_path();
	if (data_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get data path");
		free(metadata);
		return -1;
	}
	snprintf(manifest_path, 1024, "%s%s", data_path, "bundle-manifest.xml");

	/* [TBD] : change MANIFEST_PATH to manifest_path */
	fp = fopen(MANIFEST_PATH, "w");
	if (fp != NULL) {
		fwrite(metadata, 1, sizeof(metadata), fp);
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to create manifest file");
		free(metadata);
		return -1;
	}

	fclose(fp);
	free(data_path);
	free(metadata);
	return 0;
}

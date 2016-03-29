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
	snprintf(metadata_path, PATH_MAX, "%s%s", res_path, "data/.sample-BundleManifest.xml");
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

static void __strrep(char *in, char **out, char *old, const char *new)
{
	char* temp;
	char* found = strstr(in, old);
	if(!found) {
		*out = malloc(strlen(in) + 1);
		strcpy(*out, in);
		return ;
	}

	int idx = found - in;

	*out = realloc(*out, strlen(in) - strlen(old) + strlen(new) + 1);
	strncpy(*out, in, idx);
	strcpy(*out + idx, new);
	strcpy(*out + idx + strlen(new), in + idx + strlen(old));

	temp = malloc(idx+strlen(new)+1);
	strncpy(temp,*out,idx+strlen(new));
	temp[idx + strlen(new)] = '\0';

	__strrep(found + strlen(old), out, old, new);
	temp = realloc(temp, strlen(temp) + strlen(*out) + 1);
	strcat(temp,*out);
	free(*out);
	*out = temp;
	return ;
}

static char *__create_zone_manifest(char *metadata, const char *zone_name)
{
	char *manifest = NULL;

	__strrep(metadata, &manifest, "ZoneName", zone_name);

	return manifest;
}

int _send_zone_manifest(const char *zone_name)
{
	FILE *fp = NULL;
	char *metadata = NULL;
	char *manifest = NULL;
	char *data_path = NULL;
	char manifest_path[PATH_MAX] = "\0";

	metadata = __get_zone_metadata();
	if (metadata == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get metadata");
		return -1;
	}

	manifest = __create_zone_manifest(metadata, zone_name);
	free(metadata);

	data_path = app_get_data_path();
	if (data_path == NULL) {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to get data path");
		free(manifest);
		return -1;
	}
	snprintf(manifest_path, PATH_MAX, "%s%s", data_path, "bundle-manifest.xml");
	free(data_path);

	/* [TBD] : change MANIFEST_PATH to manifest_path */
	fp = fopen(MANIFEST_PATH, "w");
	if (fp != NULL) {
		fwrite(manifest, 1, strlen(manifest), fp);
	} else {
		dlog_print(DLOG_ERROR, LOG_TAG, "failed to create manifest file");
		free(manifest);
		return -1;
	}

	fclose(fp);
	free(manifest);
	return 0;
}

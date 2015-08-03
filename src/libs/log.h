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

#ifndef __DPM_LOG_H__
#define __DPM_LOG_H__

#include<stdio.h>

enum dpm_log_type {
	ERROR,
	DEBUG,
	INFO,
	WARN
};

#ifndef LOG_TAG
#define LOG_TAG "DEVICE_POLICY"
#endif

#ifndef __MODULE__
#include <string.h>
#define __MODULE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#if 1
#define ERROR(...) do {fprintf(stderr, "DEVICE_POLICY : " __VA_ARGS__); fprintf(stderr, "\n");} while(0)
#define DEBUG(...) do {fprintf(stderr, "DEVICE_POLICY : " __VA_ARGS__); fprintf(stderr, "\n");} while(0)
#define WARN(...) do {fprintf(stderr, "DEVICE_POLICY : " __VA_ARGS__); fprintf(stderr, "\n");} while(0)
#define INFO(...) do {fprintf(stderr, "DEVICE_POLICY : " __VA_ARGS__); fprintf(stderr, "\n");} while(0)
#else
#define ERROR(fmg, arg...) do { dpm_log(ERROR, LOG_TAG, "[%s:%s](%d)>" fmt, __MODULE__, __func__, __LINE__, ##arg);} while(0)

#define DEBUG(fmg, arg...) do { dpm_log(DEBUG, LOG_TAG, "[%s:%s](%d)>" fmt, __MODULE__, __func__, __LINE__, ##arg);} while(0)

#define WARN(fmg, arg...) do { dpm_log(WARN, LOG_TAG, "[%s:%s](%d)>" fmt, __MODULE__, __func__, __LINE__, ##arg);} while(0)

#define INFO(fmg, arg...) do { dpm_log(INFO, LOG_TAG, "[%s:%s](%d)>" fmt, __MODULE__, __func__, __LINE__, ##arg);} while(0)

#endif

#endif

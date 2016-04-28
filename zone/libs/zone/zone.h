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

#ifndef __ZONE_ZONE_H__
#define __ZONE_ZONE_H__

#include <tizen.h>

/**
 * @file zone.h
 * @brief This file defines common data types required to zone APIs.
 */

#ifndef ZONE_API
#define ZONE_API __attribute__((visibility("default")))
#endif // API

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief       Enumeration of device policy API errors
 * @since_tizen 3.0
 */
typedef enum {
    ZONE_ERROR_NONE                 = TIZEN_ERROR_NONE,                 /**< The operation was successful */
    ZONE_ERROR_INVALID_PARAMETER    = TIZEN_ERROR_INVALID_PARAMETER,    /**< Invalid parameter */
    ZONE_ERROR_CONNECTION_REFUSED   = TIZEN_ERROR_CONNECTION_REFUSED,   /**< Connection refused */
    ZONE_ERROR_TIMED_OUT            = TIZEN_ERROR_TIMED_OUT,            /**< Time out */
    ZONE_ERROR_PERMISSION_DENIED    = TIZEN_ERROR_PERMISSION_DENIED,    /**< Access privilege is not sufficient */
    ZONE_ERROR_NOT_SUPPORTED        = TIZEN_ERROR_NOT_SUPPORTED,        /**< Operation is not supported */
    ZONE_ERROR_NO_SUCH_FILE         = TIZEN_ERROR_NO_SUCH_FILE,         /**< No such file or directory */
    ZONE_ERROR_FILE_EXISTS          = TIZEN_ERROR_FILE_EXISTS,          /**< File exists */
    ZONE_ERROR_OUT_OF_MEMORY        = TIZEN_ERROR_OUT_OF_MEMORY         /**< Out of memory */
} zone_error_type_e;


#ifdef __cplusplus
}
#endif

#endif /* __ZONE_ZONE_H__ */

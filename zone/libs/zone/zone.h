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
 * @addtogroup  CAPI_ZONE_ZONE_MODULE
 * @{
 */

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

/**
 * @brief       The zone manager handle
 * @details     The zone manager  handle is an abstraction of the
 *              logical connection between the zone manager and it's client.
 *              The zone manager handle must be created by using
 *              zone_manager_create() before attempting to use almost any of
 *              the zone related APIs, and it should be freed when interaction
 *              with the zone manager is no longer required.
 *              To release the handle, use zone_manager_destroy().
 * @since_tizen 3.0
 * @see         zone_manager_create()
 * @see         zone_manager_destroy()
 */
typedef void* zone_manager_h;

/**
 * @brief       Creates the zone manager handle.
 * @details     This API creates zone manager handle required to
 *              the zone related APIs.
 *              This API is also used to verify whether caller is authorized
 *              or not.
 * @since_tizen 3.0
 * @param[out]  handle The zone manager handle
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_CONNECTION_REFUSED Connection refused
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @see         zone_manager_destroy()
 * @see         get_last_result()
 */
ZONE_API int zone_manager_create(zone_manager_h *handle);

/**
 * @brief       Releases the zone manager handle.
 * @details     This API must be called if interaction with the zone manager is
 *              no longer required.
 * @since_tizen 3.0
 * @param[in]   handle The zone manager handle
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by zone_manager_create()
 * @see         zone_manager_create()
 */
ZONE_API int zone_manager_destroy(zone_manager_h handle);

/**
 * @brief       Called when a zone raises a event.
 * @since_tizen 3.0
 * @param[in]   name The zone name
 * @param[in]   object The object name triggered the event
 * @param[in]   user_data The user data passed from zone_manager_add_event_cb
 * @see         zone_manager_add_event_cb()
 * @see         zone_manager_remove_event_cb()
 */
typedef void(*zone_event_cb)(const char* name, const char* object, void *user_data);

/**
 * @brief       Adds zone event callback.
 * @details     This API can be used to receive events sent by zone
 *              The callback specified to this function is automatically called when
 *              the zone is created or removed.
 * @since_tizen 3.0
 * @param[in]   context The zone_manager handle
 * @param[in]   event The event name to receive
 * @param[in]   callback The event callback
 * @param[in]   user_data The user data passed to the callback function
 * @param[out]  id Signal identifier
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by zone_manager_create().
 * @see         zone_manager_create()
 * @see         zone_manager_destroy()
 * @see         zone_manager_remove_event_cb()
 */
ZONE_API int zone_manager_add_event_cb(zone_manager_h handle,
                                      const char* event,
                                      zone_event_cb callback, void* user_data,
                                      int* id);

/**
 * @brief       Removes zone event callback.
 * @details     This API removes zone event callback
 * @since_tizen 3.0
 * @param[in]   context The zone manager handle
 * @param[in]   id Signal identifier
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         The context must be created by zone_manager_create().
 * @see         zone_manager_create()
 * @see         zone_manager_destroy()
 * @see         zone_manager_add_event_cb()
 */
ZONE_API int zone_manager_remove_event_cb(zone_manager_h handle, int id);

/**
 * @brief       Creates a new zone.
 * @details     This API creates a container. All file system objects neeeded
 *              will be also created. manifest XML passed by parameter will be
 *              used when the zone is running.
 * @since_tizen 3.0
 * @param[in]   handle The zone manager handle
 * @param[in]   name The zone name to be created
 * @param[in]   manifest The manifest XML to be used when the zone is runned.
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API
 * @pre         The handle must be created by zone_manager_create().
 * @see         zone_manager_create()
 * @see         zone_manager_destroy()
 * @see         zone_manager_destroy_zone()
 * @see         zone_manager_create_zone_iterator()
 */
ZONE_API int zone_manager_create_zone(zone_manager_h handle, const char* name, const char* manifest);

/**
 * @brief       Removes existing zone.
 * @details     This removes zone. All file system objects created for the zone
 *              will be also erased.
 * @since_tizen 3.0
 * @param[in]   handle The zone manager handle
 * @param[in]   name The zone name to be removed
 * @return      #DPM_ERROR_NONE on success, otherwise a negative value
 * @retval      #DPM_ERROR_NONE Successful
 * @retval      #DPM_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #DPM_ERROR_TIMED_OUT Time out
 * @retval      #DPM_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by zone_manager_create().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         zone_manager_create()
 * @see         zone_manager_destroy()
 * @see         zone_manager_create_zone()
 * @see         zone_manager_create_zone_iterator()
 */
ZONE_API int zone_manager_destroy_zone(zone_manager_h handle, const char* name);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZONE_ZONE_H__ */

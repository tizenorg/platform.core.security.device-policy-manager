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

#ifndef __CAPI_ZONE_APP_INFO_H__
#define __CAPI_ZONE_APP_INFO_H__

#include <app_info.h>
#include <app_control.h>
#include <app_manager.h>

#include <zone/zone.h>

/**
 * @file app-info.h
 * @brief This file provides APIs to get the information of applications in the zone
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup  CAPI_ZONE_APPLICATION_INFO_MODULE
 * @{
 */

/**
 * @brief       The zone application manager handle
 * @since_tizen 3.0
 * @see         zone_app_proxy_create()
 * @see         zone_app_proxy_destroy()
 */
typedef void* zone_app_proxy_h;

/**
 * @brief       Acquires the zone application manager handle
 * @details     This API acquires zone application manager handle required to call
 *              the zone application manager APIs.
 * @since_tizen 3.0
 * @param[in]   manager The zone manager handle
 * @param[in]   name The zone name
 * @param[out]  handle The zone app proxy handle
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_CONNECTION_REFUSED Connection refused
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @see         zone_manager_create()
 * @see         zone_app_proxy_create()
 * @see         get_last_result()
 */
ZONE_API int zone_app_proxy_create(zone_manager_h manager, const char* name, zone_app_proxy_h* handle);

/**
 * @brief       Releases the zone application manager handle
 * @details     This API must be called if interaction with the zone application
 *              manager handle is no longer required.
 * @since_tizen 3.0
 * @param[in]   context The device policy manager context
 * @param[in]   handle The zone application manager handle
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by zone_app_proxy_create()
 * @see         zone_app_proxy_create()
 */
ZONE_API int zone_app_proxy_destroy(zone_app_proxy_h handle);

/**
 * @brief       Gets the application information for the given application in the zone.
 * @details     This API gets application information handle required to get
 *              information of the application in the zone
 * @since_tizen 3.0
 * @param[in]   handle The zone application manager handle
 * @param[in]   appid The application ID
 * @return      Zone application handle on success, otherwise NULL
 * @remark      The specific error code can be obtained by using the
 *              get_last_result() method. Error codes are described in
 *              exception section.
 * @exception   #ZONE_ERROR_NONE No error
 * @exception   #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @exception   #ZONE_ERROR_TIMED_OUT Time out
 * @see         zone_app_proxy_create()
 * @see         zone_app_proxy_create()
 * @see         zone_application_release()
 * @see         application_manager_get_application_info()
 * @see         get_last_result()
 */
ZONE_API int zone_app_proxy_get_app_info(zone_app_proxy_h handle, const char* appid, app_info_h* app_info);

/**
 * @brief       Retrieves all the handles of the application in the zone.
 * @details     This API calls app_manager_app_info_cb() once for each application
 *              ID with traversing the installed application list in the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone application manager handle
 * @param[in]   callback The iteration callback function
 * @param[in]   user_data The user data passed to the callback function
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @pre         The handle must be created by zone_app_proxy_create().
 * @see         zone_app_proxy_create()
 * @see         zone_app_proxy_create()
 * @see         application_manager_foreach_app_info()
 */
ZONE_API int zone_app_proxy_foreach_app_info(zone_app_proxy_h handle, app_manager_app_info_cb callback, void *user_data);

/**
 * @brief       Launch the application located at the given path into the zone.
 * @details     Administrator can use this API to launch the application in the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone application manager handle
 * @param[in]   app_control The app_control handle
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @retval      #ZONE_ERROR_NO_SUCH_FILE No such application file
 * @retval      #ZONE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by zone_app_proxy_create().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         zone_app_proxy_create()
 * @see         zone_app_proxy_destroy()
 * @see         zone_app_proxy_resume()
 * @see         zone_app_proxy_terminate()
 * @see         zone_is_running_app()
 * @see         app_control_send_launch_request()
 */
ZONE_API int zone_app_proxy_send_launch_request(zone_app_proxy_h handle, app_control_h app_control);

/**
 * @brief       Terminate the application located at the given path into the zone.
 * @details     Administrator can use this API to terminate the application in
 *              the zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone application manager handle
 * @param[in]   appid The application ID to be terminated
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @retval      #ZONE_ERROR_NO_SUCH_FILE No such application file
 * @retval      #ZONE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by zone_app_proxy_create().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         zone_app_proxy_create()
 * @see         zone_app_proxy_destroy()
 * @see         zone_app_proxy_resume()
 * @see         zone_app_proxy_send_launch_request()
 * @see         zone_is_running_app()
 * @see         app_control_send_terminate_request()
 * @see         app_manager_send_terminate_bt_app()
 */
ZONE_API int zone_app_proxy_terminate(zone_app_proxy_h handle, const char* appid);

/**
 * @brief       Resume the application located at the given path into the zone.
 * @details     Administrator can use this API to resume the application in the
 *              zone.
 * @since_tizen 3.0
 * @param[in]   handle The zone application manager handle
 * @param[in]   appid The application ID to be resumed
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @retval      #ZONE_ERROR_NO_SUCH_FILE No such application file
 * @retval      #ZONE_ERROR_PERMISSION_DENIED The application does not have
 *              the privilege to call this API or the caller is not the owner
 *              of the zone
 * @pre         The handle must be created by zone_app_proxy_create().
 * @pre         The zone corresponding to the given name must be
 *              created before use of this API.
 * @see         zone_app_proxy_create()
 * @see         zone_app_proxy_destroy()
 * @see         zone_app_proxy_send_launch_request()
 * @see         zone_app_proxy_terminate()
 * @see         app_manager_resume_app()
 */
ZONE_API int zone_app_proxy_resume(zone_app_proxy_h handle, const char* appid);

/**
 * @brief       Checks whether the application in the zone is running.
 * @details     This API can be used to check whether the application in the zone
 *              is running.
 * @since_tizen 3.0
 * @param[in]   handle The zone application manager handle
 * @param[in]   appid The application ID
 * @param[out]  result true if the application is running,
 *              otherwise false if the application is not running
 * @return      #ZONE_ERROR_NONE on success, otherwise a negative value
 * @retval      #ZONE_ERROR_NONE Successful
 * @retval      #ZONE_ERROR_NO_DATA No such pacakge to check
 * @retval      #ZONE_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval      #ZONE_ERROR_TIMED_OUT Time out
 * @pre         The handle must be created by zone_app_proxy_create().
 * @see         zone_app_proxy_create()
 * @see         zone_app_proxy_destroy()
 * @see         zone_app_proxy_send_launch_request()
 * @see         zone_app_proxy_terminate()
 * @see         app_manager_is_running_app()
 */
ZONE_API int zone_app_proxy_is_running(zone_app_proxy_h handle, const char* appid, int* result);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __CAPI_ZONE_APP_INFO_H__ */

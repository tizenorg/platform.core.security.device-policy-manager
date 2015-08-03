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

#ifndef __DP_BUS_H__
#define __DP_BUS_H__

#include <gio/gio.h>

#define DP_BUS_ADDRESS "/tmp/.device-policy-manager"

typedef struct _dp_bus_client dp_bus_client;
struct _dp_bus_client {
	int uid;
	int pid;
	GDBusConnection *conn;	
	pthread_rwlock_t lock;
};

typedef struct _dp_bus_server dp_bus_server;
struct _dp_bus_server{
	int status;
	char *unix_path;
	int clients;
	GHashTable *client_map;
	GDBusServer *g_svc;
};

typedef struct _dp_bus_connection dp_bus_connection;
struct _dp_bus_connection{
	int status;
	GDBusConnection *g_conn;
};

dp_bus_server*  dp_bus_server_new(const char *address);

int dp_bus_server_finish(dp_bus_server *svc);
int dp_bus_server_start(dp_bus_server *svc);
int dp_bus_server_stop(dp_bus_server *svc);

int dp_bus_call_method(void);
int dp_bus_send_message(void);

dp_bus_connection *dp_bus_connect(const char *address);
int dp_bus_disconnect(dp_bus_connection *conn);

#endif

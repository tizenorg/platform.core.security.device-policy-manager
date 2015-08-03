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

#include<stdio.h>

#include<limits.h>
#include<errno.h>

#include "dp-bus.h"
#include "log.h"


dp_bus_server *dp_bus_server_new(const char *address)
{
	char path[PATH_MAX]; 
	dp_bus_server *svc;
	GDBusServer   *g_svc;
	GHashTable    *hashtbl;
	gchar *guid;
	int rc;
	GError *err = NULL;

	if( address == NULL)
		return NULL;

	if(access(address, F_OK) == 0)	{
		rc = unlink(address);
		if( rc == -1 ) {
			ERROR("Failed to remove old socket file");
			return NULL;
		}
	}

	if( rc == 0 ) 
		unlink(address);
	
	snprintf(path, PATH_MAX, "unix:path=%s", address);

	svc = (dp_bus_server *)malloc(sizeof(dp_bus_server));
	if( svc == NULL) 
		return NULL;

	svc->unix_path = strdup(path);
	guid = g_dbus_generate_guid();
	g_svc = g_dbus_server_new_sync(path, G_DBUS_SERVER_FLAGS_NONE, guid, NULL,
			NULL, &err);

	g_free(guid);
	if( g_svc == NULL) {
		ERROR("Failed to create dp_bus server %s", err->message);
		return NULL;
	}

	hashtbl = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_object_unref);

	svc->g_svc = g_svc;
	svc->clients = 0;
	svc->client_map = hashtbl;

	return svc;
}

static close_all_connections(dp_bus_server * svc)
{
	GHashTableIter iter;
	gpointer*conn;
	gpointer *cli;
	
	g_hash_table_iter_init(&iter, svc->client_map);
	while( g_hash_table_iter_next(&iter, &conn, &cli))
	{
		dp_bus_client *entity = (dp_bus_client *)cli;
		g_dbus_connection_close_sync(entity->conn, NULL, NULL);
	}
}


int dp_bus_server_finish(dp_bus_server *svc)
{
	GDBusServer *g_svc = svc->g_svc;

	close_all_connections(svc);

	g_dbus_server_stop(g_svc);

	g_object_unref(g_svc);

	free(svc);

	return 0;
}

static void __dp_bus_close_handler(
		GDBusConnection *conn,
		gboolean remote_vanished, 
		GError *err,
		gpointer user_data)
{
	dp_bus_server *svc = (dp_bus_server *)user_data;
	dp_bus_client *cli = (dp_bus_client *)g_hash_table_lookup(svc->client_map, conn);
	
	INFO("Client connection is closed");

	g_hash_table_steal(svc->client_map, conn);

	pthread_rwlock_destroy(&cli->lock);	
	free(cli);

	/* FIXME : It has to replace to g_dbus_connection_unregister_object */
	g_object_unref(conn);

	svc->clients--;

	return ;
}

static gboolean __dp_bus_accept_handler(
		GDBusServer *dbus_svc,
		GDBusConnection *conn,
		gpointer user_data)
{
	int pid;
	int uid;
	dp_bus_server *svc = (dp_bus_server *)user_data;
	GCredentials *gcred;
	GError *err = NULL;
	dp_bus_client *cli;

	/* Get peer Credential */
	gcred = g_dbus_connection_get_peer_credentials(conn);
	if( gcred == NULL ) {
		ERROR("Failed to get peer credentials");		
	}
	pid = g_credentials_get_unix_pid(gcred, &err);
	if( pid == -1) {
		WARN("Failed to get peer pid , error:%s", err->message);
	}
	err = NULL;
	uid = g_credentials_get_unix_user(gcred, &err);
	if( uid == -1) {
		WARN("Failed to get peer uid , error:%s", err->message);
	}

	cli = (dp_bus_client *)malloc(sizeof(dp_bus_client));
	if( cli == NULL ) {
		ERROR("Failed to memory allocation");
		return FALSE;
	}

	cli->pid = pid;
	cli->uid = uid;
	cli->conn = conn;
	pthread_rwlock_init(&cli->lock, NULL);
	
	g_hash_table_insert(svc->client_map, conn, cli);

	INFO("Establish new connection PID:%d UID:%d", pid, uid);

	/* Set up close handler for peer */
	g_signal_connect (conn, "closed", G_CALLBACK(__dp_bus_close_handler), svc);
	
	svc->clients++;

	/* FIXME : It has to replace to g_dbus_connection_register_object() */
	g_object_ref(conn);

	return TRUE;
}


int dp_bus_register_object_handler(void)
{

	return 0;
}
			
int dp_bus_unregister_object_handlers(void)
{

	return 0;
}


int dp_bus_server_start(dp_bus_server *svc)
{
	if(svc == NULL)
		return -1;
	
	/* Setup Peer Accept handler */
	g_signal_connect(svc->g_svc, "new-connection", G_CALLBACK(__dp_bus_accept_handler), svc);

	if( !g_dbus_server_is_active(svc->g_svc))
		g_dbus_server_start(svc->g_svc);

	return 0;
}

int dp_bus_server_stop(dp_bus_server *svc)
{

	return 0;
}

int dp_bus_call_method(void)
{

	return 0;
}

int dp_bus_send_message(void)
{
	return 0;
}

dp_bus_connection *dp_bus_connect(const char *address)
{
	char path[PATH_MAX]; 
	GDBusConnection *conn;
	GError *err = NULL;
	dp_bus_connection *dp_conn;

	snprintf(path, PATH_MAX, "unix:path=%s", address);

	conn = g_dbus_connection_new_for_address_sync(path, 
			G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT, NULL, NULL, &err);
	
	
	if( conn == NULL) {
		ERROR("Failed to g_dbus_connection_new_for_address_sync %s", err->message);
		return NULL;
	}
	
	dp_conn = (dp_bus_connection *)malloc(sizeof(dp_bus_connection));
	if( dp_conn == NULL) 
		return NULL;

	dp_conn->g_conn = conn;

	return dp_conn;
}

int dp_bus_disconnect(dp_bus_connection *conn)
{
	gboolean res;
	GError *err = NULL;

	res = g_dbus_connection_close_sync(conn->g_conn, NULL, &err);

	if( res == FALSE) {
		ERROR("Failed to close g_dbus_connection, %s", err->message);
		return -1;
	}

	return 0;
}

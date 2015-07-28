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

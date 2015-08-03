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

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include<gio/gio.h>
#include<gum-user.h>
#include<common/gum-user-types.h>

#include"log.h"


#define SYSTEM_BUS_ADDRESS "unix:path=/var/run/dbus/system_bus_socket"
#define LOGIND_DBUS_PATH   "org.freedesktop.login1.Manager" 
#define LOGIND_DBUS_INF    "/org/freedesktop/login1"

typedef struct dpm_admin * dpm_admin_t;
struct dpm_admin {
	const char *name;	
	int uid;
	int status;
};

/* Call LoginD SetUserLinger() Method */
static int setup_user_lingering(uid_t uid)
{
	GError *err = NULL;
	GDBusConnection *conn;
	GVariant *ret_val;
	GDBusConnectionFlags flags = G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT |
		G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION;

	conn = g_dbus_connection_new_for_address_sync(SYSTEM_BUS_ADDRESS, flags, NULL, NULL, &err);

	if( err ) {
		fprintf(stderr,"Failed to get dbus connection : %s", err->message);
		return -ENONET;
	}
	
	ret_val = g_dbus_connection_call_sync(conn, 
								LOGIND_DBUS_PATH,
								LOGIND_DBUS_INF,
								LOGIND_DBUS_PATH,
								"SetUserLinger",
								g_variant_new("(ubb)",uid, TRUE,FALSE),
								/* Uid, Enable/Disable, Polkit authorization */
								NULL,
								G_DBUS_CALL_FLAGS_NONE,
								-1,
								NULL,
								&err);

	/* FIXME : Check Return value here */

	g_dbus_connection_close_sync(conn, NULL, NULL);

	return 0;
}


int dpm_create_admin(const char *name, const char *password, dpm_admin_t *admin)
{
	dpm_admin_t new_admin;
	GumUser *gum_user;
	uid_t uid;
	int rc;

	if( name == NULL || password == NULL || admin == NULL)
		return -EINVAL;
	
	gum_user = gum_user_create_sync(FALSE);
	if( !gum_user ) {
		ERROR("Failed to gumd user allocation\n");
		return -1;
	}

	g_object_set(G_OBJECT(gum_user), "usertype", GUM_USERTYPE_NORMAL,
									"secret", password,
									"username", name,
									NULL);

	if( !gum_user_add_sync(gum_user)) {
		ERROR("Failed to gumd user add\n");
		g_object_unref(gum_user);
	}

	g_object_get(G_OBJECT(gum_user), "uid", &uid, NULL);

	INFO("Success to add new gumd user - name : %s, uid : %d\n", name, uid);
	
	rc = setup_user_lingering(uid);
	if( rc != 0 ){
		ERROR("Failed to user lingering");
		g_object_unref(gum_user);
	}
	
	new_admin = (dpm_admin_t)malloc(sizeof(struct dpm_admin));
	new_admin->name = strdup(name);
	new_admin->uid = uid;

	*admin = new_admin;

	return 0;
}

int dpm_get_admin(const char *name, dpm_admin_t admin)
{

	return 0;
}

int dpm_release_admin(dpm_admin_t *admin)
{

}

int dpm_destroy_admin(dpm_admin_t admin)
{
	//DisableLinger
    //RemoveUser

	return 0;
}


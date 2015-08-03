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
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <gio/gio.h>

#include "dp-bus.h"
#include "log.h"

static void child_handler(int sig)
{
	int pid;
	int status;

	if( sig != SIGCHLD)
		return ;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		INFO("Child process exited :%d, ret :%d", pid, status);
	}
	return;
}

static void signal_handler(int sig)
{
	ERROR("Signal: %d", sig);
	exit(EXIT_SUCCESS);
}

static int setup_signals(void)
{
	sigset_t mask;

	if (sigfillset(&mask) ||
			sigdelset(&mask, SIGILL) ||
			sigdelset(&mask, SIGCHLD) ||
			sigdelset(&mask, SIGSEGV) ||
			sigdelset(&mask, SIGBUS) ||
			sigdelset(&mask, SIGTERM) || sigprocmask(SIG_BLOCK, &mask, NULL)) {
		ERROR("ailed to set signal mask");
		return 1;
	}

	signal(SIGILL, signal_handler);
	signal(SIGCHLD, child_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGBUS, signal_handler);
	signal(SIGTERM, signal_handler);

	return 0;
}

int main(int argc, char *argv[])
{
	int rc;
	GMainLoop *loop;
	dp_bus_server *dp_svc;

	INFO("Main Thread started");

#if !GLIB_CHECK_VERSION (2, 36, 0)
	g_type_init();
#endif

	rc = setup_signals();
	if( rc != 0) 
		return 1;

	loop = g_main_loop_new(NULL, FALSE);
	if(loop == NULL) {
		return 1;
	}

	dp_svc = dp_bus_server_new(DP_BUS_ADDRESS);
	if( dp_svc == NULL )
		return 1;

	rc = dp_bus_server_start(dp_svc);
	if (rc != 0)
		return 1;

	g_main_loop_run(loop);

	rc = dp_bus_server_stop(dp_svc);
	
	return 0;
}


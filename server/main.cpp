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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

#include <iostream>
#include <stdexcept>

#include "server.h"

#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void signalHandler(int signum)
{
	exit(0);
}

static bool daemonize(void)
{
	pid_t pid;

	pid = ::fork();
	if (pid == -1) {
		std::cerr << "Fork failed" << std::endl;
		return false;
	} else if (pid != 0) {
		exit(EXIT_SUCCESS);
	}

	if (::setsid() == -1) {
		return false;
	}

	if (::chdir("/") == -1) {
		return false;
	}

	int fd = ::open("/dev/null", O_RDWR);
	if (fd == -1) {
		return false;
	}

	for (int i = 0; i < 3; i++) {
		if (::dup2(fd, i) == -1) {
			::close(fd);
			return false;
		}
	}

	::close(fd);

	return true;
}

static void usage(const std::string& prog)
{
	std::cout << "Usage: " << prog << std::endl
			  << "-d            : daemonize" << std::endl
			  << "-l log-level  : chnage log level(default is 0)" << std::endl;
}

int main(int argc, char *argv[])
{
	int opt, index, logLevel = 0;
	bool runAsDaemon = false;
	struct option options[] = {
		{"daemonize", no_argument, 0, 'd'},
		{"loglevel", required_argument, 0, 'l'},
		{0, 0, 0, 0}
	};

	while ((opt = getopt_long(argc, argv, "dl:", options, &index)) != -1) {
		switch (opt) {
		case 'd':
			runAsDaemon = true;
			break;
		case 'l':
			logLevel = atoi(optarg);
			break;
		default:
			std::cerr << "unknown" << std::endl;
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	std::cout << "Daemonize: " << runAsDaemon << std::endl;
	std::cout << "Log Level: " << logLevel << std::endl;

	if (runAsDaemon && !daemonize()) {
		exit(EXIT_FAILURE);
	}

	::signal(SIGINT, signalHandler);

	::umask(0);

	try {
		Server& server = Server::instance();
		server.run();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

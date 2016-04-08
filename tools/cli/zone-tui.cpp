/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Sungbae Yoo <sungbae.yoo@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <string>
#include <vector>
#include <iostream>

#include <unistd.h>
#include <getopt.h>
#include <dpm/zone.h>

extern char** environ;

static inline void usage(const std::string name)
{
    std::cout << "Usage: " << name << " [OPTIONS]" << std::endl
              << "Manage the zones" << std::endl
              << std::endl
              << "Options :" << std::endl
              << "   -c, --create=ZONE  create the specified zone" << std::endl
              << "   -w, --wizard=PKG   specify the setup wizard" << std::endl
              << "   -d, --destroy=ZONE destroy the specified zone" << std::endl
              << "   -s, --state=ZONE   show the state of the zone" << std::endl
              << "   -l, --list         show all of zones" << std::endl
              << "   -h, --help         show this" << std::endl
              << std::endl;
}

int main(int argc, char* argv[])
{
    int opt = 0, index, ret;
    std::string create, wizard;

    struct option options[] = {
        {"create", required_argument, 0, 'c'},
        {"wizard", required_argument, 0, 'w'},
        {"destroy", required_argument, 0, 'd'},
        {"state", required_argument, 0, 's'},
        {"list", no_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    if (argc <= 1) {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }

    if (getuid() == 0) {
        std::cerr << "this test should be done as non-root user" << std::endl;
        return EXIT_FAILURE;
    }

    dpm_client_h handle = dpm_create_client();
    if (handle == NULL) {
        std::cerr << "Failed to create client handle" << std::endl;
        return EXIT_FAILURE;
    }

    while (opt != -1) {
        opt = getopt_long(argc, argv, "c:w:d:s:lh", options, &index);
        switch (opt) {
        case 'c':
            create = optarg;
            break;
        case 'w':
            wizard = optarg;
            break;
        case 'd':
            ret = dpm_remove_zone(handle, optarg);
            if (ret != 0) {
                std::cerr << optarg << " can't be destroyed." << std::endl;
            } else {
                std::cerr << optarg << " will be removed." << std::endl;
            }
            break;
        case 's':
            ret = dpm_get_zone_state(handle, optarg);
            std::cout << optarg << " state is " << ret << "." << std::endl;
            break;
        case 'l':
            break;
        case 'h':
            usage(argv[0]);
            break;
        }
    }

    if (create.size() > 0 && wizard.size() > 0) {
        ret = dpm_create_zone(handle, create.c_str(), wizard.c_str());
        if (ret != 0) {
            std::cerr << create << " can't be created."<< std::endl;
        } else {
            std::cerr << create << " has been created."<< std::endl;
        }
    } else if (wizard.size() > 0) {
        std::cerr << "Wizard option should be used with create option." << std::endl;
    } else if (create.size() > 0) {
        std::cerr << "Setup wizard package should be specified." << std::endl
                  << "  ex) --wizard org.tizen.zone-setup-wizard" << std::endl;
    }

    dpm_destroy_client(handle);

    return EXIT_SUCCESS;
}

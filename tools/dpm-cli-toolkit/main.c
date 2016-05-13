// Copyright (c) 2015 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "dpm-cli-toolkit.h"

void print_rule(void)
{
    printf("--------------------------------------------------------------\n");
    printf("[Password Polocy]\n");
    printf("password\n");
    printf("[Security Polocy]\n");
    printf("security\n");
    printf("[Wifi Polocy]\n");
    printf("wifi\n");
    printf("[Zone Polocy]\n");
    printf("zone\n");
    printf("[Bluetooth Polocy]\n");
    printf("bluetooth\n");
    printf("Usage: dpm-cli-toolkit -s [policy-name]\n");
    printf("Ex: dpm-cli-toolkit -s password\n");
    printf("--------------------------------------------------------------\n");
    printf("[Restrictioin Polocy]\n");
    printf("camera, microphone, location, clipboard, settings, usb_debugging\n");
    printf("Usage: dpm-cli-toolkit -s [policy-name] -v [value: 0 or 1]\n");
    printf("Ex: dpm-cli-toolkit -s camera -v 1\n");
    printf("--------------------------------------------------------------\n");
}

int main(int argc, char **argv)
{
    int dpm_opt;
    int prev_opt = 0;
    int state = 1;
    char *policy_name = NULL;
    char *value = NULL;

    if (argc == 1) {
        printf("Wrong command rule! Please refer to the rule bleow.\n");
        print_rule();
        return 0;
    }

    opterr = 0;

    while ((dpm_opt = getopt(argc, argv, "s:g:v:h")) != -1) {
        switch (dpm_opt) {
        case 's':
            prev_opt = 's';
            policy_name = optarg;
            //printf("[%s] SET\n", policy_name);
            break;
        case 'g':
            prev_opt = 'g';
            policy_name = optarg;
            //printf("[%s] GET\n", policy_name);
            break;
        case 'v':
            if (prev_opt != 's') {
                printf("Usage: dpm-cli-toolkit -s [policy-name] -v [value: 0 or 1]\n");
                return 0;
            } else {
                prev_opt = 'v';
                if ((*optarg == '0') | (*optarg == '1')) {
                    value = optarg;
                    state = atoi(value);
                    printf("State: %s\n", value);
                } else {
                    printf("Usage: dpm-cli-toolkit -s [policy-name] -v [value: 0 or 1]\n");
                    return 0;
                }
            }
            break;
        case 'h':
            print_rule();
            break;
        case '?':
            if ((optopt == 's') | (optopt == 'v'))
                printf("Usage: dpm-cli-toolkit -s [policy-name] -v [value: 0 or 1]\n");
            else if (optopt == 'g')
                printf("Usage: dpm-cli-toolkit -g [policy-name]\n");
            else
                printf("Usage: dpm-cli-toolkit -s [policy-name] -v [value: 0 or 1] or dpm-cli-toolkit -g [policy-name]\n");
            return 0;
            break;
        }
    }

    if (prev_opt == 's') {
        if (strcmp(policy_name, "password") == 0)
            password_policy_handler();
        else if (strcmp(policy_name, "security") == 0)
            security_policy_handler();
        else if (strcmp(policy_name, "wifi") == 0)
            wifi_policy_handler();
        else if (strcmp(policy_name, "zone") == 0)
            zone_policy_handler();
        else if (strcmp(policy_name, "bluetooth") == 0)
            bluetooth_policy_handler();
        else {
            printf("Wrong policy name! Please refer to the policy names bleow.\n");
            print_rule();
        }
        return 0;
    }

    restriction_policy_command_handler(policy_name, prev_opt, state);

    return 0;
}

void restriction_policy_command_handler(char *policy, int command, int state)
{
    if (strcmp(policy, "camera") == 0)
        camera_policy_handler(command, state);
    else if (strcmp(policy, "microphone") == 0)
        microphone_policy_handler(command, state);
    else if (strcmp(policy, "location") == 0)
        location_policy_handler(command, state);
    else if (strcmp(policy, "clipboard") == 0)
        clipboard_policy_handler(command, state);
    else if (strcmp(policy, "settings") == 0)
        settings_policy_handler(command, state);
    else if (strcmp(policy, "usb_debugging") == 0)
        usb_debugging_policy_handler(command, state);
    else if (strcmp(policy, "bluetooth-tethering") == 0)
        bluetooth_tethering_policy_handler(command, state);
    else {
        printf("Wrong policy name! Please refer to the policy names bleow.\n");
        print_rule();
    }
}

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

void print_menu(int num)
{
    switch (num) {
    case 0:
        printf("[Password Policy]\n");
        printf("1. Set Password Quality\n");
        printf("2. Set Password Minimum Length\n");
        printf("3. Set Password Minimum Complex Characters\n");
        printf("4. Set Password Maximum number of failed attempts before device is wiped\n");
        printf("5. Set the number of days after which the password expires\n");
        printf("6. Set the number of previous passwords\n");
        printf("7. Set Password Pattern\n");
        printf("8. Reset Password\n");
        printf("9. Enforce Password Change\n");
        printf("10. Set Maximum Inactivity Time Device Lock\n");
        printf("11. Get Maximum Inactivity Time Device Lock\n");
        printf("12. Set Password Status\n");
        printf("13. Delete Password Pattern\n");
        printf("14. Get Password Pattern\n");
        printf("15. Set Maximum Character Occurrences\n");
        printf("16. Get Maximum Character Occurrences\n");
        printf("17. Set Maximum Numeric Sequence Length\n");
        printf("18. Get Maximum Numeric Sequence Length\n");
        break;
    case 1:
        printf("[Security Policy]\n");
        printf("1. Force Lock\n");
        printf("2. Wipe Data\n");
        printf("3. Reboot\n");
        printf("4. Power Off Device\n");
        break;
    case 2:
        printf("[Wifi Policy]\n");
        printf("1. Set Allow WIFI\n");
        printf("2. Get Allow WIFI\n");
        break;
    case 3:
        printf("[Zone Policy]\n");
        printf("1. Create zone\n");
        printf("2. Destroy zone\n");
        printf("3. Get zones list\n");
        printf("4. Get zone state\n");
        break;
    case 4:
        printf("[Bluetooth Policy]\n");
        printf("1. Add device MAC address to blacklist\n");
        printf("2. Remove device MAC address from blacklist\n");
        printf("3. Set device restriction\n");
        printf("4. Check device restriction\n");
        printf("5. Add uuid to blacklist\n");
        printf("6. Remove uuid from blacklist\n");
        printf("7. Set uuid restriction\n");
        printf("8. Check uuid restriction\n");
        printf("9. Set mode change state\n");
        printf("10. Check mode change state\n");
        break;
    default:
        break;
    }
    printf("> ");
}

void password_policy_handler(void)
{
    int channel;
    int input_status = 0;
    int ret = -1;
    int value = 0;
    char pattern[100], new_password[100];
    char *get_pattern = NULL;

    print_menu(DPM_PASSWORD);
    if (scanf("%d", &channel) < 0) {
        input_status = -1;
        goto out;
    }

    switch (channel) {
    case 1:
        printf("[Password Quality]\n");
        printf("1. UNSPECIFIED\n");
        printf("2. SIMPLE_PASSWORD\n");
        printf("3. SOMETHING\n");
        printf("4. NUMERIC\n");
        printf("5. ALPHABETIC\n");
        printf("6. ALPHANUMERIC\n");
        printf("> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        if (channel < 1 || channel > 6) {
            input_status = -1;
            goto out;
        }
        channel--;
        ret = set_password_quality_handler(channel);
        break;
    case 2:
        printf("minimum length(<=32)> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_password_min_length_handler(channel);
        break;
    case 3:
        printf("minimum complex characters(<=32)> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_password_min_complex_chars_handler(channel);
        break;
    case 4:
        printf("max failed attempts> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_password_max_failed_attempts_for_wipe_handler(channel);
        break;
    case 5:
        printf("day for password expiration> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_password_expires_handler(channel);
        break;
    case 6:
        printf("history> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_password_history_handler(channel);
        break;
    case 7:
        printf("pattern> ");
        if (scanf("%100s", pattern) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_password_pattern_handler(pattern);
        break;
    case 8:
        printf("new password> ");
        if (scanf("%100s", new_password) < 0) {
            input_status = -1;
            goto out;
        }
        ret = reset_password_handler(new_password);
        break;
    case 9:
        ret = enforce_password_change_handler();
        break;
    case 10:
        printf("max inactivity time> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_max_inactivity_time_device_lock_handler(channel);
        break;
    case 11:
        ret = get_max_inactivity_time_device_lock_handler(&value);
        if (ret == 0)
            printf("max inactivity time: %d\n", value);
        break;
    case 12:
        printf("status> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_password_status_handler(channel);
        break;
    case 13:
        ret = delete_password_pattern_handler();
        break;
    case 14:
        ret = get_password_pattern_handler(&get_pattern);
        if (ret == 0)
            printf("pattern: %s\n", get_pattern);
        break;
    case 15:
        printf("max character occurrences(<=32)> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_max_char_occurrences_handler(channel);
        break;
    case 16:
        ret = get_max_char_occurrences_handler(&value);
        if (ret == 0)
            printf("max character occurrences: %d\n", value);
        break;
    case 17:
        printf("max numeric sequence length(<=32)> ");
        if (scanf("%d", &channel) < 0) {
            input_status = -1;
            goto out;
        }
        ret = set_max_numeric_sequence_length_handler(channel);
        break;
    case 18:
        ret = get_max_numeric_sequence_length_handler(&value);
        if (ret == 0)
            printf("max numeric sequence length: %d\n", value);
        break;
    default:
        printf("Wrong command!\n");
        break;
    }

out:
    if (input_status < 0)
        printf("Input Error!\n");

    if (ret < 0)
        printf("password policy operation is failed.\n");
    else
        printf("password policy operation is passed.\n");
}

void security_policy_handler(void)
{
    int channel;
    int input_status = 0;
    int ret = -1;

    print_menu(DPM_SECURITY);
    if (scanf("%d", &channel) < 0) {
        input_status = -1;
        goto out;
    }

    switch (channel) {
    case 1:
    case 2:
    case 3:
    case 4:
        ret = 0;
        printf("TBD\n");
        break;
    default:
        printf("Wrong command!\n");
        break;
    }

out:
    if (input_status < 0)
        printf("Input Error!\n");

    if (ret < 0)
        printf("security policy operation is failed.\n");
    else
        printf("security policy operation is passed.\n");
}

void wifi_policy_handler(void)
{
    int channel;
    int input_status = 0;
    int ret = -1;

    print_menu(DPM_WIFI);
    if (scanf("%d", &channel) < 0) {
        input_status = -1;
        goto out;
    }

    switch (channel) {
    case 1:
    case 2:
        ret = 0;
        printf("TBD\n");
        break;
    default:
        printf("Wrong command!\n");
        break;
    }

out:
    if (input_status < 0)
        printf("Input Error!\n");

    if (ret < 0)
        printf("wifi policy operation is failed.\n");
    else
        printf("wifi policy operation is passed.\n");
}

void zone_policy_handler(void)
{
    int channel;
    int input_status = 0;
    int ret = -1;
    char name[100];

    print_menu(DPM_ZONE);
    if (scanf("%d", &channel) < 0) {
        input_status = -1;
        goto out;
    }

    switch (channel) {
    case 1:
        printf("zone name> ");
        if (scanf("%100s", name) < 0) {
            input_status = -1;
            goto out;
        }
        ret = create_zone_handler(name);
        break;
    case 2:
        printf("zone name> ");
        if (scanf("%100s", name) < 0) {
            input_status = -1;
            goto out;
        }
        ret = destroy_zone_handler(name);
        break;
    case 3:
    case 4:
        ret = 0;
        printf("TBD\n");
        break;
    default:
        printf("Wrong command!\n");
        break;
    }

out:
    if (input_status < 0)
        printf("Input Error!\n");

    if (ret < 0)
        printf("zone policy operation is failed.\n");
    else
        printf("zone policy operation is passed.\n");
}

void bluetooth_policy_handler(void)
{
    int channel;
    int input_status = 0;
    int ret = -1, enable = 0;
    char mac_address[100], uuid[100];

    print_menu(DPM_BLUETOOTH);
    if (scanf("%d", &channel) < 0) {
        input_status = -1;
        goto out;
    }

    switch (channel) {
    case 1:
        printf("mac address> ");
        if (scanf("%100s", mac_address) < 0) {
            input_status = -1;
            goto out;
        }
        ret = add_device_to_blacklist_handler(mac_address);
        break;
    case 2:
        printf("mac address> ");
        if (scanf("%100s", mac_address) < 0) {
            input_status = -1;
            goto out;
        }
        ret = remove_device_from_blacklist_handler(mac_address);
        break;
    case 3:
        printf("enable(1) or disable(0)> ");
        if (scanf("%d", &enable) < 0) {
            input_status = -1;
            goto out;
        }
        if ((enable != 0) && (enable != 1)) {
            input_status = -1;
            goto out;
        }
        ret = set_device_restriction_handler(enable);
        break;
    case 4:
        ret = is_device_restricted_handler(&enable);
        if (ret == 0) {
            if (enable == 0)
                printf("Device Restriction: Disallowed\n");
            else
                printf("Device Restriction: Allowed\n");
        }
        break;
    case 5:
        printf("uuid> ");
        if (scanf("%100s", uuid) < 0) {
            input_status = -1;
            goto out;
        }
        ret = add_uuid_to_blacklist_handler(uuid);
        break;
    case 6:
        printf("uuid> ");
        if (scanf("%100s", uuid) < 0) {
            input_status = -1;
            goto out;
        }
        ret = remove_uuid_from_blacklist_handler(uuid);
        break;
    case 7:
        printf("enable(1) or disable(0)> ");
        if (scanf("%d", &enable) < 0) {
            input_status = -1;
            goto out;
        }
        if ((enable != 0) && (enable != 1)) {
            input_status = -1;
            goto out;
        }
        ret = set_uuid_restriction_handler(enable);
        break;
    case 8:
        ret = is_uuid_restricted_handler(&enable);
        if (ret == 0) {
            if (enable == 0)
                printf("UUID Restriction: Disallowed\n");
            else
                printf("UUID Restriction: Allowed\n");
        }
        break;
    case 9:
        printf("enable(1) or disable(0)> ");
        if (scanf("%d", &enable) < 0) {
            input_status = -1;
            goto out;
        }
        if ((enable != 0) && (enable != 1)) {
            input_status = -1;
            goto out;
        }
        ret = set_mode_change_state_handler(enable);
        break;
    case 10:
        ret = is_mode_change_state_handler(&enable);
        if (ret == 0) {
            if (enable == 0)
                printf("Mode change state: Disallowed\n");
            else
                printf("Mode change state: Allowed\n");
        }
        break;
    default:
        printf("Wrong command!\n");
        break;
    }

out:
    if (input_status < 0)
        printf("Input Error!\n");

    if (ret < 0)
        printf("bluetooth policy operation is failed.\n");
    else
        printf("bluetooth policy operation is passed.\n");
}

void camera_policy_handler(int command , int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISABLE", "ENABLE"};

    if (command == 'v') {
        p_state = state;
        ret = set_camera_state_handler(p_state);
    } else
        ret = get_camera_state_handler(&p_state);

    if (ret < 0)
        printf("camera policy operation is failed.\n");
    else
        printf("camera state: %s\n", state_text[p_state]);
}

void microphone_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISABLE", "ENABLE"};

    if (command == 'v') {
        p_state = state;
        ret = set_microphone_state_handler(p_state);
    } else
        ret = get_microphone_state_handler(&p_state);

    if (ret < 0)
        printf("microphone policy operation is failed.\n");
    else
        printf("microphone state: %s\n", state_text[p_state]);
}

void location_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISABLE", "ENABLE"};

    if (command == 'v') {
        p_state = state;
        ret = set_location_state_handler(p_state);
    } else
        ret = get_location_state_handler(&p_state);

    if (ret < 0)
        printf("location policy operation is failed.\n");
    else
        printf("location state: %s\n", state_text[p_state]);
}

void clipboard_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISABLE", "ENABLE"};

    if (command == 'v') {
        p_state = state;
        ret = set_clipboard_state_handler(p_state);
    } else
        ret = get_clipboard_state_handler(&p_state);

    if (ret < 0)
        printf("clipboard policy operation is failed.\n");
    else
        printf("clipboard state: %s\n", state_text[p_state]);
}

void settings_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISABLE", "ENABLE"};

    if (command == 'v') {
        p_state = state;
        ret = set_settings_changes_restriction_handler(p_state);
    } else
        ret = get_settings_changes_restriction_handler(&p_state);

    if (ret < 0)
        printf("settings changes policy operation is failed.\n");
    else
        printf("settings changes state: %s\n", state_text[p_state]);
}

void usb_debugging_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISABLE", "ENABLE"};

    if (command == 'v') {
        p_state = state;
        ret = set_usb_debugging_state_handler(p_state);
    } else
        ret = get_usb_debugging_state_handler(&p_state);

    if (ret < 0)
        printf("usb_debugging policy operation is failed.\n");
    else
        printf("usb_debugging state: %s\n", state_text[p_state]);
}

void usb_tethering_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISALLOW", "ALLOW"};

    if (command == 'v') {
        p_state = state;
        ret = set_usb_tethering_state_handler(p_state);
    } else
        ret = get_usb_tethering_state_handler(&p_state);

    if (ret < 0)
        printf("usb-tethering policy operation is failed.\n");
    else
        printf("usb-tethering state: %s\n", state_text[p_state]);
}

void bluetooth_tethering_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISALLOW", "ALLOW"};

    if (command == 'v') {
        p_state = state;
        ret = set_bluetooth_tethering_state_handler(p_state);
    } else
        ret = get_bluetooth_tethering_state_handler(&p_state);

    if (ret < 0)
        printf("bluetooth-tethering policy operation is failed.\n");
    else
        printf("bluetooth-tethering state: %s\n", state_text[p_state]);
}

void bluetooth_desktop_connectivity_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISALLOW", "ALLOW"};

    if (command == 'v') {
        p_state = state;
        ret = set_bluetooth_desktop_connectivity_handler(p_state);
    } else
        ret = get_bluetooth_desktop_connectivity_handler(&p_state);

    if (ret < 0)
        printf("bluetooth-desktop-connectivity policy operation is failed.\n");
    else
        printf("bluetooth-desktop-connectivity state: %s\n", state_text[p_state]);
}

void bluetooth_pairing_policy_handler(int command, int state)
{
    int ret = 1;
    int p_state = 1;
    char state_text[2][10] = {"DISALLOW", "ALLOW"};

    if (command == 'v') {
        p_state = state;
        ret = set_bluetooth_pairing_handler(p_state);
    } else
        ret = get_bluetooth_pairing_handler(&p_state);

    if (ret < 0)
        printf("bluetooth-pairing policy operation is failed.\n");
    else
        printf("bluetooth-pairing state: %s\n", state_text[p_state]);
}

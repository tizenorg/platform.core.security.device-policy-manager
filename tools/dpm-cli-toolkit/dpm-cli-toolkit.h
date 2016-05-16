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

#ifndef __DPM_CLI_TOOLKIT_H__
#define __DPM_CLI_TOOLKIT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DPM_PASSWORD 0
#define DPM_SECURITY 1
#define DPM_WIFI 2
#define DPM_ZONE 3
#define DPM_BLUETOOTH 4

void password_policy_handler(void);
void security_policy_handler(void);
void wifi_policy_handler(void);
void zone_policy_handler(void);
void bluetooth_policy_handler(void);
void restriction_policy_command_handler(char *policy, int command, int state);
void camera_policy_handler(int command, int state);
void microphone_policy_handler(int command, int state);
void location_policy_handler(int command, int state);
void clipboard_policy_handler(int command, int state);
void settings_policy_handler(int command, int state);
void usb_debugging_policy_handler(int command, int state);
void bluetooth_tethering_policy_handler(int command, int state);

int set_password_quality_handler(int password_quality);
int set_password_min_length_handler(int min_length);
int set_password_min_complex_chars_handler(int min_complex_chars);
int set_password_max_failed_attempts_for_wipe_handler(int max_failed_attempts);
int set_password_expires_handler(int expired_day);
int set_password_history_handler(int history);
int set_password_pattern_handler(char *pattern);
int reset_password_handler(char *new_password);
int enforce_password_change_handler(void);
int set_max_inactivity_time_device_lock_handler(int max_inactivity_time);
int get_max_inactivity_time_device_lock_handler(int *max_inactivity_time);
int set_password_status_handler(int status);
int delete_password_pattern_handler(void);
int get_password_pattern_handler(char **password_pattern);
int set_max_char_occurrences_handler(int max_char_occur);
int get_max_char_occurrences_handler(int *max_char_occur);
int set_max_numeric_sequence_length_handler(int max_numeric_seq_length);
int get_max_numeric_sequence_length_handler(int *max_numeric_seq_length);

int create_zone_handler(char *name);
int destroy_zone_handler(char *name);

int add_device_to_blacklist_handler(const char *mac_address);
int remove_device_from_blacklist_handler(const char *mac_address);
int set_device_restriction_handler(const int enable);
int is_device_restricted_handler(int *enable);
int add_uuid_to_blacklist_handler(const char *uuid);
int remove_uuid_from_blacklist_handler(const char *uuid);
int set_uuid_restriction_handler(const int enable);
int is_uuid_restricted_handler(int *enable);
int set_mode_change_state_handler(const int enable);
int is_mode_change_state_handler(int *enable);

int set_camera_state_handler(int state);
int get_camera_state_handler(int *state);
int set_microphone_state_handler(int state);
int get_microphone_state_handler(int *state);
int set_location_state_handler(int state);
int get_location_state_handler(int *state);
int set_clipboard_state_handler(int state);
int get_clipboard_state_handler(int *state);
int set_settings_changes_restriction_handler(int state);
int get_settings_changes_restriction_handler(int *state);
int set_usb_debugging_state_handler(int state);
int get_usb_debugging_state_handler(int *state);
int set_bluetooth_tethering_state_handler(int state);
int get_bluetooth_tethering_state_handler(int *state);

#endif /* !__DPM_CLI_TOOLKIT_H__ */

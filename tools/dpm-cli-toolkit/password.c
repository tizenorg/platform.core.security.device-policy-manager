#include "dpm-cli-toolkit.h"
#include <dpm/password.h>

int set_password_quality_handler(int password_quality)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;
    dpm_password_quality_e quality;

    switch (password_quality) {
    case 0:
        quality = DPM_PASSWORD_QUALITY_UNSPECIFIED;
        break;
    case 1:
        quality = DPM_PASSWORD_QUALITY_SIMPLE_PASSWORD;
        break;
    case 2:
        quality = DPM_PASSWORD_QUALITY_SOMETHING;
        break;
    case 3:
        quality = DPM_PASSWORD_QUALITY_NUMERIC;
        break;
    case 4:
        quality = DPM_PASSWORD_QUALITY_ALPHABETIC;
        break;
    case 5:
        quality = DPM_PASSWORD_QUALITY_ALPHANUMERIC;
        break;
    default:
        quality = DPM_PASSWORD_QUALITY_UNSPECIFIED;
        break;
    }

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create device policy context handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_quality(password_policy_handle, quality) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_password_min_length_handler(int min_length)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_minimum_length(password_policy_handle, min_length) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_password_min_complex_chars_handler(int min_complex_chars)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_min_complex_chars(password_policy_handle, min_complex_chars) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_password_max_failed_attempts_for_wipe_handler(int max_failed_attempts)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_maximum_failed_attempts_for_wipe(password_policy_handle, max_failed_attempts) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_password_expires_handler(int expired_day)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_expires(password_policy_handle, expired_day) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_password_history_handler(int history)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_history(password_policy_handle, history) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_password_pattern_handler(char *pattern)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_pattern(password_policy_handle, pattern) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int reset_password_handler(char *new_password)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_reset(password_policy_handle, new_password) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int enforce_password_change_handler(void)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_enforce_change(password_policy_handle) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_max_inactivity_time_device_lock_handler(int max_inactivity_time)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_max_inactivity_time_device_lock(password_policy_handle, max_inactivity_time) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int get_max_inactivity_time_device_lock_handler(int *max_inactivity_time)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_get_max_inactivity_time_device_lock(password_policy_handle, max_inactivity_time) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_password_status_handler(int status)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_status(password_policy_handle, status) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int delete_password_pattern_handler(void)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_delete_pattern(password_policy_handle) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int get_password_pattern_handler(char **password_pattern)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_get_pattern(password_policy_handle, password_pattern) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_max_char_occurrences_handler(int max_char_occur)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_maximum_character_occurrences(password_policy_handle, max_char_occur) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int get_max_char_occurrences_handler(int *max_char_occur)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_get_maximum_character_occurrences(password_policy_handle, max_char_occur) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int set_max_numeric_sequence_length_handler(int max_numeric_seq_length)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_set_maximum_numeric_sequence_length(password_policy_handle, max_numeric_seq_length) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

int get_max_numeric_sequence_length_handler(int *max_numeric_seq_length)
{
    dpm_context_h handle;
    dpm_password_policy_h password_policy_handle;

    handle = dpm_context_create();
    if (handle == NULL) {
        printf("Failed to create client handle\n");
        return -1;
    }

    password_policy_handle = dpm_context_acquire_password_policy(handle);
    if (password_policy_handle == NULL) {
        printf("Failed to create password policy handle\n");
        return -1;
    }

    if (dpm_password_get_maximum_numeric_sequence_length(password_policy_handle, max_numeric_seq_length) == 0) {
        dpm_context_release_password_policy(handle, password_policy_handle);
        dpm_context_destroy(handle);
        return 0;
    }

    dpm_context_release_password_policy(handle, password_policy_handle);
    dpm_context_destroy(handle);
    return -1;
}

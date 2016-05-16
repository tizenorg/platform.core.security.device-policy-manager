#include "dpm-cli-toolkit.h"
#include <dpm/restriction.h>

int set_camera_state_handler(int state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create device policy context handle\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to create camera policy handle\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_camera_state(policy, state) == 0) {
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return 0;
    }

    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);
    return -1;
}

int get_camera_state_handler(int *state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create device policy context handle\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to create camera policy handle");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_get_camera_state(policy, state) == 0) {
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return 0;
    }

    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);
    return -1;
}

int set_microphone_state_handler(int state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create device policy context handle\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to create microphone policy handle\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_microphone_state(policy, state) == 0) {
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return 0;
    }

    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);
    return -1;
}

int get_microphone_state_handler(int *state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create device policy context handle\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to create microphone policy handle\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_get_microphone_state(policy, state) == 0) {
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return 0;
    }

    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);
    return -1;
}

int set_location_state_handler(int state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create dpm context\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to create client handle\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_location_state(policy, state) != 0) {
        printf("Failed to set location state\n");
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);
    return 0;
}

int get_location_state_handler(int *state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create dpm context\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to create client handle\n");
        dpm_context_destroy(context);
        return -1;
    }

    dpm_restriction_get_location_state(policy, state);
    if (state < 0) {
        printf("Failed to get location state\n");
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

int set_clipboard_state_handler(int state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;
    int get_value = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_clipboard_state(policy, state) != 0) {
        printf("Failed in dpm_restriction_set_clipboard_state()\n");
        ret = -1;
        goto out;
    }

    if (dpm_restriction_get_clipboard_state(policy, &get_value) < 0) {
        printf("Failed in dpm_restriction_get_clipboard_state()\n");
        ret = -1;
        goto out;
    }

    if (state != get_value) {
        printf("Failed to set value of restriction\n");
        ret = -1;
        goto out;
    }
out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int get_clipboard_state_handler(int *state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_get_clipboard_state(policy, state) < 0) {
        printf("Failed in dpm_restriction_get_clipboard_state()\n");
        ret = -1;
        goto out;
    }

    if (*state == 1 || *state == 0)
        ret = 0;
    else
        ret = -1;

out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int set_settings_changes_restriction_handler(int state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;
    int get_value = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_settings_changes_state(policy, state) != 0) {
        printf("Failed in dpm_restriction_set_settings_changes_state()\n");
        ret = -1;
        goto out;
    }

    if (dpm_restriction_get_settings_changes_state(policy, &get_value) < 0) {
        printf("Failed in dpm_restriction_get_settings_changes_state()\n");
        ret = -1;
        goto out;
    }

    if (state != get_value) {
        printf("Failed to set value of restriction\n");
        ret = -1;
        goto out;
    }

out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int get_settings_changes_restriction_handler(int *state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_get_settings_changes_state(policy, state) < 0) {
        printf("Failed in dpm_restriction_get_settings_changes_state()\n");
        ret = -1;
        goto out;
    }

    if (*state == 1 || *state == 0)
        ret = 0;
    else
        ret = -1;

out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int set_usb_debugging_state_handler(int state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;
    int get_value = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_usb_debugging_state(policy, state) != 0) {
        printf("Failed in dpm_restriction_set_usb_debugging_state()\n");
        ret = -1;
        goto out;
    }

    if (dpm_restriction_get_usb_debugging_state(policy, &get_value) < 0) {
        printf("Failed in dpm_restriction_get_usb_debugging_state()\n");
        ret = -1;
        goto out;
    }

    if (state != get_value) {
        printf("Failed to set value of restriction\n");
        ret = -1;
        goto out;
    }

out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int get_usb_debugging_state_handler(int *state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_get_usb_debugging_state(policy, state) < 0) {
        printf("Failed in dpm_restriction_get_usb_debugging_state()\n");
        ret = -1;
        goto out;
    }

    if (*state == 1 || *state == 0)
        ret = 0;
    else
        ret = -1;

out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int set_bluetooth_tethering_state_handler(int state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;
    int get_value = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_bluetooth_tethering_state(policy, state) != 0) {
        printf("Failed in dpm_restriction_set_bluetooth_tethering_state()\n");
        ret = -1;
        goto out;
    }

    if (dpm_restriction_get_bluetooth_tethering_state(policy, &get_value) < 0) {
        printf("Failed in dpm_restriction_get_bluetooth_tethering_state()\n");
        ret = -1;
        goto out;
    }

    if (state != get_value) {
        printf("Failed to set value of restriction\n");
        ret = -1;
        goto out;
    }

out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int get_bluetooth_tethering_state_handler(int *state)
{
    dpm_context_h context = NULL;
    dpm_restriction_policy_h policy = NULL;
    int ret = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed in dpm_context_create()\n");
        return -1;
    }

    policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed in dpm_context_acquire_restriction_policy()\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_get_bluetooth_tethering_state(policy, state) < 0) {
        printf("Failed in dpm_restriction_get_bluetooth_tethering_state()\n");
        ret = -1;
        goto out;
    }

    if (*state == 1 || *state == 0)
        ret = 0;
    else
        ret = -1;

out:
    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    if (ret == -1)
        return ret;
    else
        return 0;
}

int set_mode_change_state_handler(const int enable)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_restriction_policy_h policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to get restriction policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_set_bluetooth_mode_change_state(policy, enable) < 0) {
        printf("Failed to set mode change state\n");
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    return 0;
}

int is_mode_change_state_handler(int *enable)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_restriction_policy_h policy = dpm_context_acquire_restriction_policy(context);
    if (policy == NULL) {
        printf("Failed to get restriction policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_restriction_get_bluetooth_mode_change_state(policy, enable) != DPM_ERROR_NONE) {
        printf("Failed to check mode change state\n");
        dpm_context_release_restriction_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_restriction_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

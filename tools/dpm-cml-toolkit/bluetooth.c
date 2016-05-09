#include <stdbool.h>

#include "dpm-cml-toolkit.h"
#include <dpm/bluetooth.h>

int add_device_to_blacklist_handler(const char *mac_address)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_add_device_to_blacklist(policy, mac_address) < 0) {
        printf("Failed to add MAC Address to blacklist\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_bluetooth_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

int remove_device_from_blacklist_handler(const char *mac_address)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_remove_device_from_blacklist(policy, mac_address) < 0) {
        printf("Failed to remove MAC address to blacklist\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_bluetooth_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

int set_device_restriction_handler(const int enable)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_set_device_restriction(policy, enable) < 0) {
        printf("Failed to set device restriction\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_bluetooth_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

int is_device_restricted_handler(int *enable)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_is_device_restricted(policy, enable) != DPM_ERROR_NONE) {
        printf("Failed to check device restriction\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_bluetooth_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

int add_uuid_to_blacklist_handler(const char *uuid)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_add_uuid_to_blacklist(policy, uuid) < 0) {
        printf("Failed to add UUID to blacklist\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_bluetooth_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

int remove_uuid_from_blacklist_handler(const char *uuid)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_remove_uuid_from_blacklist(policy, uuid) < 0) {
        printf("Failed to remove UUID to blacklist\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_bluetooth_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

int set_uuid_restriction_handler(const int enable)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_set_uuid_restriction(policy, enable) < 0) {
        printf("Failed to set uuid restriction\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    return 0;
}

int is_uuid_restricted_handler(int *enable)
{
    dpm_context_h context;
    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    dpm_bluetooth_policy_h policy = dpm_context_acquire_bluetooth_policy(context);
    if (policy == NULL) {
        printf("Failed to get bluetooth policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    if (dpm_bluetooth_is_uuid_restricted(policy, enable) != DPM_ERROR_NONE) {
        printf("Failed to check uuid restriction\n");
        dpm_context_release_bluetooth_policy(context, policy);
        dpm_context_destroy(context);
        return -1;
    }

    dpm_context_release_bluetooth_policy(context, policy);
    dpm_context_destroy(context);

    return 0;
}

#include "dpm-cli-toolkit.h"
#include <dpm/zone.h>

int create_zone_handler(char *name)
{
    dpm_zone_policy_h policy;
    dpm_context_h context;
    int ret = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    policy = dpm_context_acquire_zone_policy(context);
    if (policy == NULL) {
        printf("Failed to get zone policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    ret = dpm_zone_create(policy, name, "org.tizen.zone-setup-wizard");

    dpm_context_release_zone_policy(context, policy);
    dpm_context_destroy(context);

    return ret;
}

int destroy_zone_handler(char *name)
{
    dpm_zone_policy_h policy;
    dpm_context_h context;
    int ret = 0;

    context = dpm_context_create();
    if (context == NULL) {
        printf("Failed to create client context\n");
        return -1;
    }

    policy = dpm_context_acquire_zone_policy(context);
    if (policy == NULL) {
        printf("Failed to get zone policy interface\n");
        dpm_context_destroy(context);
        return -1;
    }

    ret = dpm_zone_destroy(policy, name);

    dpm_context_release_zone_policy(context, policy);
    dpm_context_destroy(context);

    return ret;
}

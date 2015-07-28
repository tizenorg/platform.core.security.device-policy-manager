
#ifndef API
#define API __attribute__((visibility("default")))
#endif // API

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dpm_admin * dpm_admin_t;


int dpm_create_admin(const char *name, const char *password, dpm_admin_t *admin);

int dpm_get_admin(const char *name, dpm_admin_t admin);

int dpm_destroy_admin(dpm_admin_t admin);

int dpm_connect_service(void);

#ifdef __cplusplus
}
#endif

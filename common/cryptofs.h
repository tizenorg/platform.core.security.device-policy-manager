#ifndef __RUNTIME_CRYPTOFS_H__
#define __RUNTIME_CRYPTOFS_H__

#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include "keymngt.h"
#include "ecryptfs-common.h"

namespace runtime {

class Cryptofs {
public:
    int crypto_mount_dir(pam_handle_t* pamh);
    int crypto_umount_dir(pam_handle_t* pamh);

private:
    static int check_dir(char* target);
    static int check_mount_point(const char* target_path);
    static char* get_user_dir(pam_handle_t* pamh);
    static int mount_ecryptfs(const char* path, const char* sig);
    static int umount_ecryptfs(const char* path);

    Keymngt keymngt;
};
}
#endif

#include <pwd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <syslog.h>
#include <dirent.h>
#include <errno.h>

#include <sys/smack.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <mntent.h>
#include <string>

#include "cryptofs.h"
#include "exception.h"

namespace runtime {
int Cryptofs::check_dir(char* target)
{
    DIR* dp = NULL;
    int ret = 0;
    int errn;

    dp = opendir(target);
    if (dp == NULL) {
        errn = errno;
        return errn;
    }

    closedir(dp);
    return ret;
}

int Cryptofs::check_mount_point(const char* target_path)
{
    int ret = 0;
    struct mntent* mnt;
    FILE* fp = NULL;

    fp = setmntent("/etc/mtab", "r");
    if (!fp) {
        return -errno;
    }

    while ((mnt = getmntent(fp)) != NULL) {
        if (strcmp(mnt->mnt_type, "ecryptfs") != 0)
            continue;
	    if (!strcmp(mnt->mnt_dir, target_path)) {
            ret = 1;
            break;
        }
    }

    endmntent(fp);
    return ret;
}

char* Cryptofs::get_user_dir(pam_handle_t* pamh)
{
    int rc;
    const char* username = NULL;
    char* buf;

    rc = pam_get_user(pamh, &username, NULL);
    if (rc != PAM_SUCCESS || username == NULL) {
        throw runtime::Exception("Getting username error");
        return NULL;
    }

    rc = asprintf(&buf, "/home/%s", username);
    if (rc == 1) {
        throw runtime::Exception("asprintf error");
        return NULL;
    }

    return buf;
}

int Cryptofs::mount_ecryptfs(const char* path, const char* sig)
{
    int ret;
    char ecryptfs_opts[1024];

    snprintf(ecryptfs_opts, 1024, "ecryptfs_passthrough,"
            "ecryptfs_cipher=aes,"
            "ecryptfs_key_bytes=%d,"
            "ecryptfs_sig=%s,"
            "smackfsroot=*,smackfsdef=*",
            32, sig);
    ret = mount(path, path, "ecryptfs", MS_NODEV, ecryptfs_opts);

    if (ret != 0) {
        ret = errno;
        return ret;
    }

    return 0;
}


int Cryptofs::umount_ecryptfs(const char* path)
{
    int ret;

    ret = umount(path);
    if (ret != 0) {
        ret = umount2(path, MNT_EXPIRE);
        if (ret != 0 || errno == EAGAIN) {
            ret = umount2(path, MNT_EXPIRE);
            ret = (ret != 0) ? -errno : 0;
        } else {
            ret = 0;
        }
    }

    if (ret != 0) {
        ret = check_mount_point(path);
        if (ret) {
            ::sync();
            ret = umount2(path, MNT_DETACH);
        }
    }
    return ret;
}

int Cryptofs::crypto_mount_dir(pam_handle_t* pamh)
{
    int pid;
    int rc;
    int status;
    char* sig;
    char* target_path = NULL;
    const char* username = NULL;

    target_path = get_user_dir(pamh);
    if (target_path == NULL) {
        ::pam_syslog(pamh, LOG_ERR, "Failed to get user private dir\n");
        rc = -1;
        goto out;
    }

    rc = ::pam_get_user(pamh, &username, NULL);

    pid = fork();
    if (pid < 0) {
        ::pam_syslog(pamh, LOG_ERR, "Failed to fork \n");
        return -1;
    } else if (pid == 0) {
        ::smack_set_label_for_self("System");

        if (keymngt.keySearch(username) != 1) {
            keymngt.keyNew(username);
        }

        rc = keymngt.keyPush(username);
        if (rc != 0) {
            ::pam_syslog(pamh, LOG_ERR, "Failed to push key to keyring\n");
            rc = -1;
            goto out;
        }

        sig = keymngt.keyGetSig(username);
        if (sig == NULL) {
            ::pam_syslog(pamh, LOG_ERR, "Failed to get signature from keyname [%s]\n", username);
            rc = -1;
            goto out;
        }

        rc = mount_ecryptfs(target_path, sig);
        if (rc != 0) {
            ::exit(EXIT_FAILURE);
        } else {
            ::exit(0);
        }
    } else {
        ::waitpid(pid, &status, 0);
    }

    rc = 0;

out:
    free(target_path);
    return rc;
}

int Cryptofs::crypto_umount_dir(pam_handle_t* pamh)
{
    int ret;
    char* target_path = NULL;

    target_path = (char*)get_user_dir(pamh);
    if (target_path == NULL) {
        ::pam_syslog(pamh, LOG_ERR, "Failed to get user private dir\n");
        ret = -1;
        goto out;
    }

    ret = umount_ecryptfs(target_path);
    if (ret != 0)
        ::pam_syslog(pamh, LOG_ERR, "Failed to unmount crypto dir\n");

out:
    return 0;
}
}

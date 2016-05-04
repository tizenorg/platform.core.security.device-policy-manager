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
int Cryptofs::checkDir(char* target)
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

int Cryptofs::checkMountPoint(const char* targetPath)
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
        if (!strcmp(mnt->mnt_dir, targetPath)) {
            ret = 1;
            break;
        }
    }

    endmntent(fp);
    return ret;
}

int Cryptofs::mountEcryptfs(const std::string& path, const char* sig)
{
    int ret;
    char ecryptfsOpts[1024];

    snprintf(ecryptfsOpts, 1024, "ecryptfs_passthrough,"
            "ecryptfs_cipher=aes,"
            "ecryptfs_key_bytes=%d,"
            "ecryptfs_sig=%s,"
            "smackfsroot=*,smackfsdef=*",
            32, sig);
    ret = ::mount(path.c_str(), path.c_str(), "ecryptfs", MS_NODEV, ecryptfsOpts);

    if (ret != 0) {
        ret = errno;
        return ret;
    }
    return 0;
}


int Cryptofs::umountEcryptfs(const char* path)
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
        ret = checkMountPoint(path);
        if (ret) {
            ::sync();
            ret = umount2(path, MNT_DETACH);
        }
    }
    return ret;
}

}

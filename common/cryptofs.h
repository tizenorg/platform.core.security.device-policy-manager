#ifndef __RUNTIME_CRYPTOFS_H__
#define __RUNTIME_CRYPTOFS_H__

#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include <security/pam_appl.h>
#include <sys/types.h>

#include "keymngt.h"
#include "ecryptfs-common.h"

namespace runtime {

class Cryptofs {
public:
    static int checkDir(char* target);
    static int checkMountPoint(const char* targetPath);
    static int mountEcryptfs(const std::string& path, const char* sig);
    static int umountEcryptfs(const char* path);

    Keymngt keymngt;
};
}
#endif

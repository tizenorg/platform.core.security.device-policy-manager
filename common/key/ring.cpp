#include <unistd.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <linux/limits.h>

#include "key/ring.h"

#ifndef KEYCTL_SEARCH
#define KEYCTL_SEARCH   10
#endif

namespace key {

keySerial Ring::add(const char *type, const char* description, const void* payload, size_t plen, keySerial ringid)
{
    return ::syscall(__NR_add_key, type, description, payload, plen, ringid);
}

long Ring::search(keySerial ringid, const char* type, const char* description, keySerial destringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_SEARCH, ringid, type, description, destringid);
}

long Ring::link(keySerial keyid, keySerial ringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_LINK, keyid, ringid);
}

long Ring::unlink(keySerial keyid, keySerial ringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_UNLINK, keyid, ringid);
}

} // namespace key

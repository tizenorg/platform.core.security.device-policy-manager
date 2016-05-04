#include <unistd.h>
#include <linux/limits.h>
#include <sys/mount.h>
#include <sys/syscall.h>
#include <linux/keyctl.h>

#include "ecryptfs.h"
#include "key/ring.h"

namespace runtime {

keySerial Keyctl::add(const std::string& type, const std::string& description, const void* payload, size_t plen, keySerial ringid)
{
    return ::syscall(__NR_add_key, type.c_str(), description.c_str(), payload, plen, ringid);
}

long Keyctl::search(keySerial ringid, const std::string& type, const std::string& description, keySerial destringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_SEARCH, ringid, type.c_str(), description.c_str(), destringid);
}

long Keyctl::link(keySerial keyid, keySerial ringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_LINK, keyid, ringid);
}

long Keyctl::unlink(keySerial keyid, keySerial ringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_UNLINK, keyid, ringid);
}

}

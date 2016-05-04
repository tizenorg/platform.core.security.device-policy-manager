#ifndef __RUNTIME_KEYCTL_H__
#define __RUNTIME_KEYCTL_H__

#include "ecryptfs.h"
#include <sys/types.h>
#include <string>

namespace runtime {

typedef int32_t keySerial;
class Keyctl {

public:

static keySerial addKey(const std::string& type, const std::string& description, const void* payload, size_t plen, keySerial ringid);
static long keyctlSearch(keySerial ringid, const std::string& type, const std::string& description, keySerial destringid);
static long keyctlLink(keySerial keyid, keySerial ringid);
static long keyctlUnlink(keySerial keyid, keySerial ringid);
};

}
#endif

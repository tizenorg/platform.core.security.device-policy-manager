#ifndef __RUNTIME_KEYCTL_H__
#define __RUNTIME_KEYCTL_H__

#include "ecryptfs.h"
#include <sys/types.h>
#include <string>

namespace runtime {

typedef int32_t keySerial;

class Keyctl {
public:
static keySerial add(const std::string& type, const std::string& description, const void* payload, size_t plen, keySerial ringid);
static long search(keySerial ringid, const std::string& type, const std::string& description, keySerial destringid);
static long link(keySerial keyid, keySerial ringid);
static long unlink(keySerial keyid, keySerial ringid);
};

}
#endif

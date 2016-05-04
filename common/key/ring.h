#ifndef __RUNTIME_KEY_RING_H__
#define __RUNTIME_KEY_RING_H__

#include <string>
#include <sys/types.h>
#include <linux/keyctl.h>

namespace key {

typedef int32_t keySerial;

class Ring {
public:
    Ring() = delete;
    Ring(const Ring&) = delete;
    Ring(Ring&&) = delete;

    Ring& operator=(const Ring&) = delete;
    Ring& operator=(Ring&&) = delete;

    static keySerial add(const char* type, const char* description, const void* payload, size_t plen, keySerial ringid);
    static long search(keySerial ringid, const char* type, const char* description, keySerial destringid);
    static long link(keySerial keyid, keySerial ringid);
    static long unlink(keySerial keyid, keySerial ringid);
};

} // namespace key
#endif

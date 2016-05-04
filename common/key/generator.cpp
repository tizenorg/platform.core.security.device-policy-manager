#include <string>
#include <iomanip>
#include <sstream>

#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#include "exception.h"
#include "key/generator.h"

#define RAND_READ_BYTES 8
#define PBKDF2_ITERATION 1000

namespace key {

void Generator::initialize()
{
    int ret;

    ::ERR_load_crypto_strings();
    ::OpenSSL_add_all_algorithms();

    ret = ::RAND_load_file("/dev/random", RAND_READ_BYTES);

    if (ret != RAND_READ_BYTES) {
        throw runtime::Exception("RAND_load_file error");
    }

    ret = ::RAND_status();
    if (ret != 1) {
        throw runtime::Exception("RAND_status error");
    }
}

std::string Generator::hexConvert(unsigned char* src, int srcLen)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (int i = 0; i <srcLen; i++) {
        ss << std::setw(2) << src[i];
    }

    return ss.str();
}

std::string Generator::generateRandomKey(int len)
{
    unsigned char buf[len];
    int ret;

    ret = ::RAND_bytes(buf, len);
    if (ret != 1) {
        throw runtime::Exception("RAND_bytes error");
    }

    return hexConvert(buf, len);
}

std::string Generator::wrapKey(const std::string& decrypted, const std::string& salt, int len)
{
    unsigned char encrypted[len];
    int ret;

    ret = ::PKCS5_PBKDF2_HMAC_SHA1(decrypted.c_str(), decrypted.size(),
                                   (const unsigned char *)salt.c_str(), salt.size(),
                                   PBKDF2_ITERATION, len,
                                   encrypted);

    if (ret < 0) {
        throw runtime::Exception("Failed to wrap key");
    }

    return hexConvert(encrypted, len);
}

} // namespace key

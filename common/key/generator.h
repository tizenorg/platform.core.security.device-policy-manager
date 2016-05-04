#ifndef __RUNTIME_KEY_GENERATOR_H__
#define __RUNTIME_KEY_GENERATOR_H__

#include "ecryptfs.h"

namespace key {

class Generator {
public:
    Generator() = delete;
    Generator(const Generator&) = delete;
    Generator(Generator&&) = delete;

    Generator& operator=(const Generator&) = delete;
    Generator& operator=(Generator&&) = delete;

    static std::string wrapKey(const std::string& decrypted, const std::string& salt, int len);
    static std::string generateRandomKey(int len);
    static void generateToken(char* key, ecryptfs_payload** outToken);

private:
    static std::string hexConvert(unsigned char* src, int srcLen);
};

#define SHA1_DIGEST_SIZE 20

class Crypto {
public:
    static int pbkdf2_sha1(const char* pass, int passLen, const unsigned char* salt, int saltLen, int iter, int keyLen, unsigned char* out);

private:
    typedef struct {
        unsigned int state[5];
        unsigned int count[2];
        unsigned char buffer[64];
    } SHA1_CTX;

    static void sha1Init(SHA1_CTX* context);
    static void sha1Update(SHA1_CTX* context, const void* p, unsigned int len);
    static void sha1Final(unsigned char digsest[SHA1_DIGEST_SIZE], SHA1_CTX* context);
    static void sha1Transform(unsigned int state[5], const unsigned char buffer[64]);

    static void sha1Hmac(const unsigned char* key, int keyLen, const unsigned char* data, int dataLen, unsigned char out[SHA1_DIGEST_SIZE]);
};

} // namespace key

#endif

#ifndef __RUNTIME_KEYMNGT_H__
#define __RUNTIME_KEYMNGT_H__

#include "ecryptfs-common.h"
#include "exception.h"

namespace runtime {

typedef int32_t keySerial;

class Keymngt {
public:
    void keyNew(const char* keyName);
    int keySearch(const char* keyName);
    void keyPush(const char* keyName);
    char* keyGetSig(const char* keyName);

    static keySerial addKey(const char* type, const char* description, const void* payload, size_t plen, keySerial ringid);
    static long keyctlSearch(keySerial ringid, const char* type, const char* description, keySerial destringid);
    static long keyctlLink(keySerial keyid, keySerial ringid);
    static long keyctlUnlink(keySerial keyid, keySerial ringid);

    static int checkSetKey(const char* keyName);
    static void getKey(const char* keyName, char** key);
    static int generateToken(char* key, ecryptfs_payload** outToken);
    static int validateKeyring(void);
    static int addUserAuthTokenToKeyring(ecryptfs_payload* authTok);
    static void addKeyToKeyring(const char* keyName);
    static void hexConvert(char* dest, unsigned char* src, int srcLen);
    static int initializeOpenssl(void);
    static int generateRandBytes(char** outBytes, int len);
    static int getWrappingKey(char* decryptedPw, char* salt, char** wrappingKey);
    static int saveKeyToKeymanager(const char* keyName, char* key);
    void setKey(const char* keyName);
    static void getSigFromKey(char* keyName, char** signature);
};
}
#endif

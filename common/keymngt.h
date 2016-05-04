#ifndef __RUNTIME_KEYMNGT_H__
#define __RUNTIME_KEYMNGT_H__

#include "ecryptfs.h"
#include "exception.h"

namespace runtime {

typedef int32_t keySerial;

class Keymngt {
public:
    void keyNew(const std::string& keyName);
    int keySearch(const std::string& keyName);
    void keyPush(const std::string& keyName);
    char* keyGetSig(const std::string& keyName);

    static keySerial addKey(const std::string& type, const std::string& description, const void* payload, size_t plen, keySerial ringid);
    static long keyctlSearch(keySerial ringid, const std::string& type, const std::string& description, keySerial destringid);
    static long keyctlLink(keySerial keyid, keySerial ringid);
    static long keyctlUnlink(keySerial keyid, keySerial ringid);

    static int checkSetKey(const std::string& keyName);
    static void getKey(const std::string& keyName, char** key);
    static int generateToken(char* key, ecryptfs_payload** outToken);
    static int validateKeyring(void);
    static int addUserAuthTokenToKeyring(ecryptfs_payload* authTok);
    static void addKeyToKeyring(const std::string& keyName);
    static void hexConvert(char* dest, unsigned char* src, int srcLen);
    static int initializeOpenssl(void);
    static int generateRandBytes(char** outBytes, int len);
    static int getWrappingKey(char* decryptedPw, char* salt, char** wrappingKey);
    static int saveKeyToKeymanager(const std::string& keyName, char* key);
    void setKey(const std::string& keyName);
    static void getSigFromKey(const std::string& keyName, char** signature);
};
}
#endif

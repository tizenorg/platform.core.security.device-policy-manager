#ifndef __RUNTIME_KEYMNGT_H__
#define __RUNTIME_KEYMNGT_H__

#include "ecryptfs.h"
#include "exception.h"

namespace runtime {

class Keymngt {
public:
    void keyNew(const std::string& keyName);
    bool keySearch(const std::string& keyName);
    void keyPush(const std::string& keyName);
    std::string keyGetSig(const std::string& keyName);

private:
    void getKey(const std::string& keyName, char** key);
    int generateToken(char* key, ecryptfs_payload** outToken);
    int validateKeyring(void);
    int addUserAuthTokenToKeyring(ecryptfs_payload* authTok);
    void hexConvert(char* dest, unsigned char* src, int srcLen);
    int initializeOpenssl(void);
    int generateRandBytes(char** outBytes, int len);
    int getWrappingKey(char* decryptedPw, char* salt, char** wrappingKey);
    int saveKeyToKeymanager(const std::string& keyName, char* key);
    void getSigFromKey(const std::string& keyName, char** signature);
};
}
#endif

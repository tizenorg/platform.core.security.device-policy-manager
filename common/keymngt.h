#ifndef __RUNTIME_KEYMNGT_H__
#define __RUNTIME_KEYMNGT_H__

#include "ecryptfs-common.h"

namespace runtime {

typedef int32_t keySerial;

class Keymngt {
public:
    int keyNew(const char* keyName);
    int keySearch(const char* keyName);
    int keyPush(const char* keyName);
    char* keyGetSig(const char* keyName);

    static keySerial addKey(const char* type, const char* description, const void* payload, size_t plen, keySerial ringid);
    static long keyctlSearch(keySerial ringid, const char* type, const char* description, keySerial destringid);
    static long keyctlLink(keySerial keyid, keySerial ringid);
    static long keyctlUnlink(keySerial keyid, keySerial ringid);

    static int is_set_key(const char* keyName);
    static int get_key(const char* keyName, char** key);
    static int generate_token(char* key, ecryptfs_payload** out_token);
    static int validate_keyring(void);
    static int add_user_auth_token_to_keyring(ecryptfs_payload* auth_tok);
    static int add_key_to_keyring(const char* keyName);
    static void hex_convert(char* dest, unsigned char* src, int srcLen);
    static int initialize_openssl(void);
    static int generate_rand_bytes(char** out_bytes, int len);
    static int get_wrapping_key(char* decrypted_pw, char* salt, char** wrapping_key);
    static int save_key_to_keymanager(const char* keyName, char* key);
    int set_key(const char* keyName);
    static int get_sig_from_key(char* keyName, char** signature);
};
}
#endif

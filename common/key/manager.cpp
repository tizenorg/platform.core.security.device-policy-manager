#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>
#include <linux/limits.h>
#include <time.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <mntent.h>
#include <dirent.h>
#include <linux/keyctl.h>

#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#include <ckmc/ckmc-manager.h>

#include "key/manager.h"
#include "key/generator.h"
#include "key/ring.h"

#define addAliasPrefix(alias) (ckmcIdSystem + ckmcIdSeperator + alias);

namespace {

const std::string ckmcIdSystem = ckmc_owner_id_system;
const std::string ckmcIdSeperator = ckmc_owner_id_separator;

} //namespace

namespace runtime {

void Keymngt::getKey(const std::string& keyName, char** key)
{
    int ret = 0;
    char* getKey = NULL;
    std::string ckmAlias;
    ckmc_raw_buffer_s* keyData;

    getKey = (char*)malloc(sizeof(char) * ECRYPTFS_MAX_KEY_HEX);
    if (getKey == NULL) {
        ret = -ENOMEM;
        free(getKey);
        throw runtime::Exception("getKey : malloc error");
    }

    try {
        ckmAlias = addAliasPrefix(keyName);
    } catch (runtime::Exception& e) {
        free(getKey);
        throw runtime::Exception("getKey : " + std::string(e.what()));
    }

    ret = ::ckmc_get_data(ckmAlias.c_str(), NULL, &keyData);
    if (ret != CKMC_ERROR_NONE) {
        free(getKey);
        throw runtime::Exception("getKey : ckmc_get_data error " + std::to_string(ret));
    }

    strncpy(getKey, (char*)keyData->data, ECRYPTFS_MAX_KEY_HEX);
    getKey[ECRYPTFS_MAX_KEY_HEX] = '\0';

    *key = getKey;

    ::ckmc_buffer_free(keyData);
}

int Keymngt::generateToken(char* key, ecryptfs_payload** outToken)
{
    ecryptfs_payload* authTok;
    struct ecryptfs_password* tokenKey;

    unsigned char keyBuffer[ECRYPTFS_MAX_KEY_SIZE];

    authTok = (ecryptfs_payload*)malloc(sizeof(ecryptfs_payload));
    if (authTok == NULL)
        return -ECRYPTFS_ERROR_OUT_OF_MEMORY;

    memset(authTok, 0, sizeof(ecryptfs_payload));
    memcpy(keyBuffer, key, ECRYPTFS_MAX_KEY_SIZE);

    tokenKey = &authTok->token.password;

    authTok->version = ECRYPTFS_VERSION;
    authTok->token_type = ECRYPTFS_PWD_PAYLOAD_TYPE;
    tokenKey->session_key_encryption_key_bytes = ECRYPTFS_MAX_KEY_SIZE;
    tokenKey->flags = ECRYPTFS_SESSION_KEY_ENCRYPTION_KEY_SET;
    memcpy(tokenKey->session_key_encryption_key, keyBuffer, ECRYPTFS_MAX_KEY_SIZE);
    memcpy(tokenKey->signature, keyBuffer, ECRYPTFS_MAX_SIG_HEX);

    *outToken = authTok;
    return 0;
}

void Keymngt::validateKeyring(void)
{
    if (runtime::Keyctl::link(KEY_SPEC_USER_KEYRING, KEY_SPEC_SESSION_KEYRING)) {
        throw runtime::Exception("validate Keyring error");
    }
}

int Keymngt::addUserAuthTokenToKeyring(ecryptfs_payload* authTok)
{
    int ret;

    ret = runtime::Keyctl::search(KEY_SPEC_USER_KEYRING, "user", (const char*)authTok->token.password.signature, 0);
    if (ret != -1) {
        return 0;
    } else if (ret == -1 || errno != ENOKEY) {
        ret = runtime::Keyctl::add("user",
                    (const char*)authTok->token.password.signature,
                    (void*)authTok, sizeof(ecryptfs_payload),
                    KEY_SPEC_USER_KEYRING);
        if (ret == -1)
            ret = -errno;
        else
            ret = 0;
    }

    return ret;
}

void Keymngt::getSigFromKey(const std::string& keyName, char** signature)
{
    char* key = NULL;
    unsigned char keyBuffer[ECRYPTFS_MAX_KEY_SIZE];

    try {
        getKey(keyName, &key);
    } catch (runtime::Exception&e) {
        free(key);
        throw e;
    }

    memcpy(keyBuffer, key, ECRYPTFS_MAX_KEY_SIZE);
    memcpy(*signature, keyBuffer, ECRYPTFS_MAX_SIG_HEX);
}

bool Keymngt::keySearch(const std::string& keyName)
{
    int ret;
    ckmc_raw_buffer_s* keyData = NULL;
    std::string ckmAlias = addAliasPrefix(keyName);

    ret = ::ckmc_get_data(ckmAlias.c_str(), NULL, &keyData);
    ret = (ret == CKMC_ERROR_DB_ALIAS_UNKNOWN) ? 0 : 1;

    ::ckmc_buffer_free(keyData);

    return ret;
}

void Keymngt::keyNew(const std::string& keyName)
{
    const std::string ckmAlias = addAliasPrefix(keyName);
    std::string pass, salt, wrappedKey;
    ckmc_raw_buffer_s keyData;
    ckmc_policy_s keyPolicy;
    int ret;

    keyGenerator::initialize();
    pass = keyGenerator::generateRandomKey(ECRYPTFS_MAX_KEY_SIZE);
    salt = keyGenerator::generateRandomKey(ECRYPTFS_MAX_SALT_SIZE);
    wrappedKey = keyGenerator::wrapKey(pass, salt, ECRYPTFS_MAX_KEY_HEX);

    keyData.data = ::strdup(wrappedKey.c_str());
    keyData.size = wrappedKey.size();

    keyPolicy.password = NULL;
    keyPolicy.extractable = true;

    ret = ::ckmc_save_data(ckmAlias.c_str(), keyData, keyPolicy);

    ::free(keyData.data);

    if (ret != 0)
        throw runtime::Exception("ckmc_save_data error");
}

void Keymngt::keyPush(const std::string& keyName)
{
    int ret = 0;
    char* key = NULL;
    ecryptfs_payload* authTok = NULL;

    validateKeyring();

    try {
        getKey(keyName, &key);
    } catch (runtime::Exception& e) {
        free(key);
        throw e;
    }

    ret = generateToken(key, &authTok);
    if (ret != 0) {
        free(authTok);
        throw runtime::Exception("generateToken error");
    }

    ret = addUserAuthTokenToKeyring(authTok);
    if (ret != 0) {
        free(authTok);
        throw runtime::Exception("addUserAuthTokenToKeyring error");
    }

}

std::string Keymngt::keyGetSig(const std::string& keyName)
{
    char* sig = NULL;

    sig = (char*)::malloc(sizeof(char) * ECRYPTFS_MAX_SIG_HEX);
    if (sig == NULL)
        throw runtime::Exception("keyGetSig : malloc error");

    try {
        getSigFromKey(keyName, &sig);
    } catch (runtime::Exception& e) {
        free(sig);
        throw e;
    }

    std::string result(sig);
    free(sig);

    return result;
}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>
#include <linux/limits.h>
#include <time.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <fcntl.h>
#include <mntent.h>
#include <dirent.h>
#include <linux/keyctl.h>
#include <tizen.h>

#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <openssl/err.h>

#include <ckmc/ckmc-manager.h>

#include "ecryptfs-common.h"
#include "keymngt.h"

namespace runtime {

keySerial Keymngt::addKey(const char* type, const char* description, const void* payload, size_t plen, keySerial ringid)
{
    return ::syscall(__NR_add_key, type, description, payload, plen, ringid);
}

long Keymngt::keyctlSearch(keySerial ringid, const char* type, const char* description, keySerial destringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_SEARCH, ringid, type, description, destringid);
}

long Keymngt::keyctlLink(keySerial keyid, keySerial ringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_LINK, keyid, ringid);
}

long Keymngt::keyctlUnlink(keySerial keyid, keySerial ringid)
{
    return ::syscall(__NR_keyctl, KEYCTL_UNLINK, keyid, ringid);
}

char* addAliasPrefix(const char* alias)
{
    char* ckmAlias = NULL;

    int result = asprintf(&ckmAlias, "%s%s", "/System ", alias);

    if (result < 0 || ckmAlias == NULL)
        return NULL;

    return ckmAlias;
}

int Keymngt::checkSetKey(const char* keyName)
{
    int ret;
    ckmc_raw_buffer_s* keyData = NULL;
    char* ckmAlias = NULL;

    ckmAlias = addAliasPrefix(keyName);
    if (ckmAlias == NULL)
        throw runtime::Exception("addAliasPrefix error");

    ret = ::ckmc_get_data(ckmAlias, NULL, &keyData);
    ret = (ret == CKMC_ERROR_DB_ALIAS_UNKNOWN) ? 0 : 1;

    free(ckmAlias);
    ::ckmc_buffer_free(keyData);

    return ret;
}

void Keymngt::getKey(const char* keyName, char** key)
{
    int ret = 0;
    char* getKey = NULL;
    char* ckmAlias = NULL;
    ckmc_raw_buffer_s* keyData;

    getKey = (char*)malloc(sizeof(char) * ECRYPTFS_MAX_KEY_HEX);
    if (getKey == NULL) {
        ret = -ENOMEM;
        free(getKey);
        throw runtime::Exception("getKey : malloc error");
    }

    ckmAlias = addAliasPrefix(keyName);
    if (ckmAlias == NULL) {
        free(getKey);
        throw runtime::Exception("addAliasPrefix error");
    }

    ret = ::ckmc_get_data(ckmAlias, NULL, &keyData);
    if (ret != CKMC_ERROR_NONE) {
        free(getKey);
        throw runtime::Exception("getKey : ckmc_get_data error " + std::to_string(ret));
    }

    strncpy(getKey, (char*)keyData->data, ECRYPTFS_MAX_KEY_HEX);
    getKey[ECRYPTFS_MAX_KEY_HEX] = '\0';

    *key = getKey;

    free(ckmAlias);
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

int Keymngt::validateKeyring(void)
{
    long rcLong;
    int rc = 0;

    if ((rcLong = keyctlLink(KEY_SPEC_USER_KEYRING, KEY_SPEC_SESSION_KEYRING))) {
        rc = -EIO;
        goto out;
    }
out:
    return rc;
}

int Keymngt::addUserAuthTokenToKeyring(ecryptfs_payload* authTok)
{
    int ret;

    ret = keyctlSearch(KEY_SPEC_USER_KEYRING, "user", (const char*)authTok->token.password.signature, 0);
    if (ret != -1) {
        return 0;
    } else if (ret == -1 || errno != ENOKEY) {
        ret = addKey("user",
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

void Keymngt::addKeyToKeyring(const char* keyName)
{
    int ret = 0;
    char* key = NULL;
    ecryptfs_payload* authTok = NULL;

    ret = validateKeyring();
    if (ret != 0)
        throw runtime::Exception("validateKeyring error " + std::to_string(ret));

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

void Keymngt::hexConvert(char* dest, unsigned char* src, int srcLen)
{
    int i;

    for (i = 0; i <srcLen; i++)
        sprintf(&dest[i*2], "%2x", src[i]);
    dest[srcLen*2] = '\0';
}

int Keymngt::initializeOpenssl(void)
{
    int n;
    int ret;

    ::ERR_load_crypto_strings();
    ::OpenSSL_add_all_algorithms();

    n = ::RAND_load_file("/dev/random", RAND_READ_BYTES);

    ret = (n == RAND_READ_BYTES) ? 0 : -ECRYPTFS_ERROR_FAIL;
    return ret;
}

int Keymngt::generateRandBytes(char** outBytes, int len)
{
    int ret;
    char* randValue = NULL;
    unsigned char buf[len];

    if ((randValue = (char*)malloc(sizeof(char) * len * 2)) == NULL)
        return -ECRYPTFS_ERROR_OUT_OF_MEMORY;

    ret = ::RAND_bytes(buf, len);
    if (ret != 1)
        return -ECRYPTFS_ERROR_FAIL;

    hexConvert(randValue, buf, len);

    *outBytes = randValue;
    ret = strlen(*outBytes);

    if (ret < 1 || ret > len*2) {
        ret = -ECRYPTFS_ERROR_INVALID;
    }

    return ret;
}

int Keymngt::getWrappingKey(char* decryptedPw, char* salt, char** wrappingKey)
{
    int ret;
    int decryptedPwLen, saltLen;
    unsigned char encryptedPw[ECRYPTFS_MAX_KEY_SIZE];

    decryptedPwLen = strlen(decryptedPw);
    if (decryptedPwLen < 1 || decryptedPwLen > ECRYPTFS_MAX_KEY_HEX)
        return -ECRYPTFS_ERROR_INVALID;

    saltLen = strlen(salt);
    if (saltLen < 1 || saltLen > ECRYPTFS_MAX_SALT_HEX)
        return -ECRYPTFS_ERROR_INVALID;

    ret = ::PKCS5_PBKDF2_HMAC_SHA1(decryptedPw, decryptedPwLen,
                                    (unsigned char*)salt, saltLen,
                                    PBKDF2_ITERATION, ECRYPTFS_MAX_KEY_SIZE,
                                    encryptedPw);
    if (ret < 0)
        return -ECRYPTFS_ERROR_FAIL;

    hexConvert(*wrappingKey, encryptedPw, ECRYPTFS_MAX_KEY_SIZE);
    ret = strlen(*wrappingKey);

    return ret;
}

int Keymngt::saveKeyToKeymanager(const char* keyName, char* key)
{
    int ret = 0;
    char* keyAlias = NULL;
    ckmc_raw_buffer_s keyData;
    ckmc_policy_s keyPolicy;

    keyData.data = (unsigned char*)key;
    keyData.size = ::strlen(key);

    keyPolicy.password = NULL;
    keyPolicy.extractable = true;

    keyAlias = addAliasPrefix(keyName);
    if (keyAlias == NULL)
        throw runtime::Exception("set alias fail");

    ret = ::ckmc_save_data(keyAlias, keyData, keyPolicy);

    if (ret != 0)
        throw runtime::Exception("ckmc_save_data error  " + std::to_string(ret));

    return ret;
}

void Keymngt::setKey(const char* keyName)
{
    int ret = 0;
    char* pass, *salt, *wrappingKey = NULL;
    int wrappingKeyLen;

    ret = initializeOpenssl();
    if (ret != 0)
        throw runtime::Exception("initializeOpenssl error " + std::to_string(ret));

    ret = ::RAND_status();
    if (ret != 1)
        throw runtime::Exception("RAND_status error");

    ret = generateRandBytes(&pass, ECRYPTFS_MAX_KEY_SIZE);
    if (ret < 0) {
        free(pass);
        throw runtime::Exception("pass : generateRandBytes error " + std::to_string(ret));
    }

    ret = generateRandBytes(&salt, ECRYPTFS_MAX_SALT_SIZE);
    if (ret < 0) {
		free(salt);
		throw runtime::Exception("salt : generateRandBytes error " + std::to_string(ret));
    }

    wrappingKey = (char*)malloc(sizeof(char) * ECRYPTFS_MAX_KEY_HEX);
    if (wrappingKey == NULL)
        throw runtime::Exception("wrappingKey malloc error ");

    wrappingKeyLen = getWrappingKey(pass, salt, &wrappingKey);
    if (wrappingKeyLen < 0 || wrappingKeyLen > ECRYPTFS_MAX_KEY_HEX) {
        free(wrappingKey);
        throw runtime::Exception("getWrappingKey error " + std::to_string(wrappingKeyLen));
    }

    try {
        ret = saveKeyToKeymanager(keyName, wrappingKey);
    } catch (runtime::Exception& e) {
        throw e;
    }
}

void Keymngt::getSigFromKey(char* keyName, char** signature)
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

int Keymngt::keySearch(const char* keyName)
{
    int ret;

    ret = checkSetKey(keyName);
    if (ret != 1)
        return 0;

    return 1;
}

void Keymngt::keyNew(const char* keyName)
{
    try {
        setKey(keyName);
    } catch (runtime::Exception& e) {
        throw e;
    }
}

void Keymngt::keyPush(const char* keyName)
{
	try {
		addKeyToKeyring(keyName);
	} catch (runtime::Exception& e) {
		throw e;
	}
}

char* Keymngt::keyGetSig(const char* keyName)
{
    char* sig = NULL;

    sig = (char*)::malloc(sizeof(char) * ECRYPTFS_MAX_SIG_HEX);
    if (sig == NULL)
        throw runtime::Exception("keyGetSig : malloc error");

    try {
        getSigFromKey((char*)keyName, &sig);
    } catch (runtime::Exception& e) {
        free(sig);
        throw e;
    }

    return sig;
}
}

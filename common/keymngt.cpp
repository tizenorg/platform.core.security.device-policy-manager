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

int Keymngt::is_set_key(const char* keyName)
{
    int ret;
    ckmc_raw_buffer_s* key_data = NULL;

    ret = ::ckmc_get_data(keyName, NULL, &key_data);
    ret = (ret == CKMC_ERROR_DB_ALIAS_UNKNOWN) ? 0 : 1;

    ::ckmc_buffer_free(key_data);

    return ret;
}

int Keymngt::get_key(const char* keyName, char** key)
{
    int ret = 0;
    char* get_key = NULL;
    ckmc_raw_buffer_s* key_data;

    get_key = (char*)malloc(sizeof(char) * ECRYPTFS_MAX_KEY_HEX);
    if (get_key == NULL) {
        ret = -ENOMEM;
        free(get_key);
        goto error;
    }

    ret = ::ckmc_get_data(keyName, NULL, &key_data);
    if (ret != CKMC_ERROR_NONE) {
        ret = -ECRYPTFS_ERROR_FAIL;
        goto error;
    }

    strncpy(get_key, (char*)key_data->data, ECRYPTFS_MAX_KEY_HEX);
    get_key[ECRYPTFS_MAX_KEY_HEX] = '\0';

    *key = get_key;

    ::ckmc_buffer_free(key_data);
error:
    return ret;
}

int Keymngt::generate_token(char* key, ecryptfs_payload** out_token)
{
    ecryptfs_payload* auth_tok;
    struct ecryptfs_password* tok_key;

    unsigned char key_buffer[ECRYPTFS_MAX_KEY_SIZE];

    auth_tok = (ecryptfs_payload*)malloc(sizeof(ecryptfs_payload));
    if (auth_tok == NULL) {
        return -ECRYPTFS_ERROR_OUT_OF_MEMORY;
    }

    memset(auth_tok, 0, sizeof(ecryptfs_payload));
    memcpy(key_buffer, key, ECRYPTFS_MAX_KEY_SIZE);

    tok_key = &auth_tok->token.password;

    auth_tok->version = ECRYPTFS_VERSION;
    auth_tok->token_type = ECRYPTFS_PWD_PAYLOAD_TYPE;
    tok_key->session_key_encryption_key_bytes = ECRYPTFS_MAX_KEY_SIZE;
    tok_key->flags = ECRYPTFS_SESSION_KEY_ENCRYPTION_KEY_SET;
    memcpy(tok_key->session_key_encryption_key, key_buffer, ECRYPTFS_MAX_KEY_SIZE);
    memcpy(tok_key->signature, key_buffer, ECRYPTFS_MAX_SIG_HEX);

    *out_token = auth_tok;
    return 0;
}

int Keymngt::validate_keyring(void)
{
    long rc_long;
    int rc = 0;

    if ((rc_long = keyctlLink(KEY_SPEC_USER_KEYRING, KEY_SPEC_SESSION_KEYRING))) {
        rc = -EIO;
        goto out;
    }
out:
    return rc;
}

int Keymngt::add_user_auth_token_to_keyring(ecryptfs_payload* auth_tok)
{
    int ret;

    ret = keyctlSearch(KEY_SPEC_USER_KEYRING, "user", (const char*)auth_tok->token.password.signature, 0);
    if (ret != -1) {
        return 0;
    } else if (ret == -1 || errno != ENOKEY) {
        ret = addKey("user",
                    (const char*)auth_tok->token.password.signature,
                    (void*)auth_tok, sizeof(ecryptfs_payload),
                    KEY_SPEC_USER_KEYRING);
        if (ret == -1) {
            ret = -errno;
        } else {
            ret = 0;
        }
    }

    return ret;
}

int Keymngt::add_key_to_keyring(const char* keyName)
{
    int ret = 0;
    char* key = NULL;
    ecryptfs_payload* auth_tok = NULL;

    validate_keyring();

    ret = get_key(keyName, &key);
    if (ret != 0) {
        goto key_error;
    }

    ret = generate_token(key, &auth_tok);
    if (ret != 0) {
        goto token_error;
    }

    ret = add_user_auth_token_to_keyring(auth_tok);
    if (ret != 0) {
        goto token_error;
    }

token_error:
    free(auth_tok);
key_error:
    free(key);
    return ret;
}

void Keymngt::hex_convert(char* dest, unsigned char* src, int srcLen)
{
    int i;

    for (i = 0; i <srcLen; i++)
        sprintf(&dest[i*2], "%2x", src[i]);
    dest[srcLen*2] = '\0';
}

int Keymngt::initialize_openssl(void)
{
    int n;
    int ret;

    ::ERR_load_crypto_strings();
    ::OpenSSL_add_all_algorithms();

    n = ::RAND_load_file("/dev/random", RAND_READ_BYTES);

    ret = (n == RAND_READ_BYTES) ? 0 : -ECRYPTFS_ERROR_FAIL;
    return ret;
}

int Keymngt::generate_rand_bytes(char** out_bytes, int len)
{
    int ret;
    char* rand_value = NULL;
    unsigned char buf[len];

    if ((rand_value = (char*)malloc(sizeof(char) * len * 2)) == NULL) {
        return -ECRYPTFS_ERROR_OUT_OF_MEMORY;
    }

    ret = ::RAND_bytes(buf, len);
    if (ret != 1) {
        return -ECRYPTFS_ERROR_FAIL;
    }

    hex_convert(rand_value, buf, len);

    *out_bytes = rand_value;
    ret = strlen(*out_bytes);

    if (ret < 1 || ret > len*2) {
        ret = -ECRYPTFS_ERROR_INVALID;
    }

    return ret;
}

int Keymngt::get_wrapping_key(char* decrypted_pw, char* salt, char** wrapping_key)
{
    int ret;
    int decrypted_pw_len, salt_len;
    unsigned char encrypted_pw[ECRYPTFS_MAX_KEY_SIZE];

    decrypted_pw_len = strlen(decrypted_pw);
    if (decrypted_pw_len < 1 || decrypted_pw_len > ECRYPTFS_MAX_KEY_HEX) {
        return -ECRYPTFS_ERROR_INVALID;
    }

    salt_len = strlen(salt);
    if (salt_len < 1 || salt_len > ECRYPTFS_MAX_SALT_HEX) {
        return -ECRYPTFS_ERROR_INVALID;
    }

    ret = ::PKCS5_PBKDF2_HMAC_SHA1(decrypted_pw, decrypted_pw_len,
                                    (unsigned char*)salt, salt_len,
                                    PBKDF2_ITERATION, ECRYPTFS_MAX_KEY_SIZE,
                                    encrypted_pw);
    if (ret < 0) {
        return -ECRYPTFS_ERROR_FAIL;
    }

    hex_convert(*wrapping_key, encrypted_pw, ECRYPTFS_MAX_KEY_SIZE);
    ret = strlen(*wrapping_key);

    return ret;
}

int Keymngt::save_key_to_keymanager(const char* keyName, char* key)
{
    int ret = 0;

    ckmc_raw_buffer_s key_data;
    ckmc_policy_s key_policy;

    key_data.data = (unsigned char*)key;
    key_data.size = strlen(key);

    key_policy.password = NULL;
    key_policy.extractable = true;

    ret = ::ckmc_save_data(keyName, key_data, key_policy);

    return ret;
}

int Keymngt::set_key(const char* keyName)
{
    int ret = 0;
    char* pass, *salt, *wrapping_key = NULL;
    int wrapping_key_len;

    ret = initialize_openssl();
    if (ret != 0) {
        return ret;
    }

    ret = ::RAND_status();

    ret = generate_rand_bytes(&pass, ECRYPTFS_MAX_KEY_SIZE);
    if (ret < 0) {
        goto pass_error;
    }

    ret = generate_rand_bytes(&salt, ECRYPTFS_MAX_SALT_SIZE);
    if (ret < 0) {
        goto salt_error;
    }

    wrapping_key = (char*)malloc(sizeof(char) * ECRYPTFS_MAX_KEY_HEX);
    if (wrapping_key == NULL) {
        ret = -ECRYPTFS_ERROR_OUT_OF_MEMORY;
        goto wrapping_error;
    }

    wrapping_key_len = get_wrapping_key(pass, salt, &wrapping_key);
    if (wrapping_key_len < 0 || wrapping_key_len > ECRYPTFS_MAX_KEY_HEX) {
        ret = -ECRYPTFS_ERROR_INVALID;
        goto wrapping_error;
    }

    ret = save_key_to_keymanager(keyName, wrapping_key);
    if (ret < 0) {
        ret = -ECRYPTFS_ERROR_FAIL;
    }

wrapping_error:
    free(wrapping_key);
salt_error:
    free(salt);
pass_error:
    free(pass);
    return ret;
}

int Keymngt::get_sig_from_key(char* keyName, char** signature)
{
    int ret = 0;
    char* key = NULL;
    unsigned char key_buffer[ECRYPTFS_MAX_KEY_SIZE];

    ret = get_key(keyName, &key);
    if (ret != 0) {
        goto error;
    }

    memcpy(key_buffer, key, ECRYPTFS_MAX_KEY_SIZE);
    memcpy(*signature, key_buffer, ECRYPTFS_MAX_SIG_HEX);

error:
    free(key);
    return ret;
}

int Keymngt::keySearch(const char* keyName)
{
    int ret;

    ret = is_set_key(keyName);
    if (ret != 1) {
        return 0;
    }

    return 1;
}

int Keymngt::keyNew(const char* keyName)
{
    int ret = 0;

    ret = set_key(keyName);
    if (ret != 0) {
        return -1;
    }

    return 0;
}

int Keymngt::keyPush(const char* keyName)
{
    int ret;

    ret = add_key_to_keyring(keyName);
    if (ret != 0) {
        return ret;
    }

    return 0;
}

char* Keymngt::keyGetSig(const char* keyName)
{
    int ret = 0;
    char* sig = NULL;

    sig = (char*) malloc(sizeof(char) * ECRYPTFS_MAX_SIG_HEX);
    if (sig == NULL) {
        goto error;
    }

    ret = get_sig_from_key((char*)keyName, &sig);
    if (ret != 0) {
        goto error;
    }

    return sig;
error:
    free(sig);
    return sig;
}
}

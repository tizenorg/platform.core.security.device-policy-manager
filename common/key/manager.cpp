#include <string>

#include <ckmc/ckmc-manager.h>

#include "exception.h"
#include "key/manager.h"

#define addAliasPrefix(alias) (ckmcIdSystem + ckmcIdSeperator + alias);

namespace {

const std::string ckmcIdSystem = ckmc_owner_id_system;
const std::string ckmcIdSeperator = ckmc_owner_id_separator;

} //namespace

namespace key {

bool Manager::isKeyExist(const std::string& keyName)
{
    ckmc_raw_buffer_s* keyData = NULL;
    std::string ckmAlias = addAliasPrefix(keyName);
    int ret;

    ret = ::ckmc_get_data(ckmAlias.c_str(), NULL, &keyData);
    ::ckmc_buffer_free(keyData);

    return (ret != CKMC_ERROR_DB_ALIAS_UNKNOWN);
}

void Manager::addKey(const std::string& keyName, const std::string& data)
{
    const std::string ckmAlias = addAliasPrefix(keyName);
    ckmc_raw_buffer_s keyData;
    ckmc_policy_s keyPolicy;
    int ret;

    keyData.data = (unsigned char*)data.c_str();
    keyData.size = data.size();

    keyPolicy.password = NULL;
    keyPolicy.extractable = true;

    ret = ::ckmc_save_data(ckmAlias.c_str(), keyData, keyPolicy);
    if (ret != 0)
        throw runtime::Exception("ckmc_save_data error");
}

std::string Manager::getKey(const std::string& keyName)
{
    const std::string ckmAlias = addAliasPrefix(keyName);
    ckmc_raw_buffer_s* keyData;
    std::string result;
    int ret;

    ret = ::ckmc_get_data(ckmAlias.c_str(), NULL, &keyData);
    if (ret != CKMC_ERROR_NONE) {
        throw runtime::Exception("ckmc_get_data error");
    }

    result = std::string(reinterpret_cast<char*>(keyData->data), keyData->size);

    ::ckmc_buffer_free(keyData);

    return result;
}

} // namespace key

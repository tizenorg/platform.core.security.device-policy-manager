/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#include "security.hxx"
#include "audit/logger.h"

namespace DevicePolicyManager {

SecurityPolicy::SecurityPolicy(PolicyControlContext& ctxt) :
    context(ctxt)
{
}

SecurityPolicy::~SecurityPolicy()
{
}

int SecurityPolicy::lockoutDevice()
{
    try {
        return context->methodCall<int>("SecurityPolicy::lockoutDevice");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int SecurityPolicy::lockoutScreen()
{
    try {
        return context->methodCall<int>("SecurityPolicy::lockoutScreen");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int SecurityPolicy::wipeData(const int id)
{
    try {
        return context->methodCall<int>("SecurityPolicy::wipeData", id);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int SecurityPolicy::reboot()
{
    try {
        return context->methodCall<int>("SecurityPolicy::reboot");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int SecurityPolicy::powerOffDevice()
{
    try {
        return context->methodCall<int>("SecurityPolicy::powerOffDevice");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int SecurityPolicy::setInternalStorageEncryption(const bool encrypt)
{
    try {
        return context->methodCall<int>("SecurityPolicy::setInternalStorageEncryption", encrypt);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool SecurityPolicy::isInternalStorageEncrypted()
{
    try {
        return context->methodCall<bool>("SecurityPolicy::isInternalStorageEncrypted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

int SecurityPolicy::setExternalStorageEncryption(const bool encrypt)
{
    try {
        return context->methodCall<int>("SecurityPolicy::setExternalStorageEncryption", encrypt);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool SecurityPolicy::isExternalStorageEncrypted()
{
    try {
        return context->methodCall<bool>("SecurityPolicy::isExternalStorageEncrypted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

std::vector<std::string> SecurityPolicy::getFileNamesOnDevice(const std::string& path)
{
    try {
        return context->methodCall<std::vector<std::string>>("SecurityPolicy::getFileNamesOnDevice", path);
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}

std::vector<std::string> SecurityPolicy::getFileNamesWithAttributes(const std::string& path)
{
    try {
        return context->methodCall<std::vector<std::string>>("SecurityPolicy::getFileNamesWithAttributes", path);
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}

} // namespace DevicePolicyManager

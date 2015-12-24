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

Security::Security(PolicyControlContext& ctxt) :
    context(ctxt)
{
}

Security::~Security()
{
}

int Security::lockoutDevice()
{
    try {
        return context->methodCall<int>("Security::lockoutDevice");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Security::lockoutScreen()
{
    try {
        return context->methodCall<int>("Security::lockoutScreen");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Security::wipeData(const int id)
{
    try {
        return context->methodCall<int>("Security::wipeData", id);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Security::reboot()
{
    try {
        return context->methodCall<int>("Security::reboot");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Security::powerOffDevice()
{
    try {
        return context->methodCall<int>("Security::powerOffDevice");
    } catch (runtime::Exception& e) {
        return -1;
    }
}

int Security::setInternalStorageEncryption(const bool encrypt)
{
    try {
        return context->methodCall<int>("Security::setInternalStorageEncryption", encrypt);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Security::isInternalStorageEncrypted()
{
    try {
        return context->methodCall<bool>("Security::isInternalStorageEncrypted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

int Security::setExternalStorageEncryption(const bool encrypt)
{
    try {
        return context->methodCall<int>("Security::setExternalStorageEncryption", encrypt);
    } catch (runtime::Exception& e) {
        return -1;
    }
}

bool Security::isExternalStorageEncrypted()
{
    try {
        return context->methodCall<bool>("Security::isExternalStorageEncrypted");
    } catch (runtime::Exception& e) {
        return false;
    }
}

std::vector<std::string> Security::getFileNamesOnDevice(const std::string& path)
{
    try {
        return context->methodCall<std::vector<std::string>>("Security::getFileNamesOnDevice", path);
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}

std::vector<std::string> Security::getFileNamesWithAttributes(const std::string& path)
{
    try {
        return context->methodCall<std::vector<std::string>>("Security::getFileNamesWithAttributes", path);
    } catch (runtime::Exception& e) {
        return std::vector<std::string>();
    }
}

} // namespace DevicePolicyManager

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

#ifndef __SECURITY_POLICY__
#define __SECURITY_POLICY__

#include "data-type.hxx"
#include "policy-context.hxx"

namespace DevicePolicyManager {

/**
 * This class provides APIs to control security functionalities
 * such as certificate installation, encryption and wipe
 */

class Security {
public:
    Security(PolicyControlContext& ctxt);
    ~Security();

    int lockoutDevice();
    int lockoutScreen();
    int wipeData(const int id);
    int reboot();
    int powerOffDevice();
    int setInternalStorageEncryption(const bool encrypt);
    bool isInternalStorageEncrypted();
    int setExternalStorageEncryption(const bool encrypt);
    bool isExternalStorageEncrypted();
    std::vector<std::string> getFileNamesOnDevice(const std::string& path);
    std::vector<std::string> getFileNamesWithAttributes(const std::string& path);

    enum {
        WIPE_EXTERNAL_MEMORY = (1 << 0),
        WIPE_INTERNAL_MEMORY = (1 << 1)
    };
private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __SECURITY_POLICY__

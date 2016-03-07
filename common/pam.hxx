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

#ifndef __PAM__
#define __PAM__

#include <string>
#include <vector>
#include <memory>

#include <syslog.h>
#include <security/_pam_types.h>

namespace Runtime {

template <class Impl>
class PAM {
public:
    void setData(std::string name, void* data, void (*cleanup)(pam_handle_t* pamh, void* data, int error));
    const void* getData(std::string name) const;

    void setItem(int item, void* data);

    const void* getItem(int item) const;

    const std::string getUser(std::string prompt = "") const;

    void putEnv(std::string name_value);
    const std::string getEnv(std::string name) const;
    const std::vector<std::string> getEnvList() const;

    void syslog(const std::string log, int priority = LOG_ERR);


    void authenticate(int flags, std::vector<std::string> args = std::vector<std::string>());

    void setCredential(int flags, std::vector<std::string> args = std::vector<std::string>());

    void accountManagement(int flags, std::vector<std::string> args = std::vector<std::string>());

    void openSession(int flags, std::vector<std::string> args = std::vector<std::string>());
    void closeSession(int flags, std::vector<std::string> args = std::vector<std::string>());

    void changeAuthenticationToken(int flags, std::vector<std::string> args = std::vector<std::string>());

    PAM(pam_handle_t* pamh);
    ~PAM();

protected:
    pam_handle_t* pamh;

    PAM();
};

} // namespace Runtime
#endif // __PAM__

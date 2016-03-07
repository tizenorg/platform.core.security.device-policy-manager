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

#ifndef __PAM_APP__
#define __PAM_APP__

#include <string>
#include <vector>
#include <memory>

#include "pam.hxx"

namespace Runtime {

class PAMApp : public PAM<PAMApp> {
public:
    PAMApp(const std::string service, const std::string user);
    ~PAMApp();

    friend class PAM;

protected:
    void _authenticate(int flags, std::vector<std::string> args);

    void _setCredential(int flags, std::vector<std::string> args);

    void _accountManagement(int flags, std::vector<std::string> args);

    void _openSession(int flags, std::vector<std::string> args);
    void _closeSession(int flags, std::vector<std::string> args);

    void _changeAuthenticationToken(int flags, std::vector<std::string> args);
};

} // namespace Runtime
#endif // __PAM_APP__

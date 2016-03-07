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

#include <string>
#include <vector>

#include <security/pam_appl.h>

#include "exception.hxx"
#include "pam-impl.hxx"
#include "pam-app.hxx"

namespace Runtime {

template class PAM<PAMApp>;

void PAMApp::_authenticate(int flags, std::vector<std::string> args)
{
    int error = ::pam_authenticate(pamh, flags);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

void PAMApp::_setCredential(int flags, std::vector<std::string> args)
{
    int error = ::pam_setcred(pamh, PAM_SUCCESS);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

void PAMApp::_accountManagement(int flags, std::vector<std::string> args)
{
    int error = ::pam_acct_mgmt(pamh, PAM_SUCCESS);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

void PAMApp::_openSession(int flags, std::vector<std::string> args)
{
    int error = ::pam_open_session(pamh, PAM_SUCCESS);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}
void PAMApp::_closeSession(int flags, std::vector<std::string> args)
{
    int error = ::pam_close_session(pamh, PAM_SUCCESS);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

void PAMApp::_changeAuthenticationToken(int flags, std::vector<std::string> args)
{
    int error = ::pam_chauthtok(pamh, PAM_SUCCESS);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

PAMApp::PAMApp(const std::string service, const std::string user)
{
    struct pam_conv pamc;

    int error = ::pam_start(service.c_str(), user.c_str(), &pamc, &pamh);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

PAMApp::~PAMApp()
{
    int error = ::pam_end(pamh, PAM_SUCCESS);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

} // namespace Runtime

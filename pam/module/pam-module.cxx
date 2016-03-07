/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Contact: Sungbae Yoo <sungbae.yoo@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

/**
 * @file
 * @author  Sungbae Yoo (sungbae.yoo@samsung.com)
 * @brief   PAM module to create new namespace
 */

#include <string>
#include <vector>

#include <security/pam_modules.h>

#include "exception.hxx"
#include "pam-module.hxx"

#ifndef PAM_EXTERN
#define PAM_EXTERN
#endif

extern "C" {

    PAM_EXTERN  __attribute__((visibility("default"))) int
    pam_sm_authenticate(pam_handle_t* pamh, int flags, int argc, const char* argv[])
    {
        Runtime::PAMModule pam(pamh);
        std::vector<std::string> args;

        args.reserve(argc);
        for (int i = 0; i < argc; i++) {
            args.push_back(argv[i]);
        }

        try {
            pam.authenticate(flags, args);
        } catch (Runtime::Exception& e) {
            pam.syslog(e.what());
            return e.getCode();
        }

        return PAM_SUCCESS;
    }

    PAM_EXTERN  __attribute__((visibility("default"))) int
    pam_sm_acct_mgmt(pam_handle_t* pamh, int flags, int argc, const char* argv[])
    {
        Runtime::PAMModule pam(pamh);
        std::vector<std::string> args;

        args.reserve(argc);
        for (int i = 0; i < argc; i++) {
            args.push_back(argv[i]);
        }

        try {
            pam.accountManagement(flags, args);
        } catch (Runtime::Exception& e) {
            pam.syslog(e.what());
            return e.getCode();
        }

        return PAM_SUCCESS;
    }

    PAM_EXTERN  __attribute__((visibility("default"))) int
    pam_sm_open_session(pam_handle_t* pamh, int flags,
                        int argc, const char* argv[])
    {
        Runtime::PAMModule pam(pamh);
        std::vector<std::string> args;

        args.reserve(argc);
        for (int i = 0; i < argc; i++) {
            args.push_back(argv[i]);
        }

        try {
            pam.openSession(flags, args);
        } catch (Runtime::Exception& e) {
            pam.syslog(e.what());
            return e.getCode();
        }

        return PAM_SUCCESS;
    }

    PAM_EXTERN  __attribute__((visibility("default"))) int
    pam_sm_close_session(pam_handle_t* pamh, int flags,
                         int argc, const char* argv[])
    {
        Runtime::PAMModule pam(pamh);
        std::vector<std::string> args;

        args.reserve(argc);
        for (int i = 0; i < argc; i++) {
            args.push_back(argv[i]);
        }

        try {
            pam.closeSession(flags, args);
        } catch (Runtime::Exception& e) {
            pam.syslog(e.what());
            return e.getCode();
        }

        return PAM_SUCCESS;
    }

    PAM_EXTERN  __attribute__((visibility("default"))) int
    pam_sm_chauthtok(pam_handle_t* pamh, int flags, int argc, const char* argv[])
    {
        Runtime::PAMModule pam(pamh);
        std::vector<std::string> args;

        args.reserve(argc);
        for (int i = 0; i < argc; i++) {
            args.push_back(argv[i]);
        }

        try {
            pam.changeAuthenticationToken(flags, args);
        } catch (Runtime::Exception& e) {
            pam.syslog(e.what());
            return e.getCode();
        }

        return PAM_SUCCESS;
    }

#ifdef PAM_MODULE_ENTRY
    PAM_MODULE_ENTRY(PAM_MODULE_NAME);
#endif

} //extern "C"

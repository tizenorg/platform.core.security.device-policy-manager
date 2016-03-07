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

#ifndef __PAM_IMPL__
#define __PAM_IMPL__

#include <string>
#include <vector>

#include <security/pam_ext.h>
#include <security/pam_modules.h>

#include "exception.hxx"
#include "pam.hxx"

namespace Runtime {

template <class Impl>
PAM<Impl>::PAM(pam_handle_t* pamh)
    : pamh(pamh)
{

}

template <class Impl>
PAM<Impl>::PAM()
{

}

template <class Impl>
PAM<Impl>::~PAM()
{

}

template <class Impl>
void PAM<Impl>::setData(std::string name, void* data, void (*cleanup)(pam_handle_t* pamh, void* data, int error))
{
    int error = ::pam_set_data(pamh, name.c_str(), data, cleanup);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

template <class Impl>
const void* PAM<Impl>::getData(std::string name) const
{
    const void* ret;
    int error = ::pam_get_data(pamh, name.c_str(), &ret);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
    return ret;
}

template <class Impl>
void PAM<Impl>::setItem(int item, void* data)
{
    int error = ::pam_set_item(pamh, item, data);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

template <class Impl>
const void* PAM<Impl>::getItem(int item) const
{
    const void* ret;
    int error = ::pam_get_item(pamh, item, &ret);
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
    return ret;
}

template <class Impl>
const std::string PAM<Impl>::getUser(std::string prompt) const
{
    const char* user;
    int error = ::pam_get_user(pamh, &user, prompt.c_str());
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
    return std::string(user);
}

template <class Impl>
void PAM<Impl>::putEnv(std::string name_value)
{
    int error = ::pam_putenv(pamh, name_value.c_str());
    if (error != PAM_SUCCESS) {
        throw Runtime::Exception("PAM Error", error);
    }
}

template <class Impl>
const std::string PAM<Impl>::getEnv(std::string name) const
{
    const char* value = ::pam_getenv(pamh, name.c_str());
    if (value == NULL) {
        throw Runtime::Exception("PAM Error");
    }
    return std::string(value);
}

template <class Impl>
const std::vector<std::string> PAM<Impl>::getEnvList() const
{
    std::vector<std::string> ret;
    char** array = ::pam_getenvlist(pamh);
    if (array == NULL) {
        throw Runtime::Exception("PAM Error");
    }
    for (int i = 0; array[i] != NULL; i++) {
        ret.push_back(array[i]);
    }
    return ret;
}

template <class Impl>
void PAM<Impl>::syslog(const std::string log, int priority)
{
    ::pam_syslog(pamh, priority, "%s", log.c_str());
}


template <class Impl>
void PAM<Impl>::authenticate(int flags, std::vector<std::string> args)
{
    static_cast<Impl*>(this)->_authenticate(flags, args);
}

template <class Impl>
void PAM<Impl>::setCredential(int flags, std::vector<std::string> args)
{
    static_cast<Impl*>(this)->_setCredential(flags, args);
}

template <class Impl>
void PAM<Impl>::accountManagement(int flags, std::vector<std::string> args)
{
    static_cast<Impl*>(this)->_accountManagement(flags, args);
}

template <class Impl>
void PAM<Impl>::openSession(int flags, std::vector<std::string> args)
{
    static_cast<Impl*>(this)->_openSession(flags, args);
}

template <class Impl>
void PAM<Impl>::closeSession(int flags, std::vector<std::string> args)
{
    static_cast<Impl*>(this)->_closeSession(flags, args);
}

template <class Impl>
void PAM<Impl>::changeAuthenticationToken(int flags, std::vector<std::string> args)
{
    static_cast<Impl*>(this)->_changeAuthenticationToken(flags, args);
}

} // namespace Runtime
#endif // __PAM_IMPL__

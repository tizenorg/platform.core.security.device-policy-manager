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

#include <cassert>

#include "application.hxx"

#include "application.h"
#include "policy-client.h"

using namespace DevicePolicyManager;

int dpm_set_package_installation_mode(dpm_client_h handle, int mode)
{
    assert(handle);

    DevicePolicyClient& client = GetDevicePolicyClient(handle);
    Application application = client.createPolicyInterface<Application>();
    return application.setApplicationInstallationMode(mode);
}

int dpm_set_package_uninstallation_mode(dpm_client_h handle, int mode)
{
    assert(handle);

    DevicePolicyClient& client = GetDevicePolicyClient(handle);
    Application application = client.createPolicyInterface<Application>();
    return application.setApplicationUninstallationMode(mode);
}

int dpm_get_package_installation_mode(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Application application = client.createPolicyInterface<Application>();
    return application.getApplicationInstallationMode();
}

int dpm_get_package_uninstallation_mode(dpm_client_h handle)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Application application = client.createPolicyInterface<Application>();
    return application.getApplicationUninstallationMode();
}

int dpm_set_application_state(dpm_client_h handle, const char* pkgid, dpm_application_state_e state)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Application application = client.createPolicyInterface<Application>();
    return application.setApplicationState(pkgid, state);
}

int dpm_get_application_state(dpm_client_h handle, const char* pkgid)
{
    assert(handle);

    DevicePolicyClient &client = GetDevicePolicyClient(handle);
    Application application = client.createPolicyInterface<Application>();
    return application.getApplicationState(pkgid);
}

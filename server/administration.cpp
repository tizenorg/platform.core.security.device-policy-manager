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

#include "administration.hxx"
#include "audit/logger.h"

namespace DevicePolicyManager {

Administration::Administration(PolicyControlContext& ctx) :
    context(ctx)
{
    rmi::Service& manager = context.getServiceManager();

    manager.registerParametricMethod(this, (int)(Administration::registerPolicyClient)(std::string));
    manager.registerParametricMethod(this, (int)(Administration::deregisterPolicyClient)(std::string));
}

Administration::~Administration()
{
}

int Administration::registerPolicyClient(const std::string& name)
{
    ClientManager &manager = context.getClientManager();

    try {
        manager.registerClient(name);
    } catch (runtime::Exception& e) {
        ERROR("Failed to register policy client");
        return -1;
    }

    return 0;
}

int Administration::deregisterPolicyClient(const std::string& name)
{
    ClientManager& manager = context.getClientManager();

    try {
        manager.deregisterClient(name);
    } catch (runtime::Exception& e) {
        ERROR("Failed to deregister policy client");
        return -1;
    }

    return 0;
}

Administration adminPolicy(Server::instance());

} // namespace DevicePolicyManager

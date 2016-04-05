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

#include "policy-client.h"

namespace {

const std::string SUBSCRIBER_REGISTER = "Server::registerNotificationSubscriber";
const std::string SUBSCRIBER_UNREGISTER = "Server::unregisterNotificationSubscriber";

const std::string POLICY_MANAGER_ADDRESS = "/tmp/.device-policy-manager";

} // namespace


DevicePolicyClient::DevicePolicyClient() noexcept
{
}

DevicePolicyClient::~DevicePolicyClient() noexcept
{
}

int DevicePolicyClient::connect(const std::string& address) noexcept
{
    try {
        client.reset(new rmi::Client(address));
        client->connect();
    } catch (runtime::Exception& e) {
        return -1;
    }

    return 0;
}

int DevicePolicyClient::connect() noexcept
{
    return connect(POLICY_MANAGER_ADDRESS);
}

void DevicePolicyClient::disconnect() noexcept
{
    client.reset();
}

int DevicePolicyClient::subscribePolicyChange(const std::string& name,
                                              const PolicyChangeListener& listener,
                                              void* data)
{
    auto listenerDispatcher = [listener, data](const std::string& policy, std::string &state) {
        listener(policy.c_str(), state.c_str(), data);
    };

    return client->subscribe<std::string, std::string>(SUBSCRIBER_REGISTER,
                                                       name, listenerDispatcher);
}

void DevicePolicyClient::unsubscribePolicyChange(const std::string& name, int subscriberId)
{
    client->unsubscribe(SUBSCRIBER_UNREGISTER, name, subscriberId);
}

int DevicePolicyClient::subscribeSignal(const std::string& name,
                                        const SignalListener& listener,
                                        void* data)
{
    auto listenerDispatcher = [listener, data](std::string &name, std::string &from, int signal) {
        listener(from.c_str(), signal, data);
    };

    return client->subscribe<std::string, std::string, int>
                            (SUBSCRIBER_REGISTER, name, listenerDispatcher);
}

void DevicePolicyClient::unsubscribeSignal(const std::string& name, int subscriberId)
{
    client->unsubscribe(SUBSCRIBER_UNREGISTER, name, subscriberId);
}

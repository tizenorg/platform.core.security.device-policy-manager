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

#include "server.h"

namespace {

const std::string POLICY_MANAGER_ADDRESS = "/tmp/.device-policy-manager";

} // namespace

Server::Server()
{
    policyStorage.reset(new PolicyStorage("/opt/etc/dpm/policy/PolicyManifest.xml"));

    service.reset(new rmi::Service(POLICY_MANAGER_ADDRESS));

    service->registerParametricMethod(this, (FileDescriptor)(Server::registerNotificationSubscriber)(std::string));
    service->registerParametricMethod(this, (int)(Server::unregisterNotificationSubscriber)(std::string, int));
}

Server::~Server()
{
}

void Server::run()
{
    // Prepare execution environment
    service->start(true);
}

void Server::terminate()
{
    service->stop();
}

FileDescriptor Server::registerNotificationSubscriber(const std::string& name)
{
    return FileDescriptor(service->subscribeNotification(name), true);
}

int Server::unregisterNotificationSubscriber(const std::string& name, int id)
{
    return service->unsubscribeNotification(name, id);
}

int Server::updatePolicy(const std::string& name, const std::string& value,
                         const std::string& event, const std::string& info)
{
    try {
        Policy& policy = policyStorage->getPolicy(name);
        std::string old = policy.getContent();
        policy.setContent(value);
        if (old != value) {
            if (event != "") {
                service->notify(event, info);
            }

            policyStorage->flush();
        }
    } catch (runtime::Exception& e) {
        ERROR("Exception on access to policy: " + name);
        return -1;
    }

    return 0;
}

int Server::updatePolicy(const std::string& name, const std::string& value)
{
    return updatePolicy(name, value, name, value);
}

std::string Server::getPolicy(const std::string& name) const
{
    return policyStorage->getPolicy(name).getContent();
}

Server& Server::instance()
{
    static Server _instance_;

    return _instance_;
}

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

#ifndef __DEVICE_POLICY_SERVER__
#define __DEVICE_POLICY_SERVER__

#include <string>
#include <memory>

#include "client-manager.hxx"

#include "ipc/service.hxx"

namespace DevicePolicyServer {

class Server {
public:
    void run();
    void terminate();

    ClientManager& getClientManager()
    {
        return clientManager;
    }

    Ipc::Service& getServiceManager()
    {
        return *service;
    }

    static Server& instance();

private:
    Server();
    ~Server();

    std::unique_ptr<Ipc::Service> service;

    ClientManager clientManager;
};

} // namespace DevicePolicyServer
#endif //__DEVICE_POLICY_SERVER__

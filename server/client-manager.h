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

#ifndef __DPM_CLIENT_MANAGER_H__
#define __DPM_CLIENT_MANAGER_H__

#include <memory>
#include <stdexcept>
#include <string>
#include <mutex>

#include "policy.h"
#include "policy-storage.h"

#include "db/connection.h"
#include "db/statement.h"
#include "db/column.h"

class Client {
public:
    Client(const Client&) = delete;
    Client(Client&&) = default;
    Client(const std::string& name, const std::string& key);

    ~Client();

    Client& operator=(Client&&) = default;
    Client& operator=(const Client&) = delete;

    inline std::string getName() const
    {
        return name;
    }

    inline std::string getKey() const
    {
        return key;
    }

private:
    std::string name;
    std::string key;

    std::unique_ptr<PolicyStorage> policyStorage;
};

class ClientManager {
public:
    ClientManager();

    ClientManager(const ClientManager&) = delete;
    ClientManager& operator=(const ClientManager&) = delete;

    void activateClient(const std::string& name);
    void deactivateClient(const std::string& name);
    void registerClient(const std::string& name);
    void deregisterClient(const std::string& name);

    std::string generateKey();

    void dumpRegisteredClients();
    void dumpActivatedClients();

private:
    void loadClients();
    void prepareRepository();
    std::string getPackageName(int pid);

    typedef std::vector<Client> ClientList;
    ClientList activatedClients;
    ClientList registeredClients;

    std::unique_ptr<database::Connection> clientRepository;

    typedef std::recursive_mutex Mutex;
    Mutex mutex;
};

#endif //__DPM_CLIENT_MANAGER_H__

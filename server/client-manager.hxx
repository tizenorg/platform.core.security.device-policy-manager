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

#ifndef __CLIENT_MANAGER__
#define __CLIENT_MANAGER__

#include <memory>
#include <stdexcept>
#include <string>
#include <mutex>

#include "policy.hxx"
#include "policy-storage.hxx"

#include "db/connection.hxx"
#include "db/statement.hxx"
#include "db/column.hxx"

namespace DevicePolicyServer {

class Client {
public:
    Client(const Client&) = delete;
    Client(Client&&) = default;
    Client(const std::string& name, const std::string& key);

    ~Client();

    Client& operator=(Client&&) = default;
    Client& operator=(const Client&) = delete;

    inline std::string getName() const {
        return name;
    }
    inline std::string getKey() const {
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

    std::unique_ptr<Database::Connection> clientRepository;

    typedef std::recursive_mutex Mutex;
    Mutex mutex;
};

} // namespace DevicePolicyServer
#endif //__DPM_POLICY__

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

#include <climits>
#include <memory>
#include <stdexcept>
#include <string>
#include <limits>

#include "client-manager.h"
#include "policy.h"

#include "exception.h"
#include "audit/logger.h"

namespace {

const std::string clientPolicyStorage = "/opt/data/dpm";

} //namespace

Client::Client(const std::string& pkgname, const std::string& pk) :
    name(pkgname), key(pk), policyStorage(nullptr)
{
    std::string storagePath = clientPolicyStorage + "/" + name;
    policyStorage.reset(new PolicyStorage(storagePath));
}

Client::~Client()
{
}

namespace {

const std::string dataStorageLocation = "/opt/dbspace";
const std::string clientDBName = ".client.db";

} //namespace

ClientManager::ClientManager()
    : clientRepository(nullptr)
{
    std::string location = dataStorageLocation + "/" + clientDBName;
    clientRepository.reset(new database::Connection(location, database::Connection::ReadWrite |
                           database::Connection::Create));
    prepareRepository();
    loadClients();
}

void ClientManager::activateClient(const std::string& name)
{
    std::lock_guard<Mutex> lock(mutex);

    ClientList::iterator iter = registeredClients.begin();
    while (iter != registeredClients.end()) {
        if ((*iter).getName() == name) {
            activatedClients.push_back(std::move(*iter));
            registeredClients.erase(iter);
            return;
        }
        ++iter;
    }
}

void ClientManager::deactivateClient(const std::string& name)
{
    std::lock_guard<Mutex> lock(mutex);

    ClientList::iterator iter = activatedClients.begin();
    while (iter != activatedClients.end()) {
        if ((*iter).getName() == name) {
            registeredClients.push_back(std::move(*iter));
            activatedClients.erase(iter);
            return;
        }
        ++iter;
    }
}

void ClientManager::registerClient(const std::string& name)
{
    std::lock_guard<Mutex> lock(mutex);

    std::string selectQuery = "SELECT * FROM CLIENT WHERE PKG = \"" + name + "\"";
    database::Statement stmt0(*clientRepository, selectQuery);
    if (stmt0.step()) {
        throw runtime::Exception("Client already registered");
    }

    std::string key = generateKey();

    std::string insertQuery = "INSERT INTO CLIENT (PKG, KEY, VALID) VALUES (?, ?, ?)";
    database::Statement stmt(*clientRepository, insertQuery);
    stmt.bind(1, name);
    stmt.bind(2, key);
    stmt.bind(3, true);

    if (!stmt.exec()) {
        throw runtime::Exception("Failed to insert client data");
    }

    registeredClients.push_back(Client(name, key));
}

void ClientManager::deregisterClient(const std::string& name)
{
    auto removeClient = [](ClientList& list, const std::string& name) {
        ClientList::iterator iter = list.begin();
        while (iter != list.end()) {
            Client& client = *iter;
            if (client.getName() == name) {
                list.erase(iter);
                return true;
            }
            ++iter;
        }

        return false;
    };

    std::lock_guard<Mutex> lock(mutex);

    std::string query = "DELETE FROM CLIENT WHERE PKG = \"" + name + "\"";
    if (!clientRepository->exec(query)) {
        throw runtime::Exception("Failed to delete client data");
    }

    if (!removeClient(registeredClients, name)) {
        removeClient(activatedClients, name);
    }
}

void ClientManager::dumpRegisteredClients()
{
    std::cout << "Dump Registered Clients" << std::endl;

    std::lock_guard<Mutex> lock(mutex);

    ClientList::iterator iter = registeredClients.begin();
    while (iter != registeredClients.end()) {
        Client &client = (*iter);
        std::cout << "R[" << client.getName()
                  << "/" << client.getKey() << "]" << std::endl;
        ++iter;
    }
}

void ClientManager::dumpActivatedClients()
{
    std::cout << "Dump Activated Clients" << std::endl;

    std::lock_guard<Mutex> lock(mutex);

    ClientList::iterator iter = activatedClients.begin();
    while (iter != activatedClients.end()) {
        Client &client = (*iter);
        std::cout << "A[" << client.getName()
                  << "/" << client.getKey() << "]" << std::endl;
        ++iter;
    }
}

std::string ClientManager::generateKey()
{
    std::string key = "TestKey";
    return key;
}

void ClientManager::loadClients()
{
    std::lock_guard<Mutex> lock(mutex);

    database::Statement stmt(*clientRepository, "SELECT * FROM CLIENT");
    while (stmt.step()) {
        std::string name = stmt.getColumn(1).getText();
        std::string key = stmt.getColumn(2).getText();

        registeredClients.push_back(Client(name, key));
    }
}

void ClientManager::prepareRepository()
{
    std::string query = "CREATE TABLE IF NOT EXISTS CLIENT ("    \
                        "ID INTEGER PRIMARY KEY AUTOINCREMENT, " \
                        "PKG TEXT, "                             \
                        "KEY TEXT, "                             \
                        "VALID INTEGER)";

    clientRepository->exec(query);
}

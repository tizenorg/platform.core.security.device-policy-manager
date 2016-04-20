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

 #ifndef __RUNTIME_DBUS_CONNECTION_H__
 #define __RUNTIME_DBUS_CONNECTION_H__

#include <string>
#include <functional>

#include <gio/gio.h>

namespace dbus {

class DBusConnection {
public:
    typedef unsigned int SubscriptionId;
    typedef GDBusSignalCallback DBusSignalCallback;

    DBusConnection() = delete;
    DBusConnection(const DBusConnection&) = delete;
    DBusConnection(DBusConnection&&);
    ~DBusConnection();

    DBusConnection& operator=(const DBusConnection&) = delete;

    void emitSignal(const std::string& object,
                    const std::string& interface,
                    const std::string& name,
                    GVariant* parameters);

    SubscriptionId subscribeSignal(const std::string& sender,
                                   const std::string& interface,
                                   const std::string& object,
                                   const std::string& member,
                                   const DBusSignalCallback callback,
                                   void* data);

    void unsubscribeSignal(SubscriptionId id);

    GVariant* methodcall(const std::string& busName,
               const std::string& object,
               const std::string& interface,
               const std::string& method,
               GVariant* parameters,
               const std::string& replyType,
               int timeout);

    static DBusConnection createSystem();

private:
    DBusConnection(const std::string& address);

private:
    GDBusConnection* connection;
};

} // namespace dbus

 #endif //! __RUNTIME_DBUS_CONNECTION_H__

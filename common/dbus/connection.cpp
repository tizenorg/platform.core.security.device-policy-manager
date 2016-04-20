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

#include "connection.h"

#include "exception.h"
#include "scoped-gerror.h"

namespace dbus {

namespace {

const std::string DBUS_SYSTEM_BUS_ADDRESS = "unix:path=/var/run/dbus/system_bus_socket";

} // namespace

DBusConnection::DBusConnection(const std::string& address) :
     connection(nullptr)
{
    ScopedGError error;
    const GDBusConnectionFlags flags = static_cast<GDBusConnectionFlags>(
        G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT | G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION
    );

    connection = g_dbus_connection_new_for_address_sync(address.c_str(), flags, NULL, NULL, &error);
    if (error) {
        throw runtime::Exception(error->message);
    }
}

DBusConnection::DBusConnection(DBusConnection&& other) :
    connection(other.connection)
{
    other.connection = nullptr;
}

DBusConnection::~DBusConnection()
{
    if (connection) {
        g_object_unref(connection);
    }
}

DBusConnection DBusConnection::createSystem()
{
    return DBusConnection(DBUS_SYSTEM_BUS_ADDRESS);
}

void DBusConnection::emitSignal(const std::string& object,
                                const std::string& interface,
                                const std::string& name,
                                GVariant* parameters)
{
    ScopedGError error;
    g_dbus_connection_emit_signal(connection,
                                  NULL,
                                  object.c_str(),
                                  interface.c_str(),
                                  name.c_str(),
                                  parameters,
                                  &error);
    if (error) {
        throw runtime::Exception(error->message);
    }
}

DBusConnection::SubscriptionId DBusConnection::subscribeSignal(const std::string& sender,
                                                               const std::string& interface,
                                                               const std::string& object,
                                                               const std::string& member,
                                                               const DBusSignalCallback callback,
                                                               void* data)
{
    SubscriptionId id;
    id = g_dbus_connection_signal_subscribe(connection,
                                            sender.empty()    ? NULL : sender.c_str(),
                                            interface.empty() ? NULL : interface.c_str(),
                                            object.empty()    ? NULL : object.c_str(),
                                            member.empty()    ? NULL : member.c_str(),
                                            NULL,
                                            G_DBUS_SIGNAL_FLAGS_NONE,
                                            callback,
                                            data,
                                            NULL);
    return id;
}

void DBusConnection::unsubscribeSignal(DBusConnection::SubscriptionId id)
{
    g_dbus_connection_signal_unsubscribe(connection, id);
}

GVariant* DBusConnection::methodcall(const std::string& busName,
                           const std::string& object,
                           const std::string& interface,
                           const std::string& method,
                           GVariant* parameters,
                           const std::string& replyType,
                           int timeout)
{
    GVariant* result;
    ScopedGError error;
    result = g_dbus_connection_call_sync(connection,
                                         busName.c_str(),
                                         object.c_str(),
                                         interface.c_str(),
                                         method.c_str(),
                                         parameters,
                                         replyType.empty() ? NULL :
                                         G_VARIANT_TYPE(replyType.c_str()),
                                         G_DBUS_CALL_FLAGS_NONE,
                                         timeout,
                                         NULL,
                                         &error);
    if (error) {
        throw runtime::Exception(error->message);
    }

    return result;
}

} // namespace dbus

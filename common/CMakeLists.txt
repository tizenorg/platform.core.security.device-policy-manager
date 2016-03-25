#
# Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PROJECT(dpm-common)

SET (ERROR_SOURCES      ${DPM_COMMON}/error.cpp
)

SET (AUDIT_SOURCES      ${DPM_COMMON}/audit/console-sink.cpp
                        ${DPM_COMMON}/audit/null-sink.cpp
                        ${DPM_COMMON}/audit/logger.cpp
)

SET (RMI_SOURCES        ${DPM_COMMON}/rmi/connection.cpp
                        ${DPM_COMMON}/rmi/message.cpp
                        ${DPM_COMMON}/rmi/socket.cpp
                        ${DPM_COMMON}/mainloop.cpp
                        ${DPM_COMMON}/eventfd.cpp
)

SET (RMI_SERVER_SOURCES ${DPM_COMMON}/rmi/service.cpp
                        ${DPM_COMMON}/rmi/notification.cpp
                        ${DPM_COMMON}/thread-pool.cpp
)

SET (RMI_CLIENT_SOURCES ${DPM_COMMON}/rmi/client.cpp
)

SET (XML_SOURCES        ${DPM_COMMON}/xml/node.cpp
                        ${DPM_COMMON}/xml/document.cpp
                        ${DPM_COMMON}/xml/parser.cpp
                        ${DPM_COMMON}/xml/keepblanks.cpp
)

SET (DB_SOURCES         ${DPM_COMMON}/db/connection.cpp
                        ${DPM_COMMON}/db/statement.cpp
                        ${DPM_COMMON}/db/column.cpp

)

SET (UTIL_SOURCES       ${DPM_COMMON}/smack.cpp
                        ${DPM_COMMON}/filesystem.cpp
                        ${DPM_COMMON}/process.cpp
                        ${DPM_COMMON}/auth/user.cpp
                        ${DPM_COMMON}/auth/group.cpp
                        ${DPM_COMMON}/auth/shadow.cpp

)

SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

ADD_LIBRARY(${PROJECT_NAME}-error       STATIC ${ERROR_SOURCES})
ADD_LIBRARY(${PROJECT_NAME}-audit       STATIC ${AUDIT_SOURCES})
ADD_LIBRARY(${PROJECT_NAME}-rmi         STATIC ${RMI_SOURCES})
ADD_LIBRARY(${PROJECT_NAME}-rmi-server  STATIC ${RMI_SERVER_SOURCES})
ADD_LIBRARY(${PROJECT_NAME}-rmi-client  STATIC ${RMI_CLIENT_SOURCES})
ADD_LIBRARY(${PROJECT_NAME}-xml         STATIC ${XML_SOURCES})
ADD_LIBRARY(${PROJECT_NAME}-db          STATIC ${DB_SOURCES})
ADD_LIBRARY(${PROJECT_NAME}-util        STATIC ${UTIL_SOURCES})

PKG_CHECK_MODULES(XML_DEPS REQUIRED libxml-2.0)
PKG_CHECK_MODULES(DB_DEPS REQUIRED  sqlite3)
PKG_CHECK_MODULES(UTIL_DEPS REQUIRED  libsmack)

INCLUDE_DIRECTORIES(SYSTEM  ${DPM_COMMON}
                            ${XML_DEPS_INCLUDE_DIRS}
                            ${DB_DEPS_INCLUDE_DIRS}
                            ${UTIL_DEPS_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}-error)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-audit)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-rmi ${PROJECT_NAME}-error)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-rmi-server ${PROJECT_NAME}-rmi)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-rmi-client ${PROJECT_NAME}-rmi)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-xml ${XML_DEPS_LIBRARIES} ${PROJECT_NAME}-error)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-db ${DB_DEPS_LIBRARIES} ${PROJECT_NAME}-error)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-util ${DB_DEPS_LIBRARIES} ${PROJECT_NAME}-error)

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

SET (COMMON_SOURCES     ${DPM_COMMON}/error.cpp
                        ${DPM_COMMON}/smack.cpp
                        ${DPM_COMMON}/process.cpp
                        ${DPM_COMMON}/eventfd.cpp
                        ${DPM_COMMON}/mainloop.cpp
                        ${DPM_COMMON}/filesystem.cpp
                        ${DPM_COMMON}/thread-pool.cpp
                        ${DPM_COMMON}/rmi/socket.cpp
                        ${DPM_COMMON}/rmi/client.cpp
                        ${DPM_COMMON}/rmi/message.cpp
                        ${DPM_COMMON}/rmi/message-composer.cpp
                        ${DPM_COMMON}/rmi/service.cpp
                        ${DPM_COMMON}/rmi/connection.cpp
                        ${DPM_COMMON}/rmi/notification.cpp
                        ${DPM_COMMON}/audit/logger.cpp
                        ${DPM_COMMON}/audit/null-sink.cpp
                        ${DPM_COMMON}/audit/console-sink.cpp
                        ${DPM_COMMON}/xml/node.cpp
                        ${DPM_COMMON}/xml/parser.cpp
                        ${DPM_COMMON}/xml/document.cpp
                        ${DPM_COMMON}/xml/keepblanks.cpp
                        ${DPM_COMMON}/db/column.cpp
                        ${DPM_COMMON}/db/statement.cpp
                        ${DPM_COMMON}/db/connection.cpp
                        ${DPM_COMMON}/auth/user.cpp
                        ${DPM_COMMON}/auth/group.cpp
                        ${DPM_COMMON}/auth/shadow.cpp

)

SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

ADD_LIBRARY(${PROJECT_NAME} STATIC ${COMMON_SOURCES})

PKG_CHECK_MODULES(COMMON_DEPS REQUIRED  libxml-2.0
                                        sqlite3
                                        libsmack
)

INCLUDE_DIRECTORIES(SYSTEM  ${DPM_COMMON}
                            ${COMMON_DEPS_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${COMMON_DEPS_LIBRARIES})

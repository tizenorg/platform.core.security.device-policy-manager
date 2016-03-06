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

SET(TARGET ${PROJECT_NAME})

SET(SOURCES	main.cpp
			server.cpp
			policy.cpp
			policy-storage.cpp
			client-manager.cpp
			administration.cpp
			security.cpp
			${DPM_COMMON}/error.cpp
			${DPM_COMMON}/mainloop.cpp
			${DPM_COMMON}/process.cpp
			${DPM_COMMON}/filesystem.cpp
			${DPM_COMMON}/thread-pool.cpp
            ${DPM_COMMON}/eventfd.cpp
			${DPM_COMMON}/audit/logger.cpp
			${DPM_COMMON}/audit/null-sink.cpp
			${DPM_COMMON}/audit/console-sink.cpp
			${DPM_COMMON}/rmi/socket.cpp
			${DPM_COMMON}/rmi/service.cpp
			${DPM_COMMON}/rmi/message.cpp
			${DPM_COMMON}/rmi/connection.cpp
			${DPM_COMMON}/rmi/notification.cpp
			${DPM_COMMON}/db/connection.cpp
			${DPM_COMMON}/db/statement.cpp
			${DPM_COMMON}/db/column.cpp
			${DPM_COMMON}/xml/parser.cpp
			${DPM_COMMON}/xml/document.cpp
			${DPM_COMMON}/xml/node.cpp
			${DPM_COMMON}/xml/keepblanks.cpp
)

ADD_EXECUTABLE(${TARGET} ${SOURCES} ${INCLUDE_SRCS})

PKG_CHECK_MODULES(SERVER_DEPS REQUIRED glib-2.0 libxml-2.0 sqlite3 bundle aul appsvc vconf syspopup-caller deviced)

INCLUDE_DIRECTORIES(SYSTEM ${SERVER_DEPS_INCLUDE_DIRS} ${DPM_COMMON} ${DPM_POLICY} ${DPM_SERVER})

TARGET_LINK_LIBRARIES(${TARGET} ${SERVER_DEPS_LIBRARIES} pthread)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "-fPIE")
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "-pie")

INSTALL(TARGETS ${TARGET} DESTINATION bin)
INSTALL(FILES data/PolicyManifest.xml DESTINATION ${POLICY_INSTALL_DIR})


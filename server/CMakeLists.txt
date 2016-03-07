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
			password.cpp
			zone.cpp
			app-bundle.cpp
			syspopup.cpp
			launchpad.cpp
			${DPM_COMMON}/error.cpp
			${DPM_COMMON}/mainloop.cpp
			${DPM_COMMON}/process.cpp
			${DPM_COMMON}/filesystem.cpp
			${DPM_COMMON}/smack.cpp
			${DPM_COMMON}/thread-pool.cpp
            ${DPM_COMMON}/eventfd.cpp
			${DPM_COMMON}/auth/user.cpp
			${DPM_COMMON}/auth/group.cpp
			${DPM_COMMON}/auth/shadow.cpp
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

PKG_CHECK_MODULES(SERVER_DEPS REQUIRED glib-2.0 gio-2.0 libxml-2.0 sqlite3 bundle aul appsvc vconf syspopup-caller deviced libsmack libtzplatform-config authentication-server-admin)

INCLUDE_DIRECTORIES(SYSTEM ${SERVER_DEPS_INCLUDE_DIRS} ${DPM_COMMON} ${DPM_POLICY} ${DPM_SERVER})

TARGET_LINK_LIBRARIES(${TARGET} ${SERVER_DEPS_LIBRARIES} pthread)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "-fPIE")
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "-pie")

TARGET_COMPILE_DEFINITIONS(${TARGET} PRIVATE
    CONF_PATH="${CONF_INSTALL_DIR}"
    DATA_PATH="${DATA_INSTALL_DIR}"
    DB_PATH="${DB_INSTALL_DIR}"
)

INSTALL(TARGETS ${TARGET} DESTINATION bin)
INSTALL(FILES data/PolicyManifest.xml DESTINATION ${CONF_INSTALL_DIR}/policy)
INSTALL(FILES data/DefaultBundleManifest.xml DESTINATION ${CONF_INSTALL_DIR}/zone RENAME owner.xml)

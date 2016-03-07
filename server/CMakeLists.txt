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

SET(SOURCES	main.cxx
			server.cxx
			policy.cxx
			policy-storage.cxx
			client-manager.cxx
			administration.cxx
			security.cxx
			${DPM_COMMON}/error.cxx
			${DPM_COMMON}/user.cxx
			${DPM_COMMON}/group.cxx
			${DPM_COMMON}/shadow.cxx
			${DPM_COMMON}/process.cxx
			${DPM_COMMON}/filesystem.cxx
			${DPM_COMMON}/smack.cxx
			${DPM_COMMON}/thread-pool.cxx
            ${DPM_COMMON}/eventfd.cxx
			${DPM_COMMON}/audit/logger.cxx
			${DPM_COMMON}/audit/null-sink.cxx
			${DPM_COMMON}/audit/console-sink.cxx
			${DPM_COMMON}/ipc/socket.cxx
			${DPM_COMMON}/ipc/service.cxx
			${DPM_COMMON}/ipc/mainloop.cxx
			${DPM_COMMON}/ipc/message.cxx
			${DPM_COMMON}/ipc/connection.cxx
            ${DPM_COMMON}/ipc/notification.cxx
			${DPM_COMMON}/db/connection.cxx
			${DPM_COMMON}/db/statement.cxx
			${DPM_COMMON}/db/column.cxx
			${DPM_COMMON}/xml/parser.cxx
			${DPM_COMMON}/xml/document.cxx
			${DPM_COMMON}/xml/node.cxx
			${DPM_COMMON}/xml/keepblanks
)

ADD_EXECUTABLE(${TARGET} ${SOURCES} ${INCLUDE_SRCS})

PKG_CHECK_MODULES(SERVER_DEPS REQUIRED glib-2.0 libxml-2.0 sqlite3 bundle aul appsvc vconf syspopup-caller deviced libsmack libtzplatform-config)

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

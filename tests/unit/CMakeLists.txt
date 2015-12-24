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
SET(UNIT_TEST_TARGET	"dpm-unit-tests")

SET(UNIT_TEST_SOURCES	main.cxx
						ipc.cxx
						database.cxx
						xml.cxx
						filesystem.cxx
						${DPM_COMMON}/error.cxx
						${DPM_COMMON}/filesystem.cxx
						${DPM_COMMON}/audit/logger.cxx
						${DPM_COMMON}/audit/null-sink.cxx
						${DPM_COMMON}/audit/console-sink.cxx
						${DPM_COMMON}/ipc/socket.cxx
						${DPM_COMMON}/ipc/connection.cxx
						${DPM_COMMON}/ipc/message.cxx
						${DPM_COMMON}/ipc/service.cxx
						${DPM_COMMON}/ipc/client.cxx
						${DPM_COMMON}/ipc/mainloop.cxx
						${DPM_COMMON}/db/connection.cxx
						${DPM_COMMON}/db/statement.cxx
						${DPM_COMMON}/db/column.cxx
						${DPM_COMMON}/xml/parser.cxx
						${DPM_COMMON}/xml/document.cxx
						${DPM_COMMON}/xml/node.cxx
						${DPM_COMMON}/xml/keepblanks.cxx
						${DPM_TESTBENCH}/testbench.cxx
)

ADD_EXECUTABLE(${UNIT_TEST_TARGET} ${UNIT_TEST_SOURCES})

PKG_CHECK_MODULES(TESTS_DEPS REQUIRED glib-2.0 libxml-2.0 sqlite3)
INCLUDE_DIRECTORIES(SYSTEM ${TESTS_DEPS_INCLUDE_DIRS} ${DPM_COMMON} ${DPM_LIBS} ${DPM_TESTS})

SET_TARGET_PROPERTIES(${UNIT_TEST_TARGET} PROPERTIES
	COMPILE_FLAGS "-pthread"
	LINK_FLAGS "-pthread"
)

TARGET_LINK_LIBRARIES(${UNIT_TEST_TARGET} ${TESTS_DEPS_LIBRARIES})

INSTALL(TARGETS ${UNIT_TEST_TARGET} DESTINATION bin)
INSTALL(FILES   data/sample-policy.xml DESTINATION ${DATA_INSTALL_DIR}/dpm)

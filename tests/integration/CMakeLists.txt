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
SET(INTEGRATION_TEST_TARGET	"dpm-integration-tests")

SET(INTEGRATION_TEST_SOURCES main.cpp
							 administration.cpp
							 security.cpp
							 ${DPM_LIBS}/policy-client.cpp
							 ${DPM_LIBS}/administration.cpp
							 ${DPM_LIBS}/security.cpp
							 ${DPM_COMMON}/error.cpp
							 ${DPM_COMMON}/mainloop.cpp
							 ${DPM_COMMON}/thread-pool.cpp
							 ${DPM_COMMON}/eventfd.cpp
							 ${DPM_COMMON}/audit/logger.cpp
							 ${DPM_COMMON}/audit/null-sink.cpp
							 ${DPM_COMMON}/audit/console-sink.cpp
							 ${DPM_COMMON}/rmi/socket.cpp
							 ${DPM_COMMON}/rmi/connection.cpp
							 ${DPM_COMMON}/rmi/message.cpp
							 ${DPM_COMMON}/rmi/service.cpp
							 ${DPM_COMMON}/rmi/client.cpp
							 ${DPM_COMMON}/rmi/notification.cpp
							 ${DPM_TESTBENCH}/testbench.cpp
)

ADD_EXECUTABLE(${INTEGRATION_TEST_TARGET} ${INTEGRATION_TEST_SOURCES})

PKG_CHECK_MODULES(TESTS_DEPS REQUIRED glib-2.0)
INCLUDE_DIRECTORIES(SYSTEM ${TESTS_DEPS_INCLUDE_DIRS} ${DPM_COMMON} ${DPM_POLICY} ${DPM_LIBS} ${DPM_TESTS})

SET_TARGET_PROPERTIES(${INTEGRATION_TEST_TARGET} PROPERTIES
	COMPILE_FLAGS "-pthread"
	LINK_FLAGS "-pthread"
)

TARGET_LINK_LIBRARIES(${INTEGRATION_TEST_TARGET} ${TESTS_DEPS_LIBRARIES})

INSTALL(TARGETS ${INTEGRATION_TEST_TARGET} DESTINATION bin)

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
#							 administration.cpp
#							 application.cpp
#							 bluetooth.cpp
#							 misc.cpp
#							 restriction.cpp
#							 security.cpp
#							 zone.cpp
#							 ${DPM_LIBS}/policy-client.cpp
#							 ${DPM_LIBS}/administration.cpp
#							 ${DPM_LIBS}/application.cpp
#							 ${DPM_LIBS}/bluetooth.cpp
#							 ${DPM_LIBS}/misc.cpp
#							 ${DPM_LIBS}/restriction.cpp
#							 ${DPM_LIBS}/security.cpp
#							 ${DPM_LIBS}/wifi.cpp
#							 ${DPM_LIBS}/zone.cpp
							 ${DPM_TESTBENCH}/testbench.cpp
)

ADD_EXECUTABLE(${INTEGRATION_TEST_TARGET} ${INTEGRATION_TEST_SOURCES})

INCLUDE_DIRECTORIES(SYSTEM ${DPM_COMMON} ${DPM_POLICY} ${DPM_LIBS} ${DPM_TESTS})

TARGET_LINK_LIBRARIES(${INTEGRATION_TEST_TARGET} dpm-common)
SET_TARGET_PROPERTIES(${INTEGRATION_TEST_TARGET} PROPERTIES
	COMPILE_FLAGS "-pthread"
	LINK_FLAGS "-pthread"
)

INSTALL(TARGETS ${INTEGRATION_TEST_TARGET} DESTINATION bin)

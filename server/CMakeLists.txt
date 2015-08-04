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

SET(SOURCES	main.cxx server.cxx)

ADD_EXECUTABLE(${TARGET} ${SOURCES} ${INCLUDE_SRCS})

PKG_CHECK_MODULES(SERVER_DEPS REQUIRED glib-2.0)

INCLUDE_DIRECTORIES(SYSTEM ${SERVER_DEPS_INCLUDE_DIRS} ${DPM_COMMON} ${DPM_POLICY} ${DPM_SERVER})

TARGET_LINK_LIBRARIES(${TARGET} ${SERVER_DEPS_LIBRARIES} pthread)
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "-fPIE")
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "-pie")

INSTALL(TARGETS ${TARGET} DESTINATION bin)


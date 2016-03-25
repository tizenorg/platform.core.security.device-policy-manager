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

PROJECT(dpm-pam-zone)

FILE(GLOB PAM_ZONE_SRCS      module/*.cpp)
FILE(GLOB PAM_ZONE_CLI_SRCS  cli/*.cpp
							 ${DPM_COMMON}/pam.cpp
)

FILE(GLOB DPM_COMMON_SRCS				${DPM_COMMON}/filesystem.cpp
							${DPM_COMMON}/access-control.cpp
							${DPM_COMMON}/auth/user.cpp
							${DPM_COMMON}/auth/group.cpp
							${DPM_COMMON}/auth/shadow.cpp
)


SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

SET(PAM_ZONE_NAME "pam_zone")
ADD_LIBRARY(${PAM_ZONE_NAME} MODULE ${PAM_ZONE_SRCS})
SET_TARGET_PROPERTIES(${PAM_ZONE_NAME} PROPERTIES  PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_INSTALL_DIR}/pam-zone"
    COMPILE_FLAGS "-fvisibility=hidden"
)

SET(PAM_ZONE_CLI_NAME "nsattach")
ADD_EXECUTABLE(${PAM_ZONE_CLI_NAME} ${PAM_ZONE_CLI_SRCS})
SET_TARGET_PROPERTIES(${PAM_ZONE_CLI_NAME} PROPERTIES PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_INSTALL_DIR}/pam-zone"
    COMPILE_FLAGS "-fPIE"
    LINK_FLAGS "-pie"
)

FIND_PATH(PAM_INCLUDE_DIR NAMES security/pam_appl.h security/pam_ext.h security/pam_modules.h
    HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES include)
FIND_LIBRARY(PAM_LIBRARY pam  HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES ${LIB_INSTALL_DIR})
MARK_AS_ADVANCED(PAM_INCLUDE_DIR PAM_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PAM DEFAULT_MSG PAM_LIBRARY PAM_INCLUDE_DIR)

INCLUDE_DIRECTORIES(${PAM_INCLUDE_DIR} ${DPM_COMMON})

TARGET_LINK_LIBRARIES(${PAM_ZONE_NAME}  ${PAM_LIBRARY} dpm-common)
TARGET_LINK_LIBRARIES(${PAM_ZONE_CLI_NAME}  ${PAM_LIBRARY} dpm-common)

TARGET_COMPILE_DEFINITIONS(${PAM_ZONE_NAME} PRIVATE
    CONF_PATH="${CONF_INSTALL_DIR}"
)

INSTALL(TARGETS ${PAM_ZONE_NAME} DESTINATION ${LIB_INSTALL_DIR}/security)
INSTALL(TARGETS ${PAM_ZONE_CLI_NAME} DESTINATION sbin)
INSTALL(FILES pam.d/nsattach DESTINATION ${PAMD_INSTALL_DIR})
INSTALL(FILES pam.d/systemd-user-zone DESTINATION ${PAMD_INSTALL_DIR})

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

PROJECT(dpm-pam-unshare)

FILE(GLOB PAM_UNSHARE_SRCS      module/*.cxx)
FILE(GLOB PAM_UNSHARE_CLI_SRCS  cli/*.cxx
                                ${DPM_COMMON}/pam-app.cxx
)

FILE(GLOB DPM_COMMON_SRCS       ${DPM_COMMON}/error.cxx
                                ${DPM_COMMON}/parameter.cxx
                                ${DPM_COMMON}/filesystem.cxx
                                ${DPM_COMMON}/access-control.cxx
                                ${DPM_COMMON}/shadow/user.cxx
                                ${DPM_COMMON}/shadow/group.cxx
                                ${DPM_COMMON}/shadow/lock.cxx
                                ${DPM_COMMON}/xml/parser.cxx
                                ${DPM_COMMON}/xml/document.cxx
                                ${DPM_COMMON}/xml/node.cxx
                                ${DPM_COMMON}/xml/keepblanks.cxx
)


SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

SET(PAM_UNSHARE_NAME "pam_unshare")
ADD_LIBRARY(${PAM_UNSHARE_NAME} MODULE ${PAM_UNSHARE_SRCS} ${DPM_COMMON_SRCS})
SET_TARGET_PROPERTIES(${PAM_UNSHARE_NAME} PROPERTIES  PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_INSTALL_DIR}/pam-unshare"
    COMPILE_FLAGS "-fvisibility=hidden"
)

SET(PAM_UNSHARE_CLI_NAME "nsattach")
ADD_EXECUTABLE(${PAM_UNSHARE_CLI_NAME} ${PAM_UNSHARE_CLI_SRCS} ${DPM_COMMON_SRCS})
SET_TARGET_PROPERTIES(${PAM_UNSHARE_CLI_NAME} PROPERTIES PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_INSTALL_DIR}/pam-unshare"
    COMPILE_FLAGS "-fPIE"
    LINK_FLAGS "-pie"
)

FIND_PATH(PAM_INCLUDE_DIR NAMES security/pam_appl.h security/pam_ext.h security/pam_modules.h
    HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES include)
FIND_LIBRARY(PAM_LIBRARY pam  HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES ${LIB_INSTALL_DIR})
MARK_AS_ADVANCED(PAM_INCLUDE_DIR PAM_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PAM DEFAULT_MSG PAM_LIBRARY PAM_INCLUDE_DIR)

PKG_CHECK_MODULES(DPM_COMMON_DEPS REQUIRED libtzplatform-config libsmack libxml-2.0)

INCLUDE_DIRECTORIES(${PAM_INCLUDE_DIR} ${DPM_COMMON} ${DPM_COMMON_DEPS_INCLUDE_DIRS})

TARGET_LINK_LIBRARIES(${PAM_UNSHARE_NAME} ${pkgs_LDFLAGS} ${PAM_LIBRARY} ${DPM_COMMON_DEPS_LIBRARIES} pthread)
TARGET_LINK_LIBRARIES(${PAM_UNSHARE_CLI_NAME} ${pkgs_LDFLAGS} ${PAM_LIBRARY} ${DPM_COMMON_DEPS_LIBRARIES})

INSTALL(TARGETS ${PAM_UNSHARE_NAME} DESTINATION ${LIB_INSTALL_DIR}/security)
INSTALL(TARGETS ${PAM_UNSHARE_CLI_NAME} DESTINATION sbin)
INSTALL(FILES pam.d/nsattach DESTINATION ${PAMD_INSTALL_DIR})
INSTALL(FILES pam.d/systemd-user DESTINATION ${PAMD_INSTALL_DIR})

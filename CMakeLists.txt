# Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#
#
# @file   CMakeLists.txt
# @author Sungbae Yoo (sungbae.yoo@samsung.com)
#

PROJECT(PAM)

MESSAGE(STATUS "")
MESSAGE(STATUS "Generating makefile for the PAM...")

## Directories #################################################################
IF(NOT DEFINED LIB_INSTALL_DIR)
    SET(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
ENDIF(NOT DEFINED LIB_INSTALL_DIR)

IF(NOT DEFINED RUN_DIR)
    SET(RUN_DIR "/var/run")
ENDIF(NOT DEFINED RUN_DIR)


FILE(GLOB PAM_VASUM_SRCS      src/*.c)
FILE(GLOB PAM_VASUM_CLI_SRCS  cli/*.c)

## Setup target ################################################################
SET(PAM_VASUM_NAME "pam_unshare")
ADD_LIBRARY(${PAM_VASUM_NAME} MODULE ${PAM_VASUM_SRCS})
SET_TARGET_PROPERTIES(${PAM_VASUM_NAME} PROPERTIES PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_DIR}/unshare-pam"
    COMPILE_FLAGS "-pthread"
    LINK_FLAGS "-pthread"
)

SET(PAM_VASUM_CLI_NAME "nsattach")
ADD_EXECUTABLE(${PAM_VASUM_CLI_NAME} ${PAM_VASUM_CLI_SRCS})
SET_TARGET_PROPERTIES(${PAM_VASUM_CLI_NAME} PROPERTIES PREFIX ""
    COMPILE_DEFINITIONS PID_FILE_PATH="${RUN_DIR}/unshare-pam"
)

## PAM detection ###############################################################
FIND_PATH(PAM_INCLUDE_DIR NAMES security/pam_appl.h security/pam_ext.h security/pam_modules.h
    HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES include)
FIND_LIBRARY(PAM_LIBRARY pam  HINTS ${PAM_ROOT_DIR} PATH_SUFFIXES ${LIB_INSTALL_DIR})
MARK_AS_ADVANCED(PAM_INCLUDE_DIR PAM_LIBRARY)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PAM DEFAULT_MSG PAM_LIBRARY PAM_INCLUDE_DIR)

SET(PAM_INCLUDE_DIRS ${PAM_INCLUDE_DIR})
SET(PAM_LIBRARIES ${PAM_LIBRARY})
INCLUDE_DIRECTORIES(${PAM_INCLUDE_DIRS})

## Link libraries ##############################################################
TARGET_LINK_LIBRARIES(${PAM_VASUM_NAME} ${pkgs_LDFLAGS} ${PAM_LIBRARIES})
TARGET_LINK_LIBRARIES(${PAM_VASUM_CLI_NAME} ${pkgs_LDFLAGS} ${PAM_LIBRARIES})

## Install #####################################################################
INSTALL(TARGETS ${PAM_VASUM_NAME} DESTINATION ${LIB_INSTALL_DIR}/security)
INSTALL(TARGETS ${PAM_VASUM_CLI_NAME} DESTINATION sbin)
INSTALL(FILES pam.d/attach DESTINATION /etc/pam.d)
INSTALL(FILES pam.d/systemd-user-unshare DESTINATION /etc/pam.d)
INSTALL(FILES security/unshare_fstab DESTINATION /etc/security)

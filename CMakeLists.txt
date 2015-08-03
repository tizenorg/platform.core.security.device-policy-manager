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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(user-monitor C)
SET(VERSION_MAJOR 0)
SET(VERSION "${VERSION_MAJOR}.1.0")
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(LIBDIR "\${prefix}/lib")
SET(INCLUDEDIR "\${prefix}/include")

SET(LIBS        ${PROJECT_SOURCE_DIR}/src/libs)
SET(SERVER      ${PROJECT_SOURCE_DIR}/src/server)
SET(INCLUDE_DIR src/include)

INCLUDE_DIRECTORIES(${INCLUDE_DIR})

INCLUDE(FindPkgConfig)

# Setup this later
# SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -Wextra -g")

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")

ADD_SUBDIRECTORY(${LIBS})
ADD_SUBDIRECTORY(${SERVER})

INSTALL(DIRECTORY ${INCLUDE_DIR}/ DESTINATION ${INCLUDEDIR}
            FILES_MATCHING
            PATTERN "*_private.h" EXCLUDE
            PATTERN "${INCLUDE_DIR}/*.h"
        )



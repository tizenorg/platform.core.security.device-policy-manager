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

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

PROJECT(dpm C)
SET(VERSION_MAJOR 0)
SET(VERSION "${VERSION_MAJOR}.0.1")

SET(LIBNAME "${PROJECT_NAME}")

INCLUDE(FindPkgConfig)

pkg_check_modules(libdpm_pkgs REQUIRED
	gio-2.0
	libgum
)

FOREACH(flag ${libdpm_pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} -fPIC")

SET(SOURCES
		log.c
		dp-bus.c
		admin.c
		connect.c
)

SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack")

ADD_LIBRARY(${LIBNAME} SHARED ${SOURCES} )
TARGET_LINK_LIBRARIES(${LIBNAME} ${libdpm_pkgs_LDFLAGS} pthread)
SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES SOVERSION ${VERSION_MAJOR})
SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES VERSION ${VERSION})

INSTALL(TARGETS ${LIBNAME} DESTINATION ${LIBDIR})

CONFIGURE_FILE(libdpm.pc.in libdpm.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/libdpm.pc DESTINATION lib/pkgconfig)


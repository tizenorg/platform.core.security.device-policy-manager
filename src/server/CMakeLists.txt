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
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

PROJECT(dpm-server C)
SET(VERSION_MAJOR 0)
SET(VERSION "${VERSION_MAJOR}.0.1")

SET(BINNAME "${PROJECT_NAME}")
INCLUDE(FindPkgConfig)

pkg_check_modules(dpmsvc_pkgs REQUIRED
			libsystemd-journal
			glib-2.0
			libgum
		)

FOREACH(flag ${dpmsvc_pkgs_CFLAGS})
	SET(dpmsvc_pkgs_CFLAGS "${dpmsvc_pkgs_CFLAGS} ${flag}")
ENDFOREACH(flag)

INCLUDE_DIRECTORIES(${LIBS}
		${dpmsvc_pkgs_INCLUDE_DIRS}
	)

SET(SOURCES main.c
	   )


ADD_EXECUTABLE(${BINNAME} ${SOURCES} ${INCLUDE_SRCS} )
TARGET_LINK_LIBRARIES(${BINNAME} ${dpmvc_pkgs_LDFLAGS} dpm )

INSTALL(TARGETS ${BINNAME} DESTINATION bin)


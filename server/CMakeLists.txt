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

SET(SOURCES	main.cpp
			server.cpp
			policy.cpp
			policy-storage.cpp
			client-manager.cpp
			administration.cpp
			security.cpp
			password.cpp
			zone.cpp
			app-bundle.cpp
			syspopup.cpp
			launchpad.cpp
			packman.cpp
)

ADD_EXECUTABLE(${TARGET} ${SOURCES} ${INCLUDE_SRCS})

PKG_CHECK_MODULES(SERVER_DEPS REQUIRED glib-2.0 gio-2.0 bundle aul appsvc pkgmgr pkgmgr-info vconf syspopup-caller deviced libtzplatform-config auth-fw-admin)

INCLUDE_DIRECTORIES(SYSTEM ${SERVER_DEPS_INCLUDE_DIRS} ${DPM_COMMON} ${DPM_POLICY} ${DPM_SERVER})

TARGET_LINK_LIBRARIES(${TARGET} ${SERVER_DEPS_LIBRARIES} pthread dpm-common)

SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "-fPIE")
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "-pie")

TARGET_COMPILE_DEFINITIONS(${TARGET} PRIVATE
    CONF_PATH="${CONF_INSTALL_DIR}"
    DATA_PATH="${DATA_INSTALL_DIR}"
    DB_PATH="${DB_INSTALL_DIR}"
)

INSTALL(TARGETS ${TARGET} DESTINATION bin)
INSTALL(FILES data/PolicyManifest.xml DESTINATION ${CONF_INSTALL_DIR}/policy)
INSTALL(FILES data/DefaultBundleManifest.xml DESTINATION ${CONF_INSTALL_DIR}/zone RENAME owner.xml)

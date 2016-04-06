CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(org.tizen.dpm-syspopup C)

INCLUDE_DIRECTORIES(./include)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(dpm_syspopup_pkgs REQUIRED
        dlog
        glib-2.0
	bundle
	efl-extension
	elementary
	capi-appfw-application
	evas
	capi-system-system-settings
)

INCLUDE_DIRECTORIES(${dpm_syspopup_pkgs_INCLUDE_DIRS} ${DPM_LIBS})
LINK_DIRECTORIES(${dpm_syspopup_pkgs_LIBRARY_DIRS})

SET(PKG_NAME "${PROJECT_NAME}")
SET(PKG_SRC src/main.c
            src/ui.c
            src/popup-list.c)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

ADD_EXECUTABLE(${PKG_NAME} ${PKG_SRC})
INSTALL(TARGETS ${PKG_NAME} DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/bin)

TARGET_LINK_LIBRARIES(${PKG_NAME} ${dpm_syspopup_pkgs_LIBRARIES})

#Install app data
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PKG_NAME}.xml DESTINATION ${APP_SHARE_PACKAGES_DIR})

# i18n
ADD_SUBDIRECTORY(po)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(org.tizen.kaskit C)

INCLUDE_DIRECTORIES(./include)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(launcher_pkgs REQUIRED
	dlog
	glib-2.0
	bundle
	efl-extension
	elementary
	evas
	capi-appfw-application
	capi-appfw-package-manager
	capi-appfw-app-manager
	badge
)

INCLUDE_DIRECTORIES(${launcher_pkgs_INCLUDE_DIRS} ${ZONE_LIBS})
LINK_DIRECTORIES(${launcher_pkgs_LIBRARY_DIRS})

SET(PKG_NAME "${PROJECT_NAME}")
SET(PKG_SRC src/main.c
            src/ui.c
            src/widget.c)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

ADD_EXECUTABLE(${PKG_NAME} ${PKG_SRC})
INSTALL(TARGETS ${PKG_NAME} DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/bin)

TARGET_LINK_LIBRARIES(${PKG_NAME} ${launcher_pkgs_LIBRARIES})

#Create edj file
ADD_CUSTOM_TARGET(org.tizen.kaskit.edj
	COMMAND edje_cc -no-save -id ${CMAKE_CURRENT_SOURCE_DIR}/res/images
	${CMAKE_CURRENT_SOURCE_DIR}/res/edje/org.tizen.kaskit.edc
	${CMAKE_BINARY_DIR}/org.tizen.kaskit.edj
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/edje/org.tizen.kaskit.edc)

ADD_DEPENDENCIES(${PROJECT_NAME} org.tizen.kaskit.edj)
INSTALL(FILES ${CMAKE_BINARY_DIR}/org.tizen.kaskit.edj DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/res)

INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/images/default_app_icon.png DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/res/images)

#Install app data
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PKG_NAME}.xml DESTINATION ${APP_SHARE_PACKAGES_DIR})

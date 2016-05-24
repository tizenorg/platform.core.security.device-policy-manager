CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(org.tizen.zone-setup-wizard C)

INCLUDE_DIRECTORIES(./include)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(setup_pkgs REQUIRED
	dlog
	glib-2.0
	bundle
	efl-extension
	elementary
	capi-appfw-application
	evas
	notification
)

INCLUDE_DIRECTORIES(${setup_pkgs_INCLUDE_DIRS} ${ZONE_LIBS})
LINK_DIRECTORIES(${setup_pkgs_LIBRARY_DIRS})

SET(PKG_NAME "${PROJECT_NAME}")
SET(PKG_SRC src/main.c
            src/ui.c
            src/util.c
            src/widget.c)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

ADD_EXECUTABLE(${PKG_NAME} ${PKG_SRC})
INSTALL(TARGETS ${PKG_NAME} DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/bin)

TARGET_LINK_LIBRARIES(${PKG_NAME} ${setup_pkgs_LIBRARIES} zone)

#Create edj file
ADD_CUSTOM_TARGET(org.tizen.zone-setup-wizard.edj
	COMMAND edje_cc -no-save -id ${CMAKE_CURRENT_SOURCE_DIR}/res/images
	${CMAKE_CURRENT_SOURCE_DIR}/res/org.tizen.zone-setup-wizard.edc
	${CMAKE_BINARY_DIR}/org.tizen.zone-setup-wizard.edj
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/org.tizen.zone-setup-wizard.edc)

ADD_DEPENDENCIES(${PROJECT_NAME} org.tizen.zone-setup-wizard.edj)
INSTALL(FILES ${CMAKE_BINARY_DIR}/org.tizen.zone-setup-wizard.edj DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/res)

#Install app data
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PKG_NAME}.xml DESTINATION ${APP_SHARE_PACKAGES_DIR})
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/data/ZoneManifest.xml DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/res/data)

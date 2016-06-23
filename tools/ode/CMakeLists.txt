SET(ODE_TARGET org.tizen.ode)

INCLUDE_DIRECTORIES(./include)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(tools_pkgs REQUIRED
		dlog
		glib-2.0
		efl-extension
		elementary
		capi-appfw-application
		libxml-2.0
		capi-system-system-settings
		evas
		capi-system-device
		capi-system-runtime-info
		vconf
		capi-appfw-app-control
)

INCLUDE_DIRECTORIES(${tools_pkgs_INCLUDE_DIRS})
LINK_DIRECTORIES(${tools_pkgs_LIBRARY_DIRS})

SET(PKG_NAME "${ODE_TARGET}")
SET(PKG_SRC ./src/ode-app.c
			./src/ode-app-widget.c
			./src/ode-app-main.c
			./src/ode-app-confirm.c)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pie")

ADD_EXECUTABLE(${PKG_NAME} ${PKG_SRC})
INSTALL(TARGETS ${PKG_NAME} DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/bin)

TARGET_LINK_LIBRARIES(${PKG_NAME} ${tools_pkgs_LIBRARIES})

#Create edj file
ADD_CUSTOM_TARGET(org.tizen.ode.edj
    COMMAND edje_cc -no-save -id ${CMAKE_CURRENT_SOURCE_DIR}/res/images
    ${CMAKE_CURRENT_SOURCE_DIR}/res/org.tizen.ode.edc
    ${CMAKE_BINARY_DIR}/org.tizen.ode.edj
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/res/org.tizen.ode.edc)

ADD_DEPENDENCIES(${PROJECT_NAME} org.tizen.ode.edj)
INSTALL(FILES ${CMAKE_BINARY_DIR}/org.tizen.ode.edj DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/res)

#Install app data
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/images/icon_encrypt_bar.png DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/res/images)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/images/icon_encrypt_check.png DESTINATION ${APP_INSTALL_PREFIX}/${PKG_NAME}/res/images)
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PKG_NAME}.xml DESTINATION ${APP_SHARE_PACKAGES_DIR})

cmake_minimum_required(VERSION 3.4.0)

project(lib VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(libcontemporary REQUIRED)
find_package(ttexteditor REQUIRED)

cntp_sourcelist(
    OUTPUT HEADERS
    OUTPUT_ALL ALL_HEADERS
    FILES
        workspacefile.cpp
        zzzrequesttreeitem.cpp
        zzzrequest.cpp
        zzzreply.cpp
        zzzreplymanager.cpp
        zzznetworkcache.cpp
        zzzhelpers.cpp
        zzzrequestfolder.cpp

        providers/zzzprovider.cpp
        providers/zzzprovides.cpp
        providers/identifierprovider.cpp
        providers/headersprovider.cpp
        providers/requestcontainerprovider.cpp
        providers/endpointprovider.cpp
        providers/bodyprovider.cpp
        providers/descriptionprovider.cpp
        providers/environmentprovider.cpp
        providers/settingsprovider.cpp

        widgets/zzzworkspaceeditor.cpp widgets/zzzworkspaceeditor.ui
        widgets/zzzrequesteditor.cpp widgets/zzzrequesteditor.ui
        widgets/endpointprovidereditor.cpp widgets/endpointprovidereditor.ui
        widgets/zzzreplyviewer.cpp widgets/zzzreplyviewer.ui
        widgets/zzzreplyinspector.cpp widgets/zzzreplyinspector.ui
        widgets/zzzrequesttreewidget.cpp

        widgets/replyinspector/replyinspectorwidget.cpp
        widgets/replyinspector/replybodywidget.cpp widgets/replyinspector/replybodywidget.ui
        widgets/replyinspector/replyheaderswidget.cpp widgets/replyinspector/replyheaderswidget.ui
        widgets/replyinspector/jsonbodywidget.cpp widgets/replyinspector/jsonbodywidget.ui

        widgets/providereditor/providereditor.cpp
        widgets/providereditor/bodyprovidereditor.cpp widgets/providereditor/bodyprovidereditor.ui
        widgets/providereditor/headerprovidereditor.cpp widgets/providereditor/headerprovidereditor.ui
        widgets/providereditor/descriptionprovidereditor.cpp widgets/providereditor/descriptionprovidereditor.ui
        widgets/providereditor/environmentprovidereditor.cpp widgets/providereditor/environmentprovidereditor.ui
        widgets/providereditor/settingsprovidereditor.cpp widgets/providereditor/settingsprovidereditor.ui

        libthezzz_icons.qrc
    )

cntp_sourcelist(
    OUTPUT HEADERS
    OUTPUT_ALL ALL_HEADERS
    FILES
        libthezzz_global.h
        forwarddeclares.h

        workspacefile.h
        zzzrequesttreeitem.h
        zzzrequest.h
        zzzreply.h
        zzzreplymanager.h
        zzznetworkcache.h
        zzzhelpers.h
        zzzrequestfolder.h

        providers/zzzprovider.h
        providers/zzzprovides.h
        providers/identifierprovider.h
        providers/headersprovider.h
        providers/requestcontainerprovider.h
        providers/endpointprovider.h
        providers/bodyprovider.h
        providers/descriptionprovider.h
        providers/environmentprovider.h
        providers/settingsprovider.h

        widgets/zzzworkspaceeditor.h
        widgets/zzzrequesteditor.h
        widgets/endpointprovidereditor.h
        widgets/zzzreplyviewer.h
        widgets/zzzreplyinspector.h
        widgets/zzzrequesttreewidget.h

        widgets/replyinspector/replyinspectorwidget.h
        widgets/replyinspector/replybodywidget.h
        widgets/replyinspector/replyheaderswidget.h

        widgets/providereditor/providereditor.h
        widgets/providereditor/bodyprovidereditor.h
        widgets/providereditor/headerprovidereditor.h
        widgets/providereditor/descriptionprovidereditor.h
        widgets/providereditor/environmentprovidereditor.h
        widgets/replyinspector/jsonbodywidget.h
        widgets/providereditor/settingsprovidereditor.h
    )

set(PRIVATE_HEADERS

)

add_library(libthezzz SHARED ${SOURCES} ${HEADERS} ${PRIVATE_HEADERS})
cntp_init(libthezzz 20)
set_target_properties(libthezzz PROPERTIES
        OUTPUT_NAME thezzz
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.vicr123.thezzz
        VERSION 1.0.0
        PUBLIC_HEADER "${HEADERS}"
        TRANSLATION_SOURCES "${ALL_SOURCES};${ALL_HEADERS}"
        CNTP_DATA_SUBDIR thezzz/libthezzz)
cntp_translate(libthezzz)

target_link_libraries(libthezzz Qt::Widgets libcontemporary ttexteditor)
target_compile_definitions(libthezzz PRIVATE LIBTHEZZZ_LIBRARY)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/libthezzz.framework/Resources/CMake)
    set(HEADER_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
    set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/libthezzz.framework/Headers)
    set(LIBRARY_INSTALL_DIR ../)
ELSE()
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/libthezzz)
    set(HEADER_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/libthezzz)
    set(INCLUDE_INSTALL_DIR ${HEADER_INSTALL_DIR})
    set(LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
ENDIF()

configure_package_config_file(libthezzzConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/libthezzzConfig.cmake
        INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        PATH_VARS HEADER_INSTALL_DIR LIBRARY_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libthezzzConfig.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(EXPORT libthezzzTargets
        FILE libthezzzTargets.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(TARGETS libthezzz
        EXPORT libthezzzTargets
        PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_DIR}
        INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})

cntp_mark_support_library_directory()

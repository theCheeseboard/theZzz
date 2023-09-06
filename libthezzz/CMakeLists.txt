cmake_minimum_required(VERSION 3.4.0)

project(lib VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(libcontemporary REQUIRED)

set(SOURCES
    workspacefile.cpp
    zzzrequesttreeitem.cpp
    zzzrequest.cpp
    zzzreply.cpp
    zzzreplymanager.cpp
    zzznetworkcache.cpp
    zzzhelpers.cpp

    providers/zzzprovider.cpp
    providers/zzzprovides.cpp
    providers/headersprovider.cpp
    providers/requestcontainerprovider.cpp
    providers/endpointprovider.cpp

    widgets/zzzworkspaceeditor.cpp widgets/zzzworkspaceeditor.ui
    widgets/zzzrequesteditor.cpp widgets/zzzrequesteditor.ui
    widgets/endpointprovidereditor.cpp widgets/endpointprovidereditor.ui
    widgets/zzzreplyviewer.cpp widgets/zzzreplyviewer.ui
    widgets/zzzreplyinspector.cpp widgets/zzzreplyinspector.ui

    widgets/replyinspector/replyinspectorwidget.cpp
    widgets/replyinspector/replybodywidget.cpp widgets/replyinspector/replybodywidget.ui
    widgets/replyinspector/replyheaderswidget.cpp widgets/replyinspector/replyheaderswidget.ui

    libthezzz_icons.qrc
)

set(HEADERS
    libthezzz_global.h
    forwarddeclares.h

    workspacefile.h
    zzzrequesttreeitem.h
    zzzrequest.h
    zzzreply.h
    zzzreplymanager.h
    zzznetworkcache.h
    zzzhelpers.h

    providers/zzzprovider.h
    providers/zzzprovides.h
    providers/headersprovider.h
    providers/requestcontainerprovider.h
    providers/endpointprovider.h

    widgets/zzzworkspaceeditor.h
    widgets/zzzrequesteditor.h
    widgets/endpointprovidereditor.h
    widgets/zzzreplyviewer.h
    widgets/zzzreplyinspector.h

    widgets/replyinspector/replyinspectorwidget.h
    widgets/replyinspector/replybodywidget.h
    widgets/replyinspector/replyheaderswidget.h
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
        PUBLIC_HEADER "${HEADERS}")

target_link_libraries(libthezzz Qt::Widgets libcontemporary)
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

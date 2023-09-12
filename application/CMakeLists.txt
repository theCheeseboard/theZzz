cmake_minimum_required(VERSION 3.21.0)

project(thezzz VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(libcontemporary REQUIRED)
find_package(libthebranch REQUIRED)

cntp_sourcelist(
    OUTPUT SOURCES
    OUTPUT_ALL ALL_SOURCES
    FILES
        main.cpp
        mainwindow.cpp mainwindow.ui
        landingpage.cpp landingpage.ui
)

cntp_sourcelist(
    OUTPUT HEADERS
    OUTPUT_ALL ALL_HEADERS
    FILES
        mainwindow.h
        landingpage.h
)

cntp_sourcelist(
    OUTPUT SOURCES
    OUTPUT_ALL ALL_SOURCES
    TARGET_SYSTEM_NAME "Darwin"
    FILES
        mainwindow-mac.mm
)

# Find Apple lproj files
set(LPROJ_SOURCES)
file(GLOB LPROJ_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/translations/apple-lproj/*.lproj)
FOREACH(DIR ${LPROJ_DIRS})
    cmake_path(GET DIR FILENAME DIR_NAME)
    file(GLOB LPROJ_FILES ${DIR}/*.strings)
    FOREACH(FILE ${LPROJ_FILES})
        list(APPEND LPROJ_SOURCES ${FILE})
        set_source_files_properties(${FILE} PROPERTIES
                MACOSX_PACKAGE_LOCATION Resources/${DIR_NAME})
    ENDFOREACH()
ENDFOREACH()
set(SOURCES ${SOURCES} ${LPROJ_SOURCES})

add_executable(thezzz ${SOURCES} ${HEADERS}
    Info.plist.in)

set_target_properties(thezzz PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
        MACOSX_BUNDLE_GUI_IDENTIFIER com.vicr123.thezzz
        WIN32_EXECUTABLE TRUE
        TRANSLATION_SOURCES "${ALL_SOURCES};${ALL_HEADERS}"
        CNTP_DATA_SUBDIR thezzz)

cntp_init(thezzz 20)
cntp_translate(thezzz)
cntp_target_name(thezzz "theZzz")
cntp_app_icon(thezzz
    BASEICON icons/baseicon.svg
    BASEICON_MAC icons/baseicon-mac.svg)
cntp_target_desktop_file(thezzz
    DESKTOP_FILE com.vicr123.thezzz.desktop
    BLUEPRINT_DESKTOP_FILE com.vicr123.thezzz_blueprint.desktop)
cntp_defaults_file(thezzz
    DEFAULTS_FILE thezzz.conf)
cntp_dmg(thezzz app-dmg-background.svg)

target_link_libraries(thezzz Qt::Widgets libcontemporary libthezzz libthebranch)
target_include_directories(thezzz PUBLIC ../libthezzz/)

install(TARGETS thezzz
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        BUNDLE DESTINATION /Applications)

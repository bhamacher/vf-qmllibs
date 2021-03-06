project(notificationmanager VERSION 1.0 LANGUAGES CXX C)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Debug type: " ${CMAKE_BUILD_TYPE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -O0 -g")
endif()
# Tell CMake to call moc automaticly
set(CMAKE_AUTOMOC ON)



set(PROJECT_VERSION_MAJOR "0")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "1")

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")


include(FeatureSummary)
include(GNUInstallDirs)


find_package(Qt5 CONFIG REQUIRED COMPONENTS Core DBus Xml Gui Qml Quick)
find_package(SortFilterProxyModelCmake REQUIRED)
find_package(KF5NetworkManagerQt REQUIRED)
find_package(KF5ModemManagerQt REQUIRED)



MACRO(HEADER_DIRECTORIES dir return_list)
FILE(GLOB_RECURSE new_list ${dir}/*.h)
SET(dir_list "")
FOREACH(file_path ${new_list})
GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
SET(dir_list ${dir_list} ${dir_path})
ENDFOREACH()
LIST(REMOVE_DUPLICATES dir_list)
SET(${return_list} ${dir_list})
ENDMACRO()



file(GLOB_RECURSE APP_HEADERS
    src/*.h
    )

file(GLOB_RECURSE APP_SOURCES
    src/*.cpp
    )

set(OTHER_FILES qmldir plugins.qmltypes)

HEADER_DIRECTORIES("src" header_dir_list)
include_directories(${header_dir_list})

qt5_add_resources(PLUGIN_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/notificationmanager.qrc)

add_library(
    notificationmanager
    SHARED

    ${APP_HEADERS}
    ${APP_SOURCES}
    ${OTHER_FILES}

    ${PLUGIN_RESOURCES}     # Plugin resources file
    )


target_compile_features(
    notificationmanager
    PRIVATE
    cxx_auto_type
    cxx_generic_lambdas
    )

target_link_libraries(
    notificationmanager
    PUBLIC
    Qt5::Core
    Qt5::Quick
    )


target_include_directories(notificationmanager
    PUBLIC
    $<INSTALL_INTERFACE:include/notificationmanager>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${header_dir_list}
    )

#set target Version
set_target_properties(notificationmanager PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(notificationmanager PROPERTIES SOVERSION ${notificationmanager_VERSION_MAJOR})

#generate export header
include(GenerateExportHeader)
generate_export_header(notificationmanager)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/notificationmanager_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/notificationmanager
    )


# install library
install(TARGETS notificationmanager
    EXPORT notificationmanagerExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )



# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    notificationmanagerConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )


# configure *Config.cmake
configure_file(notificationmanagerConfig.cmake.in notificationmanagerConfig.cmake @ONLY)


# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/notificationmanagerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/notificationmanagerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/notificationmanager
    )


# install library
install(TARGETS notificationmanager
    EXPORT notificationmanager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )


# install targets cmake-files
install(EXPORT notificationmanagerExport
    FILE notificationmanagerTargets.cmake
    NAMESPACE ZeraQml::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/notificationmanager
    )

# install public headers
install(
    FILES ${APP_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/notificationmanager
    )


#install(
#    FILES ${OTHER_FILES}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/notificationmanager/notificationmanager
#    )



#This is probably not needed anymore

# During plugin searching Qt will be looking for 'qmldir' file
# So we should place it next to our plugin lib.
#add_custom_command(
#    TARGET notificationmanager
#    POST_BUILD
#    COMMAND
#        ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_LIST_DIR}/qmldir
#        $<TARGET_FILE_DIR:notificationmanager>/qmldir
#    COMMAND
#        ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_LIST_DIR}/plugins.qmltypes
#        $<TARGET_FILE_DIR:notificationmanager>/plugins.qmltypes
#)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

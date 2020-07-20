project(networkLib VERSION 1.0 LANGUAGES CXX C) 

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Debug type: " ${CMAKE_BUILD_TYPE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -O0 -g")
endif()
# Tell CMake to call moc automaticly
set(CMAKE_AUTOMOC ON)



set(vfnetworkmanager_VERSION_MAJOR "0")
set(vfnetworkmanager_VERSION_MINOR "0")
set(vfnetworkmanager_VERSION_PATCH "1")

set(PROJECT_VERSION "${vfnetworkmanager_VERSION_MAJOR}.${vfnetworkmanager_VERSION_MINOR}.${vfnetworkmanager_VERSION_PATCH}")


include(FeatureSummary)
include(GNUInstallDirs)


find_package(Qt5 CONFIG REQUIRED COMPONENTS Core DBus Xml Gui Qml Quick)
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

qt5_add_resources(PLUGIN_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/plugin.qrc)

add_library(
    vfnetworkmanager
    SHARED

    ${APP_HEADERS}
    ${APP_SOURCES}
    ${OTHER_FILES}

    ${PLUGIN_RESOURCES}     # Plugin resources file
    )


target_compile_features(
    vfnetworkmanager
    PRIVATE
    cxx_auto_type
    cxx_generic_lambdas
    )

target_link_libraries(
    vfnetworkmanager

    PUBLIC
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::DBus
    Qt5::Xml

    KF5::NetworkManagerQt
    KF5::ModemManagerQt
    )


target_include_directories(vfnetworkmanager
    PUBLIC
    $<INSTALL_INTERFACE:include/vfnetworkmanager>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${header_dir_list}

    )

#set target Version
set_target_properties(vfnetworkmanager PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(vfnetworkmanager PROPERTIES SOVERSION ${vfnetworkmanager_VERSION_MAJOR})

#generate export header
include(GenerateExportHeader)
generate_export_header(vfnetworkmanager)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/vfnetworkmanager_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vfnetworkmanager
    )

# install library
install(TARGETS vfnetworkmanager
    EXPORT vfnetworkmanagerExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )



# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    vfnetworkmanagerConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )


# configure *Config.cmake
configure_file(vfnetworkmanagerConfig.cmake.in vfnetworkmanagerConfig.cmake @ONLY)


# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vfnetworkmanagerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/vfnetworkmanagerConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vfnetworkmanager
    )


# install library
install(TARGETS vfnetworkmanager
    EXPORT vfnetworkmanager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )


# install targets cmake-files
install(EXPORT vfnetworkmanagerExport
    FILE vfnetworkmanagerTargets.cmake
    NAMESPACE ZeraQml::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vfnetworkmanager
    )

# install public headers
install(
    FILES ${APP_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vfnetworkmanager
    )


#install(
#    FILES ${OTHER_FILES}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/vfnetworkmanager/vfnetworkmanager
#    )



#This is probably not needed anymore

# During plugin searching Qt will be looking for 'qmldir' file
# So we should place it next to our plugin lib.
#add_custom_command(
#    TARGET vfnetworkmanager
#    POST_BUILD
#    COMMAND
#        ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_LIST_DIR}/qmldir
#        $<TARGET_FILE_DIR:vfnetworkmanager>/qmldir
#    COMMAND
#        ${CMAKE_COMMAND} -E copy
#        ${CMAKE_CURRENT_LIST_DIR}/plugins.qmltypes
#        $<TARGET_FILE_DIR:vfnetworkmanager>/plugins.qmltypes
#)


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

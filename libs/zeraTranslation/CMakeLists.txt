project(zeratranslation VERSION 1.0 LANGUAGES CXX C) 

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Debug type: " ${CMAKE_BUILD_TYPE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -O0 -g")
endif()
# Tell CMake to call moc automaticly
set(CMAKE_AUTOMOC ON)



set(zeratranslation_VERSION_MAJOR "0")
set(zeratranslation_VERSION_MINOR "0")
set(zeratranslation_VERSION_PATCH "1")

set(PROJECT_VERSION "${zeratranslation_VERSION_MAJOR}.${zeratranslation_VERSION_MINOR}.${zeratranslation_VERSION_PATCH}")


include(FeatureSummary)
include(GNUInstallDirs)


find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Qml)



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

#qt5_add_resources(PLUGIN_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/zeratranslation.qrc)

add_library(
    zeratranslation
    SHARED

    ${APP_HEADERS}
    ${APP_SOURCES}
  #  ${OTHER_FILES}

    ${PLUGIN_RESOURCES}     # Plugin resources file
    )


target_compile_features(
    zeratranslation
    PRIVATE
    cxx_auto_type
    cxx_generic_lambdas
    )

target_link_libraries(
    zeratranslation
    PUBLIC
    Qt5::Core
    Qt5::Qml

    )


target_include_directories(zeratranslation
    PUBLIC
    $<INSTALL_INTERFACE:include/zeratranslation>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${header_dir_list}
    )

#set target Version
set_target_properties(zeratranslation PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(zeratranslation PROPERTIES SOVERSION ${zeratranslation_VERSION_MAJOR})

#generate export header
include(GenerateExportHeader)
generate_export_header(zeratranslation)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/zeratranslation_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zeratranslation
    )


# install library
install(TARGETS zeratranslation
    EXPORT zeratranslationExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )



# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    zeratranslationConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )


# configure *Config.cmake
configure_file(zeratranslationConfig.cmake.in zeratranslationConfig.cmake @ONLY)


# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/zeratranslationConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/zeratranslationConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zeratranslation
    )

# install targets cmake-files
install(EXPORT zeratranslationExport
    FILE zeratranslationTargets.cmake
    NAMESPACE ZeraQml::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/zeratranslation
    )

# install public headers
install(
    FILES ${APP_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zeratranslation
    )


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

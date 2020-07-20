project(zeracompLib VERSION 1.0 LANGUAGES CXX C) 

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Debug type: " ${CMAKE_BUILD_TYPE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG -O0 -g")
endif()
# Tell CMake to call moc automaticly
set(CMAKE_AUTOMOC ON)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Gui Qml Quick)

include(FeatureSummary)
include(GNUInstallDirs)

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

#set(OTHER_FILES qmldir plugins.qmltypes)

HEADER_DIRECTORIES("src" header_dir_list)
include_directories(${header_dir_list})

qt5_add_resources(PLUGIN_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/zeracomp.qrc)

add_library(
    zeracomplib
    SHARED

    ${APP_HEADERS}
    ${APP_SOURCES}
    ${OTHER_FILES}

    ${PLUGIN_RESOURCES}     # Plugin resources file
)


target_compile_features(
    zeracomplib
    PRIVATE
        cxx_auto_type
        cxx_generic_lambdas
)

target_link_libraries(
    zeracomplib
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
)

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



#target_include_directories(vfnetworkmanager
#    PUBLIC
#    $<INSTALL_INTERFACE:include/vfnetworkmanager>
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
#    PRIVATE
#    ${CMAKE_CURRENT_SOURCE_DIR}
#    )

## install library
#install(TARGETS vfnetworkmanager
#    EXPORT vfnetworkmanager
#    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/zera-qml-modules/networksettings
#    )

## configure *ConfigVersion.cmake
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
#    vfnetworkmanagerConfigVersion.cmake
#    VERSION ${PACKAGE_VERSION}
#    COMPATIBILITY SameMinorVersion
#    )

#install(
#    FILES ${OTHER_FILES}
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/zera-qml-modules/networksettings
#    )


#feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

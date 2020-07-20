cmake_minimum_required(VERSION 3.0)
project(QMLLibs VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG")

## These are generic qml libs useabel in and outside of vein applications
include(${PROJECT_SOURCE_DIR}/libs/libs.cmake)
## These are complete gui modules useabel in and outside of vein applications 
include(${PROJECT_SOURCE_DIR}/modules/modules.cmake)
## These are qml libst with direct connection to vein
#include(${PROJECT_SOURCE_DIR}/veinlibs/veinlibs.cmake)
## These are complet gui modules with direct connection to vein
#include(${PROJECT_SOURCE_DIR}/veinmodules/veinmodules.cmake)

add_subdirectory(playground)

set(OTHER_FILES README.md)


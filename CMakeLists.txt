cmake_minimum_required(VERSION 3.0)
project(QMLLibs VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_QML_DEBUG -DQT_DECLARATIVE_DEBUG")

add_subdirectory(networksettings_module)
add_subdirectory(zeracomponents_lib)
include_directories(networksettings_module/src)




add_subdirectory(playground)

set(OTHER_FILES README.md)


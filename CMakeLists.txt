cmake_minimum_required(VERSION 3.0)
project(QMLLibs VERSION 1.0 LANGUAGES CXX C)


add_subdirectory(networksettings_module)
#include_directories(networkcomponents/src)




add_subdirectory(playground)

set(OTHER_FILES README.md)


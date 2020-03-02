project(PlayApp VERSION 1.0 LANGUAGES CXX)



# Tell CMake to call moc automaticly
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Quick Core Qml Widgets DBus Xml)

find_package(KF5NetworkManagerQt REQUIRED)
find_package(KF5ModemManagerQt REQUIRED)


# Telling CMake location of our app .qrc file
qt5_add_resources(APP_RESOURCES ${CMAKE_CURRENT_LIST_DIR}/qml/app.qrc)

# We're using ${PROJECT_NAME} for target name but it's not necessary
add_executable(
    app
    main.cpp        # Our example project consists only of one .cpp file

    ${APP_RESOURCES}    # App resources file
)

target_compile_features(
   app
    PRIVATE
        cxx_auto_type
        cxx_generic_lambdas
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(
        app
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()

target_link_libraries(
    app
    PRIVATE
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::DBus
        Qt5::Xml
        Qt5::Widgets
        KF5::NetworkManagerQt
        KF5::ModemManagerQt
)

cmake_minimum_required(VERSION 3.29)
project(Project1)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        Entities
        Graph
        Algorithms
        UnitTests
        GoogleTests
)
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)


file(GLOB SOURCES_MAIN
        "main.cpp"
        "Entities/*.cpp"
        "Graph/*.cpp"
        "Algorithms/*.cpp"
        "MainWindow/*.cpp"
)
#
#file(GLOB SOURCE_TESTS
##        "UnitTests/*.cpp"
#        GoogleTests/testPlanet.cpp
#        "Entities/*.cpp"
#        "Graph/*.cpp"
#        "Algorithms/*.cpp"
#        "MainWindow/*.cpp"
#)

set(CMAKE_PREFIX_PATH "D:/Qt/6.9.3/mingw_64")

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

qt6_wrap_cpp(MOC_SOURCES Graph/GraphWidget.h)

add_library(ProjectLib STATIC
        ${SOURCES_MAIN}
        ${MOC_SOURCES}
)

target_link_libraries(ProjectLib PRIVATE Qt6::Core Qt6::Widgets)

add_executable(Project1
        main.cpp
        MainWindow/windowinterface.cpp
        MainWindow/windowinterface.h
        MainWindow/windowinterface.ui
)

target_link_libraries(Project1 PRIVATE ProjectLib Qt6::Core Qt6::Widgets)
add_executable(ProjectTests
#        GoogleTests/PlanetTests.cpp
#        GoogleTests/NebulaTests.cpp
#        GoogleTests/StarTests.cpp
        GoogleTests/GalaxyListTests.cpp
)

target_link_libraries(ProjectTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)


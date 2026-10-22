cmake_minimum_required(VERSION 3.29)
project(Project1)

set(CMAKE_CXX_STANDARD 20)

set(USE_DOUBLE_PRECISION ON CACHE BOOL "Use double precision" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Don't build demos" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Don't build demos" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "Don't build extras" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Don't build tests" FORCE)
add_compile_definitions(BT_USE_DOUBLE_PRECISION)

include(FetchContent)
FetchContent_Declare(
        bullet3
        GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
        GIT_TAG master
)
FetchContent_MakeAvailable(bullet3)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.9.3/mingw_64")
find_package(Qt6 COMPONENTS Core Widgets Quick Quick3D QuickWidgets REQUIRED)

include_directories(
        ${bullet3_SOURCE_DIR}/src
        Entities
        Graph
        Algorithms
        Algorithms/PhysicsAlgorithms
        RandomGalaxy
        Include
        MainWindow
        GalaxyView
        StarSystemView
        NebulaView
        3DView
        examples
        GoogleTests
)

qt6_wrap_cpp(MOC_SOURCES Graph/GraphWidget.h)

file(GLOB CORE_SOURCES
        "Entities/*.cpp"
        "Entities/*.h"
        "Graph/*.cpp"
        "Graph/*.h"
        "Algorithms/*.cpp"
        "Algorithms/*.h"
        "Algorithms/PhysicsAlgorithms/*.cpp"
        "RandomGalaxy/*.cpp"
)

add_library(GalaxyEngine STATIC
        ${CORE_SOURCES}
        ${MOC_SOURCES}
)
target_link_libraries(GalaxyEngine PUBLIC Qt6::Core Qt6::Widgets BulletDynamics BulletCollision LinearMath)

file(GLOB UI_SOURCES
        "MainWindow/*.cpp"
        "MainWindow/*.h"
        "MainWindow/*.ui"
        "GalaxyView/*.cpp"
        "GalaxyView/*.h"
        "GalaxyView/*.ui"
        "3DView/*.cpp"
        "3DView/*.h"
)

add_executable(Project1
        main.cpp
        ${UI_SOURCES}
        Galaxy.qrc
)
target_link_libraries(Project1 PRIVATE GalaxyEngine Qt6::Widgets Qt6::Quick Qt6::Quick3D Qt6::QuickWidgets)

add_executable(Benchmark examples/PerformanceBenchmark.cpp)
target_link_libraries(Benchmark PRIVATE GalaxyEngine)

file(GLOB TEST_SOURCES "GoogleTests/*.cpp")
add_executable(AllTests ${TEST_SOURCES})
target_link_libraries(AllTests PRIVATE GalaxyEngine gtest gtest_main Qt6::Core Qt6::Widgets)
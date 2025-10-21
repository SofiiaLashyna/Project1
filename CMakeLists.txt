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

file(GLOB SOURCE_TESTS
        "UnitTests/*.cpp"
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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

add_executable(PlanetTests GoogleTests/PlanetTests.cpp)
target_link_libraries(PlanetTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(NebulaTests GoogleTests/NebulaTests.cpp)
target_link_libraries(NebulaTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(StarTests GoogleTests/StarTests.cpp)
target_link_libraries(StarTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(StarSystemTests GoogleTests/StarSystemTests.cpp)
target_link_libraries(StarSystemTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(GalaxyListTests GoogleTests/GalaxyListTests.cpp)
target_link_libraries(GalaxyListTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(GalaxyMatrixTests GoogleTests/GalaxyMatrixTests.cpp)
target_link_libraries(GalaxyMatrixTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(EdgeTests GoogleTests/EdgeTests.cpp)
target_link_libraries(EdgeTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(VertexTests GoogleTests/VertexTests.cpp)
target_link_libraries(VertexTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(DijkstraListTests GoogleTests/DijkstraListTests.cpp)
target_link_libraries(DijkstraListTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(DijkstraMatrixTests GoogleTests/DijkstraMatrixTests.cpp)
target_link_libraries(DijkstraMatrixTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(QueueTests GoogleTests/QueueTests.cpp)
target_link_libraries(QueueTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(IsConnectedListTests GoogleTests/IsConnectedListTests.cpp)
target_link_libraries(IsConnectedListTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(IsConnectedMatrixTests GoogleTests/IsConnectedMatrixTests.cpp)
target_link_libraries(IsConnectedMatrixTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(DFSListTests GoogleTests/DFSListTests.cpp)
target_link_libraries(DFSListTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(DFSMatrixTests GoogleTests/DFSMatrixTests.cpp)
target_link_libraries(DFSMatrixTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(BFSListTests GoogleTests/BFSListTests.cpp)
target_link_libraries(BFSListTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(BFSMatrixTests GoogleTests/BFSMatrixTests.cpp)
target_link_libraries(BFSMatrixTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(GraphListTests GoogleTests/GraphListTests.cpp)
target_link_libraries(GraphListTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)

add_executable(GraphMatrixTests GoogleTests/GraphMatrixTests.cpp)
target_link_libraries(GraphMatrixTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets)


add_executable(ProjectTests
        ${SOURCE_TESTS}
)
target_link_libraries(ProjectTests PRIVATE ProjectLib Qt6::Core Qt6::Widgets)

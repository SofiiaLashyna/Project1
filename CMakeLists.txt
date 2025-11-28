cmake_minimum_required(VERSION 3.29)
project(Project1)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

set(USE_DOUBLE_PRECISION ON CACHE BOOL "Use double precision" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Don't build demos" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "Don't build demos" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "Don't build extras" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Don't build tests" FORCE)

add_compile_definitions(BT_USE_DOUBLE_PRECISION)

FetchContent_Declare(
        bullet3
        GIT_REPOSITORY https://github.com/bulletphysics/bullet3.git
        GIT_TAG master
)
FetchContent_MakeAvailable(bullet3)
include_directories(${bullet3_SOURCE_DIR}/src)

include_directories(
        Entities
        Graph
        Algorithms
        Algorithms/PhysicsAlgorithms
        GoogleTests
        GalaxyView
        RandomGalaxy
        Include
        StarSystemView
        NebulaView
        3DView
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
        "Algorithms/PhysicsAlgorithms/*.cpp"
        "MainWindow/*.cpp"
        "GalaxyView/*.cpp"
        "3DView/*.cpp"
)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.9.3/mingw_64")

find_package(Qt6 COMPONENTS Core Widgets Quick Quick3D QuickWidgets REQUIRED)
qt6_wrap_cpp(MOC_SOURCES Graph/GraphWidget.h)

add_library(ProjectLib STATIC
        ${SOURCES_MAIN}
        ${MOC_SOURCES}
        Algorithms/DijkstraPathList.h
)

target_link_libraries(ProjectLib PRIVATE Qt6::Core Qt6::Widgets Qt6::Quick Qt6::Quick3D Qt6::QuickWidgets BulletDynamics BulletCollision LinearMath)

file(GLOB SOURCES_UI
        "main.cpp"
        "MainWindow/*.cpp"
        "GalaxyView/*.cpp"
        "3DView/*.cpp"
)

add_executable(Project1
        MainWindow/windowinterface.h
        MainWindow/windowinterface.ui
        GalaxyView/galaxyview.h
        GalaxyView/galaxyview.ui
        GalaxyView/AddStarSystemDialog.h
        GalaxyView/AddNebulaDialog.h
        GalaxyView/EditStarSystemDialog.h
        GalaxyView/EditNebulaDialog.h
        GalaxyView/GalaxyEditDialog.h
        GalaxyView/AddStarSystemDialog.cpp
        GalaxyView/AddNebulaDialog.cpp
        GalaxyView/EditStarSystemDialog.cpp
        GalaxyView/EditNebulaDialog.cpp
        GalaxyView/GalaxyEditDialog.cpp
        3DView/Celestialobject3DModel.h
        3DView/CelestialObjects3DModel.cpp
        Galaxy.qrc
        3DView/galaxyview3D.cpp
        3DView/galaxyview3D.h
        3DView/galaxyview3d.h
        3DView/PlanetarySystemModel.h
        3DView/PlanetarySystemModel.cpp
)

target_link_libraries(Project1 PRIVATE ProjectLib Qt6::Core Qt6::Widgets Qt6::Quick Qt6::Quick3D Qt6::QuickWidgets BulletDynamics BulletCollision LinearMath)

add_executable(AllTests
        GoogleTests/AllTests.cpp
        GoogleTests/PlanetTests.cpp
        GoogleTests/GraphMatrixTests.cpp
        GoogleTests/GraphListTests.cpp
        GoogleTests/BFSMatrixTests.cpp
        GoogleTests/BFSListTests.cpp
        GoogleTests/DFSMatrixTests.cpp
        GoogleTests/DFSListTests.cpp
        GoogleTests/IsConnectedMatrixTests.cpp
        GoogleTests/IsConnectedListTests.cpp
        GoogleTests/QueueTests.cpp
        GoogleTests/DijkstraMatrixTests.cpp
        GoogleTests/DijkstraListTests.cpp
        GoogleTests/VertexTests.cpp
        GoogleTests/EdgeTests.cpp
        GoogleTests/GalaxyMatrixTests.cpp
        GoogleTests/GalaxyListTests.cpp
        GoogleTests/StarSystemTests.cpp
        GoogleTests/StarTests.cpp
        GoogleTests/NebulaTests.cpp
        GoogleTests/PlanetTests.cpp
        GoogleTests/RandomUtilitiesTests.cpp
        GoogleTests/GalaxyGenerationTests.cpp
)
target_link_libraries(AllTests PRIVATE ProjectLib gtest gtest_main Qt6::Core Qt6::Widgets Qt6::Quick Qt6::Quick3D Qt6::QuickWidgets)


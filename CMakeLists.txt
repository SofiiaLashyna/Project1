cmake_minimum_required(VERSION 3.29)
project(Project1)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_PREFIX_PATH "D:/Qt/6.9.3/mingw_64")

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

set(SOURCES main.cpp GraphWidget.cpp)
set(HEADERS GraphWidget.h)

qt6_wrap_cpp(MOC_SOURCES GraphWidget.h)

add_executable(Project1
        main.cpp
        Graph.h
        Queue.h
        ${MOC_SOURCES}
        CelestialObject.h
        Galaxy.h
        CelestialObject.cpp
        StarSystem.cpp
        Algorithm.h
        Nebula.h
        Nebula.cpp
        Star.h
        Star.cpp
        Planet.h
        Planet.cpp
        Dijkstra_list.h
        DFS_list.h
        BFS_list.h
        Vertex.h
        Edge.h
        Graph_matrix.h
        Graph_list.h
        StarSystem.h
#        testing.cpp
        BfsListTesting.h
        BfsMatrixTesting.h
        BFS_matrix.h
        DfsListTesting.h
        DfsMatrixTesting.h
        DFS_matrix.h
        DijkstraListTesting.h
        DijkstraMatrixTesting.h
        Dijkstra_matrix.h
        QueueTesting.h
        IsConnected_list.h
        IsConnected_matrix.h
        IsConnectedListTesting.h
        IsConnectedMatrixTesting.h
        EdgeTesting.h
        VertexTesting.h
        StarSystemTesting.h
        NebulaTesting.h
        PlanetTesting.h
        StarTesting.h
        GraphTesting.h
        GalaxyTesting.h
        GraphListMatrixTesting.h
        GraphWidget.h
        GraphWidget.cpp
)

target_link_libraries(Project1 PRIVATE Qt6::Core Qt6::Widgets)


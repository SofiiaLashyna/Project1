cmake_minimum_required(VERSION 3.29)
project(Project1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project1
#        main.cpp
        Graph.h
        Queue.h
        CelestialObject.h
        Galaxy.h
        CelestialObject.cpp
        Galaxy.cpp
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
        testing.cpp
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
)

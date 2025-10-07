cmake_minimum_required(VERSION 3.29)
project(Project1)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project1 main.cpp
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
        Dijkstra.h
        DFS.h
        BFS.h
        Vertex.h
        Edge.h
        Graph_matrix.h
        Graph_list.h
        StarSystem.h
)

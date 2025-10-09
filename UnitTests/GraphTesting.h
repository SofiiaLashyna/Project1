#ifndef GRAPHTESTING_H
#define GRAPHTESTING_H

#include <cassert>
#include <iostream>
#include <vector>
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"

template <typename GraphType>
class GraphTesting {
public:
    static void runAll() {
        testFindIndexById();
        testVertexSearch();
        testEdgeExists();
        testGetNeighbors();
    }

private:
    static void testFindIndexById() {
        GraphType g;

        g.addVertex(1, "A");
        g.addVertex(2, "B");

        assert(g.findIndexById(1) == 0 && "Index of vertex 1 should be 0");
        assert(g.findIndexById(2) == 1 && "Index of vertex 2 should be 1");
        assert(g.findIndexById(3) == -1 && "Non-existent vertex should return -1");

        std::cout << "1/4 passed\n";
    }

    static void testVertexSearch() {
        GraphType g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");

        assert(g.vertexSearch(1) == "A" && "vertexSearch for 1 should return 'A'");
        assert(g.vertexSearch(2) == "B" && "vertexSearch for 2 should return 'B'");
        assert(g.vertexSearch(3) == "none" && "vertexSearch for non-existent should return 'none'");

        std::cout << "2/4 passed\n";
    }

    static void testEdgeExists() {
        GraphType g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");

        g.addEdge(1, 2);
        Edge<std::string> inactiveEdge;
        inactiveEdge.connect(0, 2);

        assert(g.edgeExists(1, 2) == true && "Edge between 1 and 2 should exist");
        assert(g.edgeExists(1, 3) == false && "Edge between 1 and 3 should not exist");
        assert(g.edgeExists(2, 1) == true && "Edge should be undirected");

        std::cout << "3/4 passed\n";
    }

    static void testGetNeighbors() {
        GraphType g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");

        g.addEdge(1, 2);
        g.addEdge(1, 3);

        auto neighborsA = g.getNeighbors(1);
        assert(neighborsA.size() == 2 && "Vertex 1 should have 2 neighbors");
        assert((neighborsA[0] == "B" || neighborsA[1] == "B") && "Neighbors should include B");
        assert((neighborsA[0] == "C" || neighborsA[1] == "C") && "Neighbors should include C");

        auto neighborsC = g.getNeighbors(3);
        assert(neighborsC.size() == 1 && neighborsC[0] == "A");

        auto neighborsNonExistent = g.getNeighbors(5);
        assert(neighborsNonExistent.empty() && "Non-existent vertex should return empty vector");

        std::cout << "4/4 passed\n";
    }
};
#endif //GRAPHTESTING_H

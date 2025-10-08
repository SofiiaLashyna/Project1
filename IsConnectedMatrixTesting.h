#ifndef ISCONNECTEDMATRIXTESTING_H
#define ISCONNECTEDMATRIXTESTING_H
#include "Graph_matrix.h"
#include <cassert>
#include <iostream>
#include "IsConnected_matrix.h"

class IsConnectedMatrixTests {
public:
    static void runAll() {
        testMatrix_emptyGraph();
        testMatrix_singleVertex();
        testMatrix_connectedGraph();
        testMatrix_disconnectedGraph();
    }
private:
    static void testMatrix_emptyGraph() {
        Graph_matrix<std::string> g;
        IsConnectedMatrix<std::string> alg;
        assert(alg.run(g) == 1 && "Empty graph should be connected");
        std::cout << "1/4 passed\n";
    }

    static void testMatrix_singleVertex() {
        Graph_matrix<std::string> g;
        g.addVertex(1, "A");
        IsConnectedMatrix<std::string> alg;
        assert(alg.run(g) == 1 && "Single vertex graph should be connected");
        std::cout << "2/4 passed\n";
    }

    static void testMatrix_connectedGraph() {
        Graph_matrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        IsConnectedMatrix<std::string> alg;
        assert(alg.run(g) == 1 && "Connected graph should be connected");
        std::cout << "3/4 passed\n";
    }

    static void testMatrix_disconnectedGraph() {
        Graph_matrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2);
        IsConnectedMatrix<std::string> alg;
        assert(alg.run(g) == 0 && "Disconnected graph should not be connected");
        std::cout << "4/4 passed\n";
    }
};
#endif //ISCONNECTEDMATRIXTESTING_H

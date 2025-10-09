#ifndef ISCONNECTEDTESTING_H
#define ISCONNECTEDTESTING_H
#include "GraphList.h"
#include <cassert>
#include <iostream>
#include "IsConnectedList.h"

class IsConnectedListTests {
public:
    static void runAll() {
        testList_emptyGraph();
        testList_singleVertex();
        testList_connectedGraph();
        testList_disconnectedGraph();
    }
private:
    static void testList_emptyGraph() {
        GraphList<std::string> g;
        IsConnectedList<std::string> alg;
        assert(alg.run(g) == 1 && "Empty graph should be connected");
        std::cout << "1/4 passed\n";
    }

    static void testList_singleVertex() {
        GraphList<std::string> g;
        g.addVertex(1, "A");
        IsConnectedList<std::string> alg;
        assert(alg.run(g) == 1 && "Single vertex graph should be connected");
        std::cout << "2/4 passed\n";
    }

    static void testList_connectedGraph() {
        GraphList<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2);
        g.addEdge(2, 3);
        IsConnectedList<std::string> alg;
        assert(alg.run(g) == 1 && "Connected graph should be connected");
        std::cout << "3/4 passed\n";
    }

    static void testList_disconnectedGraph() {
        GraphList<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2);
        IsConnectedList<std::string> alg;
        assert(alg.run(g) == 0 && "Disconnected graph should not be connected");
        std::cout << "4/4 passed\n";
    }
};
#endif //ISCONNECTEDTESTING_H

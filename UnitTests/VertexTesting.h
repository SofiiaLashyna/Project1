#ifndef VERTEXTESTING_H
#define VERTEXTESTING_H

#include <cassert>
#include <iostream>
#include "Vertex.h"

class VertexTesting {
public:
    static void runAll() {
        testInitialState();
        testMarkVisited();
        testResetVisited();
    }

private:
    static void testInitialState() {
        Vertex<std::string> v;
        v.id = 1;
        v.data = "A";

        assert(v.visited == false && "Vertex should not be visited initially");
        assert(v.id == 1 && "Vertex id should be 1");
        assert(v.data == "A" && "Vertex data should be 'A'");

        std::cout << "1/3 passed\n";
    }

    static void testMarkVisited() {
        Vertex<int> v;
        v.markVisited();

        assert(v.visited == true && "Vertex should be marked as visited after markVisited()");
        std::cout << "2/3 passed\n";
    }

    static void testResetVisited() {
        Vertex<int> v;
        v.markVisited();
        v.resetVisited();

        assert(v.visited == false && "Vertex should not be visited after resetVisited()");
        std::cout << "3/3 passed\n";
    }
};
#endif //VERTEXTESTING_H

#ifndef GRAPHLISTMATRIXTESTING_H
#define GRAPHLISTMATRIXTESTING_H
#include <iostream>
#include <cassert>

template<typename GraphType>
class GraphUnitTest {
public:
    void runAll() {
        testAddVertex();
        testAddEdge();
        testRemoveVertex();
        testRemoveEdge();
    }

private:
    void testAddVertex() {
        GraphType g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");

        assert(g.getVertices().size() == 2);
        assert(g.findIndexById(1) != -1);
        assert(g.findIndexById(2) != -1);

        g.addVertex(1, "A");
        assert(g.getVertices().size() == 2);
        std::cout << "1/4 passed\n";
    }

    void testAddEdge() {
        GraphType g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addEdge(1, 2, 5);

        assert(g.edgeExists(1, 2));
        assert(g.edgeExists(2, 1));
        std::cout << "2/4 passed\n";
    }

    void testRemoveVertex() {
        GraphType g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addEdge(1, 2);

        g.removeVertex(1);
        assert(g.findIndexById(1) == -1);

        assert(!g.edgeExists(1, 2));
        std::cout << "3/4 passed\n";
    }

    void testRemoveEdge() {
        GraphType g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addEdge(1, 2);

        g.removeEdge(1, 2);
        assert(!g.edgeExists(1, 2));
        std::cout << "4/4 passed\n";
    }
};
#endif //GRAPHLISTMATRIXTESTING_H

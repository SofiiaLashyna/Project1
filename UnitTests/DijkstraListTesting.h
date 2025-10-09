#ifndef DIJKSTRALISTTESTING_H
#define DIJKSTRALISTTESTING_H
#include "GraphList.h"
#include "Algorithm.h"
#include <cassert>
#include <iostream>

class Dijkstra_tests_list {
public:
    static void runAll() {
        testDijkstra_basic();
        testDijkstra_noPath();
        testDijkstra_multiplePaths();
        testDijkstra_invalidNodes();
    }
private:
    static void testDijkstra_basic() {
        GraphList<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2, 4);
        g.addEdge(2, 3, 5);
        g.addEdge(1, 3, 10);

        Algorithms<GraphList<std::string>, std::string> alg(g);
        int dist = alg.Dijkstra_list(1, 3);
        assert(dist == 9);
        std::cout << "1/4 passed\n";
    }

    static void testDijkstra_noPath() {
        GraphList<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2, 3);

        Algorithms<GraphList<std::string>, std::string> alg(g);
        int dist = alg.Dijkstra_list(1, 3);
        assert(dist == -1);
        std::cout << "2/4 passed\n";
    }

    static void testDijkstra_multiplePaths() {
        GraphList<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addVertex(4, "D");
        g.addEdge(1, 2, 1);
        g.addEdge(2, 4, 1);
        g.addEdge(1, 3, 2);
        g.addEdge(3, 4, 1);

        Algorithms<GraphList<std::string>, std::string> alg(g);
        int dist = alg.Dijkstra_list(1, 4);
        assert(dist == 2);
        std::cout << "3/4 passed\n";
    }

    static void testDijkstra_invalidNodes() {
        GraphList<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");

        Algorithms<GraphList<std::string>, std::string> alg(g);
        int dist1 = alg.Dijkstra_list(0, 2);
        int dist2 = alg.Dijkstra_list(1, 3);
        assert(dist1 == -1 && dist2 == -1);
        std::cout << "4/4 passed\n";
    }
};

#endif // DIJKSTRALISTTESTING_H

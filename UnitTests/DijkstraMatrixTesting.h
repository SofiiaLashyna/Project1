#ifndef DIJKSTRAMATRIXTESTING_H
#define DIJKSTRAMATRIXTESTING_H
#include "GraphMatrix.h"
#include "Algorithm.h"
#include <cassert>
#include <iostream>

class Dijkstra_tests_matrix {
public:
    static void runAll() {
        testDijkstra_basic();
        testDijkstra_noPath();
        testDijkstra_multiplePaths();
        testDijkstra_invalidNodes();
    }
private:
    static void testDijkstra_basic() {
        GraphMatrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2, 4);
        g.addEdge(2, 3, 5);
        g.addEdge(1, 3, 10);

        Algorithms<GraphMatrix<std::string>, std::string> alg(g);
        int dist = alg.Dijkstra_matrix(1, 3);
        assert(dist == 9);
        std::cout << "1/4 passed\n";
    }

    static void testDijkstra_noPath() {
        GraphMatrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2, 3);

        Algorithms<GraphMatrix<std::string>, std::string> alg(g);
        int dist = alg.Dijkstra_matrix(1, 3);
        assert(dist == -1);
        std::cout << "2/4 passed\n";
    }

    static void testDijkstra_multiplePaths() {
        GraphMatrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addVertex(4, "D");
        g.addEdge(1, 2, 1);
        g.addEdge(2, 4, 1);
        g.addEdge(1, 3, 2);
        g.addEdge(3, 4, 1);

        Algorithms<GraphMatrix<std::string>, std::string> alg(g);
        int dist = alg.Dijkstra_matrix(1, 4); // A → D
        assert(dist == 2); // обираємо A → B → D = 1 + 1
        std::cout << "3/4 passed\n";
    }

    static void testDijkstra_invalidNodes() {
        GraphMatrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");

        Algorithms<GraphMatrix<std::string>, std::string> alg(g);
        int dist1 = alg.Dijkstra_matrix(0, 2);
        int dist2 = alg.Dijkstra_matrix(1, 3);
        assert(dist1 == -1 && dist2 == -1);
        std::cout << "4/4 passed\n";
    }
};

#endif //DIJKSTRAMATRIXTESTING_H

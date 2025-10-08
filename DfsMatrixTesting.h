#ifndef DFSMATRIXTESTING_H
#define DFSMATRIXTESTING_H

#include "Graph_matrix.h"
#include "Algorithm.h"
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>

class DFS_tests_matrix {
public:
    void testDFS_basic() {
        Graph_matrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2, 1);
        g.addEdge(1, 3, 1);

        Algorithms<Graph_matrix<std::string>, std::string> alg(g);

        std::stringstream out;
        std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

        alg.DFS_matrix(1);
        std::cout.rdbuf(oldCout);

        std::string dfsOutput = out.str();
        assert(dfsOutput.find("A") != std::string::npos && "DFS must contain A");
        assert(dfsOutput.find("B") != std::string::npos && "DFS must contain B");
        assert(dfsOutput.find("C") != std::string::npos && "DFS must contain C");
        std::cout << "1/3 passed\n";
    }

    void testDFS_emptyGraph() {
        Graph_matrix<std::string> g;
        Algorithms<Graph_matrix<std::string>, std::string> alg(g);
        alg.DFS_matrix(1);
        std::cout << "2/3 passed\n";
    }

    void testDFS_singleVertex() {
        Graph_matrix<std::string> g;
        g.addVertex(1, "A");

        Algorithms<Graph_matrix<std::string>, std::string> alg(g);
        std::stringstream out;
        std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

        alg.DFS_matrix(1);
        std::cout.rdbuf(oldCout);

        std::string dfsOutput = out.str();
        assert(dfsOutput.find("A") != std::string::npos && "Should be only one vertex A");

        int countA = 0;
        size_t pos = 0;
        while ((pos = dfsOutput.find("A", pos)) != std::string::npos) {
            countA++;
            pos += 1;
        }
        assert(countA == 1 && "Should be only one vertex");
        std::cout << "3/3 passed";
    }
};

#endif //DFSMATRIXTESTING_H

#ifndef DFSMATRIXTESTING_H
#define DFSMATRIXTESTING_H

#include "GraphMatrix.h"
#include "Algorithm.h"
#include <cassert>
#include <sstream>
#include <string>
#include <iostream>

class DFS_tests_matrix {
public:
    static void runAll() {
        testDFS_basic();
        testDFS_emptyGraph();
        testDFS_singleVertex();
    }
private:
    static void testDFS_basic() {
        GraphMatrix<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2, 1);
        g.addEdge(1, 3, 1);

        Algorithms<GraphMatrix<std::string>, std::string> alg(g);

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

    static void testDFS_emptyGraph() {
        GraphMatrix<std::string> g;
        Algorithms<GraphMatrix<std::string>, std::string> alg(g);
        alg.DFS_matrix(1);
        std::cout << "2/3 passed\n";
    }

    static void testDFS_singleVertex() {
        GraphMatrix<std::string> g;
        g.addVertex(1, "A");

        Algorithms<GraphMatrix<std::string>, std::string> alg(g);
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

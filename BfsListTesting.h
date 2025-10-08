#ifndef BFSTESTING_H
#define BFSTESTING_H
#include <cassert>
#include <sstream>
#include <string>
#include "Algorithm.h"
#include "Graph_list.h"

class BFS_tests_list {
public:
    void testBFS_basic() {
        Graph_list<std::string> g;
        g.addVertex(1, "A");
        g.addVertex(2, "B");
        g.addVertex(3, "C");
        g.addEdge(1, 2, 1);
        g.addEdge(1, 3, 1);

        Algorithms<Graph_list<std::string>, std::string> alg(g);
        std::stringstream out;
        std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

        alg.BFS_list(1);
        std::cout.rdbuf(oldCout);

        std::string bfsOutput = out.str();
        assert(bfsOutput.find("A") != std::string::npos && "Should contain A");
        assert(bfsOutput.find("B") != std::string::npos && "Should contain B");
        assert(bfsOutput.find("C") != std::string::npos && "Should contain C");
        std::cout<<"1/3 passed\n";
    }

    void testBFS_emptyGraph() {
        Graph_list<std::string> g;
        Algorithms<Graph_list<std::string>, std::string> alg(g);
        alg.BFS_list(1);

        std::cout<<"2/3 passed\n";
    }
    void testBFS_singleVertex() {
        Graph_list<std::string> g;
        g.addVertex(1, "A");

        Algorithms<Graph_list<std::string>, std::string> alg(g);

        std::stringstream out;
        std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

        alg.BFS_list(1);

        std::cout.rdbuf(oldCout);

        std::string bfsOutput = out.str();
        assert(bfsOutput.find("A") != std::string::npos);

        int countA = 0;
        size_t pos = 0;
        while ((pos = bfsOutput.find("A", pos)) != std::string::npos) {
            countA++;
            pos += 1;
        }
        assert(countA == 1 && "Should be only one vertex");
        std::cout<<"3/3 passed";
    }


};

#endif //BFSTESTING_H

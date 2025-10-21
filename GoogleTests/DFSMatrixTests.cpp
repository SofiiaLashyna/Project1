#include <gtest/gtest.h>
#include "GraphMatrix.h"
#include "Algorithm.h"
#include <sstream>
#include <string>

TEST(DFSMatrixTests, EmptyGraph) {
    GraphMatrix<std::string> g;
    Algorithms<GraphMatrix<std::string>, std::string> alg(g);

    std::stringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());
    alg.DFS_matrix(1);
    std::cout.rdbuf(oldCout);

    std::string output = out.str();
    EXPECT_TRUE(output.empty()) << "DFS on empty graph should produce no output";
}

TEST(DFSMatrixTests, SingleVertex) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");

    Algorithms<GraphMatrix<std::string>, std::string> alg(g);

    std::stringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());
    alg.DFS_matrix(1);
    std::cout.rdbuf(oldCout);

    std::string dfsOutput = out.str();
    EXPECT_NE(dfsOutput.find("A"), std::string::npos) << "DFS should visit vertex A";

    int countA = 0;
    size_t pos = 0;
    while ((pos = dfsOutput.find("A", pos)) != std::string::npos) {
        countA++;
        pos++;
    }
    EXPECT_EQ(countA, 1) << "DFS should output vertex A only once";
}

TEST(DFSMatrixTests, BasicDFS) {
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

    EXPECT_NE(dfsOutput.find("A"), std::string::npos) << "DFS should visit A";
    EXPECT_NE(dfsOutput.find("B"), std::string::npos) << "DFS should visit B";
    EXPECT_NE(dfsOutput.find("C"), std::string::npos) << "DFS should visit C";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

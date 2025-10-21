#include "gtest/gtest.h"
#include "Algorithm.h"
#include "GraphList.h"
#include <sstream>
#include <string>

TEST(BFSTest, BasicBFS) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 1);

    Algorithms<GraphList<std::string>, std::string> alg(g);

    std::stringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

    alg.BFS_list(1);
    std::cout.rdbuf(oldCout);

    std::string bfsOutput = out.str();
    EXPECT_NE(bfsOutput.find("A"), std::string::npos);
    EXPECT_NE(bfsOutput.find("B"), std::string::npos);
    EXPECT_NE(bfsOutput.find("C"), std::string::npos);
}

TEST(BFSTest, EmptyGraph) {
    GraphList<std::string> g;
    Algorithms<GraphList<std::string>, std::string> alg(g);

    EXPECT_NO_THROW(alg.BFS_list(1));
}

TEST(BFSTest, SingleVertex) {
    GraphList<std::string> g;
    g.addVertex(1, "A");

    Algorithms<GraphList<std::string>, std::string> alg(g);

    std::stringstream out;
    std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

    alg.BFS_list(1);
    std::cout.rdbuf(oldCout);

    std::string bfsOutput = out.str();
    EXPECT_NE(bfsOutput.find("A"), std::string::npos);

    int countA = 0;
    size_t pos = 0;
    while ((pos = bfsOutput.find("A", pos)) != std::string::npos) {
        countA++;
        pos++;
    }
    EXPECT_EQ(countA, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

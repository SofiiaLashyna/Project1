#include "gtest/gtest.h"
#include "GraphMatrix.h"
#include "Algorithm.h"
#include <string>
#include "TestFixtures.h"
TEST(DFSMatrixTests, EmptyGraph) {
    GraphMatrix<std::string> g;
    Algorithms<GraphMatrix<std::string>, std::string> alg(g);

    auto dfsResult = alg.DFS_matrix(1);
    EXPECT_TRUE(dfsResult.empty()) << "DFS on empty graph should produce no output";
}

TEST(DFSMatrixTests, SingleVertex) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");

    Algorithms<GraphMatrix<std::string>, std::string> alg(g);

    auto dfsResult = alg.DFS_matrix(1);

    ASSERT_EQ(dfsResult.size(), 1);
    EXPECT_EQ(dfsResult[0], "A") << "DFS should visit only vertex A";
}
TEST_F(BFSMatrixFixture, BasicDFS_Clean) {
    auto dfsVertices = alg->DFS_matrix(1);
    std::vector<std::string> expected = {"A", "B", "C"};
    EXPECT_EQ(dfsVertices, expected) << "DFS order should be A, B, C";

}

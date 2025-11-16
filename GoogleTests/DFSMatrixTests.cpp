#include "gtest/gtest.h"
#include "GraphMatrix.h"
#include "Algorithm.h"
#include <string>
#include "TestFixtures.h"
TEST(DFSMatrixTests, EmptyGraph) {
    GraphMatrix<std::string> g;
    Algorithms<GraphMatrix<std::string>, std::string> alg(g);

    testing::internal::CaptureStdout();
    alg.DFS_matrix(1);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("A") == std::string::npos);
}

TEST(DFSMatrixTests, SingleVertex) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");

    Algorithms<GraphMatrix<std::string>, std::string> alg(g);

    testing::internal::CaptureStdout();
    alg.DFS_matrix(1);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("A"), std::string::npos);
}

TEST_F(BFSMatrixFixture, BasicDFS_Clean) {
    testing::internal::CaptureStdout();
    alg->DFS_matrix(1);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected = "DFS (matrix) order: A B C \n";

    EXPECT_EQ(output, expected);
}


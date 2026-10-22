#include <DFSMatrix.h>

#include "gtest/gtest.h"
#include "GraphMatrix.h"
#include <string>
#include "TestFixtures.h"
TEST_F(DFSMatrixFixture, EmptyGraph) {
    GraphMatrix<std::string> g;

    testing::internal::CaptureStdout();
    dfsAlg->run(g, 1);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("A") == std::string::npos);
}

TEST_F(DFSMatrixFixture, SingleVertex) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");


    testing::internal::CaptureStdout();
    dfsAlg->run(g, 1);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("A"), std::string::npos);
}

TEST_F(DFSMatrixFixture, BasicDFS_Clean) {
    testing::internal::CaptureStdout();

    dfsAlg->run(g, 1);
    std::string output = testing::internal::GetCapturedStdout();

    std::string expected = "DFS (matrix) order: A B C \n";

    EXPECT_EQ(output, expected);
}


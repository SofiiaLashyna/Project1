#include <RAIIGuard.h>
#include <BFSMatrix.h>
#include "GraphAlgorithms.h"
#include "GraphMatrix.h"
#include <string>
#include "gtest/gtest.h"
#include "TestFixtures.h"

TEST_F(BFSMatrixFixture, BasicBFS) {
    std::stringstream out;
    CoutGuard guard(out);

    bfsAlg->run(g, 1);
    std::string bfsOutput = out.str();

    std::istringstream iss(bfsOutput);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word) words.push_back(word);

    if (words.size() >= 2) {
        std::vector<std::string> bfsVertices(words.begin() + 2, words.end());
        std::vector<std::string> expectedOrder = {"A", "B", "C"};
        EXPECT_EQ(bfsVertices, expectedOrder) << "BFS order should be A, B, C";
    } else {
        FAIL() << "Output is too short to contain BFS data";
    }
}
TEST_F(BFSMatrixFixture, EmptyGraph) {
    GraphMatrix<std::string> g;

    std::stringstream out;
    CoutGuard guard(out);
    bfsAlg->run(g, 1);

    std::string output = out.str();
    EXPECT_TRUE(output.empty()) << "BFS on empty graph should produce no output";
}

TEST_F(BFSMatrixFixture, SingleVertex) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");

    std::stringstream out;
    CoutGuard guard(out);
    bfsAlg->run(g, 1);

    std::string bfsOutput = out.str();
    EXPECT_NE(bfsOutput.find("A"), std::string::npos) << "BFS should visit vertex A";

    int countA = 0;
    size_t pos = 0;
    while ((pos = bfsOutput.find("A", pos)) != std::string::npos) {
        countA++;
        pos++;
    }
    EXPECT_EQ(countA, 1) << "BFS should output vertex A only once";
}
#include <RAIIGuard.h>
#include <gtest/gtest.h>
#include "GraphList.h"
#include "Algorithm.h"
#include <sstream>
#include <string>
#include "TestFixtures.h"
TEST(DFSListTests, EmptyGraph) {
    GraphList<std::string> g;
    Algorithms<GraphList<std::string>, std::string> alg(g);

    std::stringstream out;
    CoutGuard guard(out);
    alg.DFS_list(1);

    std::string output = out.str();
    EXPECT_TRUE(output.empty()) << "DFS on empty graph should produce no output";
}

TEST(DFSListTests, SingleVertex) {
    GraphList<std::string> g;
    g.addVertex(1, "A");

    Algorithms<GraphList<std::string>, std::string> alg(g);

    std::stringstream out;
    CoutGuard guard(out);
    alg.DFS_list(1);

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
TEST_F(BFSListFixture, BasicDFSOrder) {
    std::stringstream out;
    CoutGuard guard(out);
    alg->DFS_list(1);

    std::string dfsOutput = out.str();
    std::istringstream iss(dfsOutput);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) words.push_back(word);

    ASSERT_GE(words.size(), 2) << "Output is too short to contain DFS data";

    std::vector<std::string> dfsVertices(words.begin() + 2, words.end());

    std::vector<std::string> expectedOrder = {"A", "B", "C"};
    EXPECT_EQ(dfsVertices, expectedOrder);
}
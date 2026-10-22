#include <DFSList.h>
#include <RAIIGuard.h>
#include <gtest/gtest.h>
#include "GraphList.h"
#include <sstream>
#include <string>
#include "TestFixtures.h"
TEST_F(DFSListFixture, EmptyGraph) {
    GraphList<std::string> g;

    std::stringstream out;
    CoutGuard guard(out);
    dfsAlg->run(g, 1);

    std::string output = out.str();
    EXPECT_TRUE(output.empty()) << "DFS on empty graph should produce no output";
}

TEST_F(DFSListFixture, SingleVertex) {
    GraphList<std::string> g;
    g.addVertex(1, "A");

    std::stringstream out;
    CoutGuard guard(out);
    dfsAlg->run(g, 1);

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
TEST_F(DFSListFixture, BasicDFSOrder) {
    std::stringstream out;
    CoutGuard guard(out);


    dfsAlg->run(g, 1);

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
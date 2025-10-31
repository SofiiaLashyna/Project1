#include "gtest/gtest.h"
#include "Algorithm.h"
#include "GraphList.h"
#include <sstream>
#include <string>
#include "TestFixtures.h"
#include "RAIIGuard.h"

TEST_F(BFSListFixture, BasicBFS) {
    std::stringstream out;
    CoutGuard guard(out);
    alg->BFS_list(1);

    std::vector<std::string> expectedOrder = { "A", "B", "C"};
    std::stringstream ss(out.str());
    std::string word;
    std::vector<std::string> bfsOrder;
    while (ss >> word) bfsOrder.push_back(word);
    std::vector<std::string> bfsVertices(bfsOrder.begin() + 2, bfsOrder.end());
    EXPECT_EQ(bfsVertices, expectedOrder);
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
    CoutGuard guard(out);
    alg.BFS_list(1);

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

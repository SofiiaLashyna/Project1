#include <BFSList.h>

#include "gtest/gtest.h"
#include "GraphList.h"
#include <sstream>
#include <string>
#include "TestFixtures.h"
#include "RAIIGuard.h"

TEST_F(BFSListFixture, BasicBFS) {
    std::stringstream out;
    CoutGuard guard(out);

    bfsAlg->run(g, 1);

    std::vector<std::string> expectedOrder = { "A", "B", "C"};
    std::stringstream ss(out.str());
    std::string word;
    std::vector<std::string> bfsOrder;
    while (ss >> word) bfsOrder.push_back(word);
    std::vector<std::string> bfsVertices(bfsOrder.begin() + 2, bfsOrder.end());
    EXPECT_EQ(bfsVertices, expectedOrder);
}


TEST_F(BFSListFixture, EmptyGraph) {
    GraphList<std::string> g;

    EXPECT_NO_THROW(bfsAlg->run(g, 1));
}

TEST_F(BFSListFixture, SingleVertex) {
    GraphList<std::string> g;
    g.addVertex(1, "A");

    std::stringstream out;
    CoutGuard guard(out);
    bfsAlg->run(g, 1);

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

#include "TestFixtures.h"
#include "gtest/gtest.h"
#include "GraphList.h"
#include "IsConnectedList.h"

TEST(IsConnectedListTest, EmptyGraph) {
    GraphList<std::string> g;
    IsConnectedList<std::string> alg;

    EXPECT_EQ(alg.run(g), 1) << "Empty graph should be considered connected";
}

TEST(IsConnectedListTest, SingleVertex) {
    GraphList<std::string> g;
    g.addVertex(1, "A");

    IsConnectedList<std::string> alg;
    EXPECT_EQ(alg.run(g), 1) << "Graph with one vertex should be connected";
}

TEST_F(GraphListFixture, ConnectedGraph) {
    IsConnectedList<std::string> alg;
    EXPECT_EQ(alg.run(g), 1) << "Graph with a full path between all vertices should be connected";
}


TEST(IsConnectedListTest, DisconnectedGraph) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2);

    IsConnectedList<std::string> alg;
    EXPECT_EQ(alg.run(g), 0) << "Graph with isolated vertices should not be connected";
}

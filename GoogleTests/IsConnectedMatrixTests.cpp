#include "TestFixtures.h"
#include "gtest/gtest.h"
#include "GraphMatrix.h"
#include "IsConnectedMatrix.h"

TEST(IsConnectedMatrixTest, EmptyGraph) {
    GraphMatrix<std::string> g;
    IsConnectedMatrix<std::string> alg;

    EXPECT_EQ(alg.run(g), 1) << "Empty graph should be considered connected";
}

TEST(IsConnectedMatrixTest, SingleVertex) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");

    IsConnectedMatrix<std::string> alg;
    EXPECT_EQ(alg.run(g), 1) << "Graph with a single vertex should be connected";
}

TEST_F(GraphMatrixFixture, ConnectedGraph) {
    IsConnectedMatrix<std::string> alg;
    EXPECT_EQ(alg.run(g), 1) << "Fully connected graph should be connected";
}

TEST(IsConnectedMatrixTest, DisconnectedGraph) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2);

    IsConnectedMatrix<std::string> alg;
    EXPECT_EQ(alg.run(g), 0) << "Graph with disconnected vertices should not be connected";
}

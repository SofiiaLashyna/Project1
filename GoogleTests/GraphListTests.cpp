#include "gtest/gtest.h"
#include "GraphList.h"
#include <set>
#include "TestFixtures.h"

TEST_F(GraphListFixture, AddVertex) {
    EXPECT_EQ(g.getVertices().size(), 3);
    EXPECT_NE(g.findIndexById(1), NOT_FOUND);
    EXPECT_NE(g.findIndexById(2), NOT_FOUND);
    EXPECT_NE(g.findIndexById(3), NOT_FOUND);

    g.addVertex(4, "D");
    EXPECT_EQ(g.getVertices().size(), 4);
}

TEST_F(GraphListFixture, AddEdge) {
    g.addEdge(2, 3, 5);
    EXPECT_TRUE(g.edgeExists(2, 3));
    EXPECT_TRUE(g.edgeExists(3, 2));
}

TEST_F(GraphListFixture, RemoveEdge) {
    g.removeEdge(1, 2);
    EXPECT_FALSE(g.edgeExists(1, 2));
}

TEST_F(GraphListFixture, RemoveVertex) {
    g.removeVertex(1);
    EXPECT_EQ(g.findIndexById(1), NOT_FOUND);
    EXPECT_FALSE(g.edgeExists(1, 2));
}

TEST_F(GraphListFixture, FindIndexById) {
    EXPECT_EQ(g.findIndexById(1), 0);
    EXPECT_EQ(g.findIndexById(2), 1);
    EXPECT_EQ(g.findIndexById(3), 2);
    EXPECT_EQ(g.findIndexById(4), NOT_FOUND);
}

TEST_F(GraphListFixture, VertexSearch) {
    EXPECT_EQ(g.vertexSearch(1), "A");
    EXPECT_EQ(g.vertexSearch(2), "B");
    EXPECT_EQ(g.vertexSearch(3), "C");
    EXPECT_THROW(g.vertexSearch(4), std::runtime_error);
}


TEST_F(GraphListFixture, EdgeExists) {
    EXPECT_TRUE(g.edgeExists(1, 2));
    EXPECT_TRUE(g.edgeExists(1, 3));
    EXPECT_TRUE(g.edgeExists(2, 1));
    EXPECT_FALSE(g.edgeExists(2, 3));
}

TEST_F(GraphListFixture, GetNeighbors) {
    auto neighborsA = g.getNeighbors(1);
    std::set<std::string> expectedA = {"B", "C"};
    std::set<std::string> actualA(neighborsA.begin(), neighborsA.end());
    EXPECT_EQ(actualA, expectedA);

    auto neighborsC = g.getNeighbors(3);
    std::set<std::string> expectedC = {"A"};
    std::set<std::string> actualC(neighborsC.begin(), neighborsC.end());
    EXPECT_EQ(actualC, expectedC);

    auto neighborsNonExistent = g.getNeighbors(5);
    EXPECT_TRUE(neighborsNonExistent.empty());
}
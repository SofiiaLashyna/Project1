#include "TestFixtures.h"
#include "gtest/gtest.h"
#include "GraphMatrix.h"

TEST_F(GraphMatrixFixture, AddVertex) {
    EXPECT_EQ(g.getVertices().size(), 3);
    EXPECT_NE(g.findIndexById(1), NOT_FOUND);
    EXPECT_NE(g.findIndexById(2), NOT_FOUND);

    g.addVertex(4, "D");
    EXPECT_EQ(g.getVertices().size(), 4);
}

TEST_F(GraphMatrixFixture, AddEdge) {
    g.addEdge(2, 3, 5);
    EXPECT_TRUE(g.edgeExists(2, 3));
    EXPECT_TRUE(g.edgeExists(3, 2));
}

TEST_F(GraphMatrixFixture, RemoveEdge) {
    g.removeEdge(1, 2);
    EXPECT_FALSE(g.edgeExists(1, 2));
}

TEST_F(GraphMatrixFixture, RemoveVertex) {
    g.removeVertex(1);
    EXPECT_EQ(g.findIndexById(1), NOT_FOUND);
    EXPECT_FALSE(g.edgeExists(1, 2));
}

TEST_F(GraphMatrixFixture, VertexSearch) {
    EXPECT_EQ(g.vertexSearch(1), "A");
    EXPECT_EQ(g.vertexSearch(2), "B");
    EXPECT_THROW(g.vertexSearch(4), std::runtime_error);

}

TEST_F(GraphMatrixFixture, GetNeighbors) {
    auto neighborsA = g.getNeighbors(1);
    EXPECT_EQ(neighborsA.size(), 2);
    EXPECT_TRUE((neighborsA[0] == "B" || neighborsA[1] == "B"));
    EXPECT_TRUE((neighborsA[0] == "C" || neighborsA[1] == "C"));
}
TEST_F(GraphMatrixFixture, FindIndexById) {
    EXPECT_EQ(g.findIndexById(1), 0);
    EXPECT_EQ(g.findIndexById(2), 1);
    EXPECT_EQ(g.findIndexById(4), NOT_FOUND);
}

TEST_F(GraphMatrixFixture, EdgeExists) {
    EXPECT_TRUE(g.edgeExists(1, 2));
    EXPECT_TRUE(g.edgeExists(1, 3));
    EXPECT_TRUE(g.edgeExists(2, 1));
    EXPECT_FALSE(g.edgeExists(2, 3));
}

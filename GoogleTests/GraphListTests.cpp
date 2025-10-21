#include <gtest/gtest.h>
#include "GraphList.h"

TEST(GraphListTest, AddVertex) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");

    EXPECT_EQ(g.getVertices().size(), 2);
    EXPECT_NE(g.findIndexById(1), -1);
    EXPECT_NE(g.findIndexById(2), -1);

    g.addVertex(1, "A");
    EXPECT_EQ(g.getVertices().size(), 2);
}

TEST(GraphListTest, AddEdge) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addEdge(1, 2, 5);

    EXPECT_TRUE(g.edgeExists(1, 2));
    EXPECT_TRUE(g.edgeExists(2, 1));
}

TEST(GraphListTest, RemoveEdge) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addEdge(1, 2);

    g.removeEdge(1, 2);
    EXPECT_FALSE(g.edgeExists(1, 2));
}

TEST(GraphListTest, RemoveVertex) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addEdge(1, 2);

    g.removeVertex(1);
    EXPECT_EQ(g.findIndexById(1), -1);
    EXPECT_FALSE(g.edgeExists(1, 2));
}

TEST(GraphListTest, FindIndexById) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");

    EXPECT_EQ(g.findIndexById(1), 0);
    EXPECT_EQ(g.findIndexById(2), 1);
    EXPECT_EQ(g.findIndexById(3), -1);
}

TEST(GraphListTest, VertexSearch) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");

    EXPECT_EQ(g.vertexSearch(1), "A");
    EXPECT_EQ(g.vertexSearch(2), "B");
    EXPECT_EQ(g.vertexSearch(3), "none");
}

TEST(GraphListTest, EdgeExists) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");

    g.addEdge(1, 2);
    Edge<std::string> inactiveEdge;
    inactiveEdge.connect(0, 2);

    EXPECT_TRUE(g.edgeExists(1, 2));
    EXPECT_FALSE(g.edgeExists(1, 3));
    EXPECT_TRUE(g.edgeExists(2, 1));
}

TEST(GraphListTest, GetNeighbors) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");

    g.addEdge(1, 2);
    g.addEdge(1, 3);

    auto neighborsA = g.getNeighbors(1);
    EXPECT_EQ(neighborsA.size(), 2);
    EXPECT_TRUE((neighborsA[0] == "B" || neighborsA[1] == "B"));
    EXPECT_TRUE((neighborsA[0] == "C" || neighborsA[1] == "C"));

    auto neighborsC = g.getNeighbors(3);
    EXPECT_EQ(neighborsC.size(), 1);
    EXPECT_EQ(neighborsC[0], "A");

    auto neighborsNonExistent = g.getNeighbors(5);
    EXPECT_TRUE(neighborsNonExistent.empty());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

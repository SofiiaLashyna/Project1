#include <gtest/gtest.h>
#include "TestFixtures.h"

TEST_F(DijkstraMatrixFixture, BasicPath) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2, 4);
    g.addEdge(2, 3, 5);
    g.addEdge(1, 3, 10);

    int dist = djkAlg->run(g, 1, 3);

    EXPECT_EQ(dist, 9) << "Shortest path 1→2→3 should have total weight 9";
}

TEST_F(DijkstraMatrixFixture, NoPath) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2, 3);

    int dist = djkAlg->run(g, 1, 3);
    constexpr int NO_PATH = -1;
    EXPECT_EQ(dist, NO_PATH) << "If no path exists, Dijkstra_matrix() should return -1";
}

TEST_F(DijkstraMatrixFixture, MultiplePaths) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addVertex(4, "D");

    g.addEdge(1, 2, 1);
    g.addEdge(2, 4, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(3, 4, 1);

    int dist = djkAlg->run(g, 1, 4);

    EXPECT_EQ(dist, 2) << "Shortest path 1→2→4 should have total weight 2";
}

TEST_F(DijkstraMatrixFixture, InvalidNodes) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");


    int dist1 = djkAlg->run(g, 0, 2);
    int dist2 = djkAlg->run(g, 1, 3);

    constexpr int NO_PATH = -1;
    EXPECT_EQ(dist1, NO_PATH) << "Invalid start node should return -1";
    EXPECT_EQ(dist2, NO_PATH) << "Invalid end node should return -1";
}

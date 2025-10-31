#include <gtest/gtest.h>
#include "GraphMatrix.h"
#include "Algorithm.h"

TEST(DijkstraMatrixTest, BasicPath) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2, 4);
    g.addEdge(2, 3, 5);
    g.addEdge(1, 3, 10);

    Algorithms<GraphMatrix<std::string>, std::string> alg(g);
    int dist = alg.Dijkstra_matrix(1, 3);

    EXPECT_EQ(dist, 9) << "Shortest path 1→2→3 should have total weight 9";
}

TEST(DijkstraMatrixTest, NoPath) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2, 3);

    Algorithms<GraphMatrix<std::string>, std::string> alg(g);
    int dist = alg.Dijkstra_matrix(1, 3);
    constexpr int NO_PATH = -1;
    EXPECT_EQ(dist, NO_PATH) << "If no path exists, Dijkstra_matrix() should return -1";
}

TEST(DijkstraMatrixTest, MultiplePaths) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addVertex(4, "D");

    g.addEdge(1, 2, 1);
    g.addEdge(2, 4, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(3, 4, 1);

    Algorithms<GraphMatrix<std::string>, std::string> alg(g);
    int dist = alg.Dijkstra_matrix(1, 4);

    EXPECT_EQ(dist, 2) << "Shortest path 1→2→4 should have total weight 2";
}

TEST(DijkstraMatrixTest, InvalidNodes) {
    GraphMatrix<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");

    Algorithms<GraphMatrix<std::string>, std::string> alg(g);
    int dist1 = alg.Dijkstra_matrix(0, 2);
    int dist2 = alg.Dijkstra_matrix(1, 3);

    constexpr int NO_PATH = -1;
    EXPECT_EQ(dist1, NO_PATH) << "Invalid start node should return -1";
    EXPECT_EQ(dist2, NO_PATH) << "Invalid end node should return -1";
}

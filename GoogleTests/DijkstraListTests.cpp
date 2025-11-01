#include <gtest/gtest.h>
#include "GraphList.h"
#include "Algorithm.h"

TEST(DijkstraListTest, BasicPath) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2, 4);
    g.addEdge(2, 3, 5);
    g.addEdge(1, 3, 10);

    Algorithms<GraphList<std::string>, std::string> alg(g);
    int dist = alg.Dijkstra_list(1, 3);

    EXPECT_EQ(dist, 9) << "Shortest path 1→2→3 should have total weight 9";
}

TEST(DijkstraListTest, NoPath) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addEdge(1, 2, 3);

    Algorithms<GraphList<std::string>, std::string> alg(g);
    int dist = alg.Dijkstra_list(1, 3);
    constexpr int NO_PATH = -1;
    EXPECT_EQ(dist, NO_PATH) << "If no path exists, Dijkstra should return -1";
}

TEST(DijkstraListTest, MultiplePaths) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");
    g.addVertex(3, "C");
    g.addVertex(4, "D");

    g.addEdge(1, 2, 1);
    g.addEdge(2, 4, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(3, 4, 1);

    Algorithms<GraphList<std::string>, std::string> alg(g);
    int dist = alg.Dijkstra_list(1, 4);

    EXPECT_EQ(dist, 2) << "Shortest path 1→2→4 should have total weight 2";
}

TEST(DijkstraListTest, InvalidNodes) {
    GraphList<std::string> g;
    g.addVertex(1, "A");
    g.addVertex(2, "B");

    Algorithms<GraphList<std::string>, std::string> alg(g);
    int dist1 = alg.Dijkstra_list(0, 2);
    int dist2 = alg.Dijkstra_list(1, 3);
    constexpr int NO_PATH = -1;
    EXPECT_EQ(dist1, NO_PATH) << "Invalid start node should return -1";
    EXPECT_EQ(dist2, NO_PATH) << "Invalid end node should return -1";
}

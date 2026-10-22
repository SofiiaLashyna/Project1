#ifndef DJIKSTRA_H
#define DJIKSTRA_H

#include "GraphAlgorithms.h"
#include "GraphList.h"
#include "Queue.h"
#include <vector>
#include <climits>
/**
 * @file DijkstraList.h
 * @brief Defines the DijkstraAlgorithm class, which implements
 * the Strategy pattern for a Dijkstra's algorithm.
 */

/**
 * @class DijkstraListAlgorithm
 * @brief Strategy class that runs Dijkstra's shortest-path algorithm
 *        on a graph represented by an adjacency list.
 *
 * @tparam T The data stored in graph vertices.
 *
 * The algorithm computes the minimum path cost from `startId`
 * to `endId` using a priority queue. Works only with non-negative weights.
 *
 * The result (shortest distance) is returned and also printed to std::cout.
 */
template<typename T>
class DijkstraListAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    /**
           * @brief Performs Dijkstra's algorithm for a graph
           * represented by an adjacency list.
           *
           * The traversal order is printed to std::cout.
           *
           * @param g The GraphList object to traverse.
           * @param startId The ID of the starting vertex.
           * @param endId The ID of the destination vertex.
           * @return The length of the shortest path, or -1 if no path is found.
           * @note Works only with non-negative weights.
           */
    int run(GraphList<T>& g, int startId, int endId) override {
        int start = g.findIndexById(startId);
        int end = g.findIndexById(endId);
        if (start == -1 || end == -1) return -1;

        int V = g.getVertices().size();
        std::vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        Queue<int> q;
        q.push(start, 0);
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            for (auto &[v, w]: g.adjacencyList[u]) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    q.push(v, dist[v]);
                }
            }
        }

        int res = (dist[end] == INT_MAX) ? -1 : dist[end];
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    /// @brief Default destructor.
    ~DijkstraListAlgorithm()= default;
};

#endif //DJIKSTRA_H
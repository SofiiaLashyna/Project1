#ifndef DIJKSTRA_MATRIX_H
#define DIJKSTRA_MATRIX_H

#include "GraphAlgorithms.h"
#include "GraphMatrix.h"
#include "Queue.h"
#include <vector>
#include <climits>
/**
 * @file DijkstraMatrix.h
 * @brief Defines the DijkstraMatrixAlgorithm class, which implements
 * the Strategy pattern for a Dijkstra's algorithm.
 */

/**
 * @class DijkstraMatrixAlgorithm
 * @brief Strategy class implementing Dijkstra's algorithm for
 *        graphs represented by an adjacency matrix.
 *
 * @tparam T The data stored in graph vertices.
 *
 * Computes the minimum path cost from `startId` to `endId`.
 * Returns -1 if no path exists. Assumes non-negative weights.
 */
template< typename T>
class DijkstraMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    /**
                * @brief Performs Dijkstra's algorithm for a graph
                * represented by an adjacency matrix.
                *
                * The traversal order is printed to std::cout.
                *
                * @param g The GraphMatrix object to traverse.
                * @param startId The ID of the starting vertex.
                * @param endId The ID of the destination vertex.
                * @return The length of the shortest path, or -1 if no path is found.
                */
    int run(GraphMatrix<T>& g, int startId, int endId) override {
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
            int n = g.adjacencyMatrix.size();
            for (int v = 0; v < n; ++v) {
                int w = g.adjacencyMatrix[u][v];
                if (w > 0 && dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    q.push(v, dist[v]);
                }
            }
        }

        int res = dist[end] == INT_MAX ? -1 : dist[end];
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }

    /// @brief Default destructor.
    ~DijkstraMatrixAlgorithm()= default;
};

#endif //DIJKSTRA_MATRIX_H
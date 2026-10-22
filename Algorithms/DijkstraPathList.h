#ifndef DIJKSTRAPATHLIST_H
#define DIJKSTRAPATHLIST_H

#include "GraphList.h"
#include "Queue.h"
#include <vector>
#include <climits>
#include <algorithm>

/**
 * @file DijkstraPathList.h
 * @brief Defines the DijkstraPathList class, which implements
 * Dijkstra's algorithm to find the actual path between vertices.
 */

/**
 * @class DijkstraPathList
 * @brief A class designed to compute and return the shortest path
 * between two vertices in a graph using an adjacency list.
 *
 * @tparam T The data type stored in the graph vertices.
 *
 * Unlike standard implementations that only return the minimum distance,
 * this class reconstructs the full sequence of vertex indices from the
 * source to the destination.
 */
template<typename T>
class DijkstraPathList {
public:
    /**
     * @brief Finds the shortest path between two vertices.
     *
     * This method uses a priority queue and a parent tracking array
     * to determine the optimal route.
     *
     * @param g The GraphList object to perform the search on.
     * @param startId The ID of the starting vertex.
     * @param endId The ID of the target vertex.
     * @return std::vector<int> A vector containing the indices of the vertices
     * forming the shortest path. Returns an empty vector if no path exists
     * or if IDs are invalid.
     * * @note The algorithm assumes non-negative edge weights.
     */
    std::vector<int> findShortestPath(GraphList<T>& g, int startId, int endId) {
        int start = g.findIndexById(startId);
        int end = g.findIndexById(endId);
        if (start == -1 || end == -1) return {};

        int V = g.getVertices().size();
        std::vector<int> dist(V, INT_MAX);
        std::vector<int> parent(V, -1);
        dist[start] = 0;

        Queue<int> q;
        q.push(start, 0);

        while (!q.empty()) {
            auto [u, d] = q.popMin();

            if (d > dist[u]) continue;
            if (u == end) break;

            for (auto &[v, w]: g.adjacencyList[u]) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    q.push(v, dist[v]);
                }
            }
        }

        if (dist[end] == INT_MAX) return {};

        std::vector<int> path;
        for (int cur = end; cur != -1; cur = parent[cur]) {
            path.push_back(cur);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    /// @brief Default destructor.
    ~DijkstraPathList() = default;
};

#endif //DIJKSTRAPATHLIST_H
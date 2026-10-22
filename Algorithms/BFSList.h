#ifndef BFS_H
#define BFS_H

#include "GraphAlgorithms.h"
#include "GraphList.h"
#include "Queue.h"
#include <vector>
/**
 * @file BFS.h
 * @brief Defines the BFSAlgorithm class implementing the Strategy
 *        pattern for Breadth-First Search on a GraphList.
 */
/**
 * @class BFSListAlgorithm
 * @brief A strategy class that executes BFS on a GraphList.
 *
 * @tparam T The data type stored in graph vertices.
 */
template<typename T>
class BFSListAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    /**
     * @brief Performs a Breadth-First Search (BFS) on a graph
     *        represented by an adjacency list.
     * It prints the traversal order to std::cout and does not modify the graph.
     *
     * @param g The GraphList object to traverse.
     * @param startId ID of the starting vertex.
     * @param endId Unused parameter, present only to match the base signature.
     *
     * @return Always returns 0.
     */
    int run(GraphList<T>& g, int startId, int endId = -1) override {
        int start = g.findIndexById(startId);
        if (start == -1) return 0;

        std::vector<bool> visited(g.getVertices().size(), false);
        Queue<int> q;
        q.push(start, 0);
        visited[start] = true;

        std::cout << "BFS order: ";
        while (!q.empty()) {
            auto [u, d] = q.popMin();

            this->printVertexData(g.getVertices()[u].getData());

            for (auto &[v, w]: g.adjacencyList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v, 0);
                }
            }
        }
        std::cout << std::endl;
        return 0;
    }
    /// @brief Default destructor.
    ~BFSListAlgorithm() = default;
};
#endif //BFS_H

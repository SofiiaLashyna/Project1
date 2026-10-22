#ifndef BFS_MATRIX_H
#define BFS_MATRIX_H

#include "GraphAlgorithms.h"
#include "GraphMatrix.h"
#include "Queue.h"
#include <vector>
/**
 * @file BFSMatrix.h
 * @brief Defines the BFSMatrixAlgorithm class implementing the Strategy
 *        pattern for BFS on a GraphMatrix.
 */

/**
 * @class BFSMatrixAlgorithm
 * @brief Strategy class executing BFS on a GraphMatrix.
 *
 * @tparam T The data type in graph vertices.
 */
template<typename T>
class BFSMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    /**
       * @brief Performs BFS on a graph represented by an adjacency matrix.
       *
       * The traversal order is printed to std::cout. The graph is not modified.
       *
       * @param g The GraphMatrix object to traverse.
       * @param startId ID of the starting vertex.
       * @param endId Ignored, kept for interface compatibility.
       *
       * @return Always returns 0.
       */
    int run(GraphMatrix<T>& g, int startId, int endId = -1) override {
        int start = g.findIndexById(startId);
        if (start == -1 || g.adjacencyMatrix.empty()) return 0;

        int n = g.adjacencyMatrix.size();
        std::vector<bool> visited(n, false);
        Queue<int> q;
        q.push(start, 0);
        visited[start] = true;

        std::cout << "BFS order: ";
        while (!q.empty()) {
            auto [u, _] = q.popMin();

            this->printVertexData(g.getVertices()[u].getData());

            for (int v = 0; v < n; ++v) {
                if (g.adjacencyMatrix[u][v] && !visited[v]) {
                    visited[v] = true;
                    q.push(v, 0);
                }
            }
        }
        std::cout << std::endl;
        return 0;
    }
    /// @brief Default destructor.
    ~BFSMatrixAlgorithm() = default;
};

#endif //BFS_MATRIX_H
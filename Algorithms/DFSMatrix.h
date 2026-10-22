#ifndef DFS_MATRIX_H
#define DFS_MATRIX_H

#include "GraphAlgorithms.h"
#include "GraphMatrix.h"
#include <vector>
/**
 * @file DFSMatrix.h
 * @brief Defines the DFSMatrixAlgorithm class implementing the Strategy
 *        pattern for DFS on a GraphMatrix.
 */

/**
 * @class DFSMatrixAlgorithm
 * @brief Strategy class executing DFS on a GraphMatrix.
 *
 * @tparam T Stored data type.
 */
template<typename T>
class DFSMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    /**
       * @brief Performs iterative DFS on an adjacency-matrix graph.
       *
       * Prints the visitation order to std::cout. The graph is not modified.
       *
       * @param g GraphMatrix object.
       * @param startId Starting vertex ID.
       * @param endId Ignored.
       *
       * @return Always returns 0.
       */
    int run(GraphMatrix<T>& g, int startId, int endId = -1) override {
        int start = g.findIndexById(startId);
        if (start == -1 || g.adjacencyMatrix.empty()) return 0;

        int n = g.adjacencyMatrix.size();
        std::vector<bool> visited(n, false);
        std::vector<int> stack;
        stack.push_back(start);

        std::cout << "DFS (matrix) order: ";
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();

            if (visited[v]) continue;
            visited[v] = true;

            this->printVertexData(g.getVertices()[v].getData());

            for (int u = n - 1; u >= 0; --u) {
                if (g.adjacencyMatrix[v][u] && !visited[u]) {
                    stack.push_back(u);
                }
            }
        }
        std::cout << std::endl;
        return 0;
    }

    /// @brief Default destructor.
    ~DFSMatrixAlgorithm() = default;
};

#endif //DFS_MATRIX_H
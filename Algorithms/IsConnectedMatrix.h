#ifndef ISCONNECTED_MATRIX_H
#define ISCONNECTED_MATRIX_H
#include "GraphAlgorithms.h"
#include "GraphMatrix.h"
/**
 * @file isConnectedMatrix.h
 * @brief Defines the IsConnectedMatrix class, which implements
 * the check for connectivity.
 */

/**
 * @class IsConnectedMatrix
 * @brief A strategy class that executes a check for connectivity.
 * @tparam T The data type stored in the graph vertices.
 */
template<typename T>
class IsConnectedMatrix : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    /**
         * @brief Performs a connection check algorithm for a graph
         * represented by an adjacency matrix.
         *
         * @param g The GraphMatrix object to traverse.
         * @param startId The ID of the starting vertex.
         * @param endId (Unused) Included to match the base class signature.
         * @return Always returns 1 if true, else 0.
         */
    int run(GraphMatrix<T> &g, int startId = 0, int endId = -1) override {
        int n = g.adjacencyMatrix.size();
        if (n == 0) return 1;

        std::vector<bool> visited(n, false);
        std::vector<int> stack;
        stack.push_back(0);

        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            if (visited[v]) continue;
            visited[v] = true;

            for (int u = 0; u < n; ++u)
                if (g.adjacencyMatrix[v][u] && !visited[u])
                    stack.push_back(u);
        }

        for (int i = 0; i < n; ++i) {
            if (!visited[i])
                return 0;
        }

        return 1;
    }
    /// @brief Default destructor.
    virtual ~IsConnectedMatrix() = default;
};

#endif //ISCONNECTED_MATRIX_H

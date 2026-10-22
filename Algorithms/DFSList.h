#ifndef DFS_H
#define DFS_H

#include "GraphAlgorithms.h"
#include "GraphList.h"
#include <vector>
/**
 * @file DFS.h
 * @brief Defines the DFSAlgorithm class implementing the Strategy
 *        pattern for Depth-First Search on a GraphList.
 */

/**
 * @class DFSListAlgorithm
 * @brief A strategy class that executes DFS on a GraphList.
 *
 * @tparam T The type stored in graph vertices.
 */
template<typename T>
class DFSListAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    /**
     * @brief Performs iterative DFS on a graph represented by an adjacency list.
     *
     * Prints the traversal order to std::cout. The graph remains unchanged.
     *
     * @param g GraphList object to traverse.
     * @param startId Starting vertex ID.
     * @param endId Unused.
     *
     * @return Always returns 0.
     */
    int run(GraphList<T>& g, int startId, int endId = -1) override {
        int start = g.findIndexById(startId);
        if (start == -1) return 0;

        std::vector<bool> visited(g.getVertices().size(), false);
        std::vector<int> stack;
        stack.push_back(start);

        std::cout << "DFS order: ";
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            if (visited[v]) continue;
            visited[v] = true;

            this->printVertexData(g.getVertices()[v].getData());

            for (int i = (int) g.adjacencyList[v].size() - 1; i >= 0; --i) {
                int neighbor = g.adjacencyList[v][i].first;
                if (!visited[neighbor]) stack.push_back(neighbor);
            }
        }
        std::cout << std::endl;
        return 0;
    }

    /// @brief Default destructor.
    ~DFSListAlgorithm() = default;
};

#endif //DFS_H
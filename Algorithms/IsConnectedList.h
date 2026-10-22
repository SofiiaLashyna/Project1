#ifndef ISCONNECTED_LIST_H
#define ISCONNECTED_LIST_H
#include "GraphAlgorithms.h"
#include "GraphList.h"
/**
 * @file isConnectedList.h
 * @brief Defines the IsConnectedList class, which implements
 * the check for connectivity.
 */

/**
 * @class IsConnectedList
 * @brief A strategy class that executes a check for connectivity.
 * @tparam T The data type stored in the graph vertices.
 */
template<typename T>
class IsConnectedList : public GraphAlgorithm<GraphList<T>, T> {
public:
    /**
         * @brief Performs a connection check algorithm for a graph
         * represented by an adjacency list.
         *
         * @param g The GraphList object to traverse.
         * @param startId The ID of the starting vertex.
         * @param endId (Unused) Included to match the base class signature.
         * @return Always returns 1 = connected, 0 = not connected.
         */
    int run(GraphList<T> &g, int startId = 0, int endId = -1) override {
        int n = g.getVertices().size();
        if (n == 0) return 1;
        if (n == 1) return 1;

        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (!g.adjacencyList[i].empty()) {
                start = i;
                break;
            }
        }

        if (start == -1) return 0;

        std::vector<bool> visited(n, false);
        std::vector<int> stack;
        stack.push_back(start);

        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            if (visited[v]) continue;
            visited[v] = true;

            for (auto &[u, w] : g.adjacencyList[v]) {
                if (!visited[u]) stack.push_back(u);
            }
        }

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) return 0;
        }

        return 1;
    }
    /// @brief Default destructor.
    virtual ~IsConnectedList() = default;
};
#endif //ISCONNECTED_LIST_H

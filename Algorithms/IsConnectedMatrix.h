#ifndef ISCONNECTED_MATRIX_H
#define ISCONNECTED_MATRIX_H
#include "Algorithm.h"
#include "GraphMatrix.h"

template<typename T>
class IsConnectedMatrix : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
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
};

#endif //ISCONNECTED_MATRIX_H

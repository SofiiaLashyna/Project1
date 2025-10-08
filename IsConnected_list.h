#ifndef ISCONNECTED_LIST_H
#define ISCONNECTED_LIST_H
#include "Algorithm.h"
#include "Graph_list.h"

template<typename T>
class IsConnectedList : public GraphAlgorithm<Graph_list<T>, T> {
public:
    int run(Graph_list<T> &g, int startId = 0, int endId = -1) override {
        int n = g.getVertices().size();
        if (n == 0) return 1;

        std::vector<bool> visited(n, false);
        std::vector<int> stack;

        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (!g.adjacencyList[i].empty()) {
                start = i;
                break;
            }
        }
        if (start == -1) return 1;

        stack.push_back(start);

        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            if (visited[v]) continue;
            visited[v] = true;

            for (auto &[u, w]: g.adjacencyList[v])
                if (!visited[u]) stack.push_back(u);
        }

        for (int i = 0; i < n; ++i)
            if (!visited[i])
                return 0;

        return 1;
    }
};
#endif //ISCONNECTED_LIST_H

#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "Graph.h"
#include "Queue.h"

template<typename GraphType, typename T>
class Algorithms {
    GraphType &graph;

public:
    Algorithms(GraphType &g) : graph(g) {
    }

    void BFS_list(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1) return;

        std::vector<bool> visited(graph.getVertices().size(), false);
        Queue<int> q;
        q.push(start, 0);
        visited[start] = true;

        std::cout << "BFS order: ";
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            std::cout << graph.getVertices()[u].data << " ";
            for (auto &[v, w]: graph.adjacencyList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v, 0);
                }
            }
        }
        std::cout << std::endl;
    }

    void BFS_matrix(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1 || graph.adjacencyMatrix.empty()) return;

        int n = graph.adjacencyMatrix.size();
        std::vector<bool> visited(n, false);
        Queue<int> q;
        q.push(start, 0);
        visited[start] = true;

        std::cout << "BFS (matrix) order: ";

        while (!q.empty()) {
            auto [u, _] = q.popMin();
            std::cout << graph.getVertices()[u].data << " ";

            for (int v = 0; v < n; ++v) {
                if (graph.adjacencyMatrix[u][v] && !visited[v]) {
                    visited[v] = true;
                    q.push(v, 0);
                }
            }
        }

        std::cout << std::endl;
    }

    void DFS_list(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1) return;

        std::vector<bool> visited(graph.getVertices().size(), false);
        std::vector<int> stack;
        stack.push_back(start);

        std::cout << "DFS (iterative) order: ";
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            if (visited[v]) continue;
            visited[v] = true;
            std::cout << graph.getVertices()[v].data << " ";

            for (int i = (int) graph.adjacencyList[v].size() - 1; i >= 0; --i) {
                int neighbor = graph.adjacencyList[v][i].first;
                if (!visited[neighbor]) stack.push_back(neighbor);
            }
        }
        std::cout << std::endl;
    }

    void DFS_matrix(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1 || graph.adjacencyMatrix.empty()) return;

        int n = graph.adjacencyMatrix.size();
        std::vector<bool> visited(n, false);
        std::vector<int> stack;
        stack.push_back(start);

        std::cout << "DFS (matrix) order: ";

        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();

            if (visited[v]) continue;
            visited[v] = true;

            std::cout << graph.getVertices()[v].data << " ";

            for (int u = n - 1; u >= 0; --u) {
                if (graph.adjacencyMatrix[v][u] && !visited[u]) {
                    stack.push_back(u);
                }
            }
        }

        std::cout << std::endl;
    }


    int Dijkstra_list(int startId, int endId) {
        int start = graph.findIndexById(startId);
        int end = graph.findIndexById(endId);
        if (start == -1 || end == -1) return -1;

        int V = graph.getVertices().size();
        std::vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        Queue<int> q;
        q.push(start, 0);
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            for (auto &[v, w]: graph.adjacencyList[u]) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    q.push(v, dist[v]);
                }
            }
        }
        return dist[end] == INT_MAX ? -1 : dist[end];
    }


    int Dijkstra_matrix(int startId, int endId) {
        int start = graph.findIndexById(startId);
        int end = graph.findIndexById(endId);
        if (start == -1 || end == -1) return -1;

        int V = graph.getVertices().size();
        std::vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        Queue<int> q;
        q.push(start, 0);
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            int n = graph.adjacencyMatrix.size();
            for (int v = 0; v < n; ++v) {
                int w = graph.adjacencyMatrix[u][v];
                if (w > 0 && dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    q.push(v, dist[v]);
                }
            }
        }
        return dist[end] == INT_MAX ? -1 : dist[end];
    }
};

template<typename GraphType, typename T>
class GraphAlgorithm {
public:
    virtual ~GraphAlgorithm() = default;

    virtual int run(GraphType &g, int startId, int endId = -1) = 0;
};


#endif //ALGORITHM_H

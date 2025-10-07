#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "Graph.h"
#include "Queue.h"

template <typename GraphType, typename  T>
class Algorithms {
    GraphType& graph;
public:
    Algorithms(GraphType& g) : graph(g) {}

    void BFS(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1) return;

        std::vector<bool> visited(graph.getVertices().size(), false);
        Queue<T> q; q.push(start, 0);
        visited[start] = true;

        std::cout << "BFS order: ";
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            std::cout << graph.getVertices()[u].data << " ";
            for (auto& [v, w] : graph.adjacencyList[u]) {
                if (!visited[v]) { visited[v] = true; q.push(v, 0); }
            }
        }
        std::cout << std::endl;
    }

    void DFS(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1) return;

        std::vector<bool> visited(graph.getVertices().size(), false);
        std::cout << "DFS order: ";
        dfsRecursive(start, visited);
        std::cout << std::endl;
    }

    void DFS_iterative(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1) return;

        std::vector<bool> visited(graph.getVertices().size(), false);
        std::vector<int> stack; stack.push_back(start);

        std::cout << "DFS (iterative) order: ";
        while (!stack.empty()) {
            int v = stack.back(); stack.pop_back();
            if (visited[v]) continue;
            visited[v] = true;
            std::cout << graph.getVertices()[v].data << " ";

            for (int i = (int)graph.adjacencyList[v].size() - 1; i >= 0; --i) {
                int neighbor = graph.adjacencyList[v][i].first;
                if (!visited[neighbor]) stack.push_back(neighbor);
            }
        }
        std::cout << std::endl;
    }

    int Dijkstra(int startId, int endId) {
        int start = graph.findIndexById(startId);
        int end = graph.findIndexById(endId);
        if (start == -1 || end == -1) return -1;

        int V = graph.getVertices().size();
        std::vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        Queue<int> q; q.push(start, 0);
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            for (auto& [v, w] : graph.adjacencyList[u]) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    q.push(v, dist[v]);
                }
            }
        }
        return dist[end] == INT_MAX ? -1 : dist[end];
    }

    bool isConnectedList() {
        return graph.isConnectedList();
    }

    bool isConnectedMatrix() {
        int n = graph.adjacencyMatrix.size();
        if (n == 0) return true;

        std::vector<bool> visited(n, false);

        int start = -1;
        for (int i = 0; i < n; ++i) {
            int degree = 0;
            for (int j = 0; j < n; ++j) if (graph.adjacencyMatrix[i][j]) degree++;
            if (degree > 0) { start = i; break; }
        }
        if (start == -1) return true;

        dfsMatrix(start, visited);

        for (int i = 0; i < n; ++i) {
            int degree = 0;
            for (int j = 0; j < n; ++j) if (graph.adjacencyMatrix[i][j]) degree++;
            if (degree > 0 && !visited[i]) return false;
        }
        return true;
    }

private:
    void dfsRecursive(int v, std::vector<bool>& visited) {
        visited[v] = true;
        std::cout << graph.getVertices()[v].data << " ";
        for (auto& [neighbor, w] : graph.adjacencyList[v])
            if (!visited[neighbor]) dfsRecursive(neighbor, visited);
    }

    void dfsMatrix(int v, std::vector<bool>& visited) {
        visited[v] = true;
        for (int i = 0; i < graph.adjacencyMatrix[v].size(); ++i)
            if (graph.adjacencyMatrix[v][i] && !visited[i]) dfsMatrix(i, visited);
    }
};

template<typename GraphType, typename T>
class GraphAlgorithm {
public:
    virtual ~GraphAlgorithm() = default;
    virtual int run(GraphType& g, int startId, int endId = -1) = 0;
};





#endif //ALGORITHM_H

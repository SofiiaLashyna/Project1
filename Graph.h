#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
#include "Queue.h"

template <typename T>
class Vertex {
public:
    int id;
    T data;
    bool visited = false;
    void markVisited() {
        visited = true;
    }
    void resetVisited() {
        visited = false;
    }
    void printData() {
        std::cout << id << " " << data;
    }
};

template <typename T>
class Edge {
public:
    int from;
    int to;
    int weight = 0;

    void changeWeight(int w) {
        weight = w;
    }
    void connect(int u, int v) {
        from = u;
        to = v;
    }
    void disconnect() {
        from = -1;
        to = -1;
        weight = 0;
    }
    bool connects(int u, int v) const {
        return (from == u && to == v) || (from == v && to == u);
    }
    bool isActive() const {
        return from != -1 && to != -1;
    }
};
template <typename T>
class Graph {
public:
    std::vector<Vertex<T>> vertices;
    std::vector<Edge<T>> edges;
    std::vector<std::vector<std::pair<int,int>>> adjacencyList;
    void constructAdjacency() {
        adjacencyList.clear();
        adjacencyList.resize(vertices.size());

        for (auto& edge : edges) {
            if(edge.isActive()) {
                adjacencyList[edge.from].push_back({edge.to, edge.weight});
                adjacencyList[edge.to].push_back({edge.from, edge.weight});
            }
            }
    }
    void addVertex(int id, T data) {
        for (auto& v : vertices) {
            if (v.id == id) return;
        }
        Vertex<T> vertex;
        vertex.id = id;
        vertex.data = data;
        vertices.push_back(vertex);
        adjacencyList.resize(vertices.size());
    }
    void addEdge(int f, int t, int weight = 1) {
        if (f >= vertices.size() || t >= vertices.size()) return;
        if (edgeExists(f, t)) return;
        Edge<T> edge;
        edge.connect(f, t);
        edge.weight = weight;
        edges.push_back(edge);

        adjacencyList[f].push_back({t, weight});
        adjacencyList[t].push_back({f, weight});
    }
    void removeVertex(int id) {
        if (id >= vertices.size()) return;

        vertices[id].id = -1;
        vertices[id].data = "";

        for (auto &edge : edges) {
            if (edge.from == id || edge.to == id) {
                edge.disconnect();
            }
        }

        constructAdjacency();
    }
    void removeEdge(int f, int t) {
        for (auto &edge : edges) {
            if ((edge.from == f && edge.to == t) || (edge.from == t && edge.to == f)) {
                edge.disconnect();
            }
        }

        constructAdjacency();
    }
    void BFS(int startId) {
        if (startId >= vertices.size()) return;

        std::vector<bool> visited(vertices.size(), false);
        Queue q;
        q.push(startId, 0);

        visited[startId] = true;
        std::cout << "BFS order: ";

        while (!q.empty()) {
            auto [u, d] = q.popMin();
            std::cout << vertices[u].data << " ";

            for (auto& [v, weight] : adjacencyList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v, 0);
                }
            }
        }
        std::cout << std::endl;
    }
    void DFS(int startId) {
        std::cout << "DFS order: ";
        if (startId >= vertices.size()) return;

        std::vector<bool> visited(vertices.size(), false);
        dfsRecursive(startId, visited);
    }
    int Dijkstra(int startId, int endId) {
        int V = vertices.size();
        std::vector<int> dist(V, INT_MAX);
        dist[startId] = 0;

        Queue q;
        q.push(startId, 0);

        while (!q.empty()) {
            auto [u, d] = q.popMin();
            for (auto& [v, weight] : adjacencyList[u]) {
                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    q.push(v, dist[v]);
                }
            }
        }
        if(dist[endId] == INT_MAX)
            return -1;
        return dist[endId];
    }
    bool vertexExists(int id) {
        for(auto vert : vertices) {
            if(vert.id == id) {
                return true;
            }
        }
        return false;
    }
    bool edgeExists(int f, int t) {
        for (auto& ed : edges) {
            if (ed.connects(f, t) && ed.isActive()) {
                return true;
            }
        }
        return false;
    }
    std::string vertexSearch(int id) {
        for(auto vert : vertices) {
            if(vert.id == id) {
                return vert.data;
            }
        }
        return "none";
    }
    std::vector<std::string> getNeighbors(int id) {
        std::vector<std::string> neighbors;
        for(auto ed : edges) {
            if (!ed.isActive()) continue;
            int idx = -1;
            if(ed.connects(ed.to, id)) {
                idx = ed.to;
            }
            if(ed.connects(ed.from, id)) {
                idx = ed.from;
            }
            if(idx != -1) {
                std::string n = vertexSearch(idx);
                bool flag = true;
                for(auto & neighbor : neighbors){
                    if(neighbor == n) {
                        flag = false;
                        break;
                    }
                }
                if(flag)
                    neighbors.push_back(n);
            }
        }
        return neighbors;
    }
    void clearGraph() {
        vertices.clear();
        edges.clear();
        adjacencyList.clear();
    }
    void print() {
        std::cout << "Graph adjacency list:\n";
        for (size_t i = 0; i < adjacencyList.size(); i++) {
            if(vertices[i].id !=-1) {
                std::cout << "Vertex " << vertices[i].id << " (" << vertices[i].data << "): ";
                for (auto& [neighborIdx, weight] : adjacencyList[i]) {
                    std::cout << vertices[neighborIdx].data << " (w=" << weight << ") ";
                }
                std::cout << std::endl;
            }
        }
    }
private:
    void dfsRecursive(int v, std::vector<bool>& visited) {
        visited[v] = true;
        std::cout << vertices[v].data << " ";

        for (auto& [neighbor, weight] : adjacencyList[v]) {
            if (!visited[neighbor]) {
                dfsRecursive(neighbor, visited);
            }
        }
    }
};
#endif //GRAPH_H

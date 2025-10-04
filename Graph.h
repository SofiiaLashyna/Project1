#ifndef GRAPH_H
#define GRAPH_H
#include <vector>

template <typename T>
class Vertex {
public:
    int id;
    T data;
    bool visited = false;
};

template <typename T>
class Edge {
public:
    int from;
    int to;
    int weight = 0;
    //data
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
            adjacencyList[edge.from].push_back({edge.to, edge.weight});
            adjacencyList[edge.to].push_back({edge.from, edge.weight});
        }
    }
    void print() {
        for (int i=0; i<adjacencyList.size(); i++) {
            std::cout << "Vertex " << i << " (" << vertices[i].data<< ")";
            for (auto& [neighbor, weight] : adjacencyList[i]) {
                std::cout << " -> " << neighbor << " (w=" << weight << ")";
            }
            std::cout << std::endl;
        }
    }

};
#endif //GRAPH_H

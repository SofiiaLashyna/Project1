#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H
#include "Graph.h"
template<typename T>
class Graph_matrix : public Graph<T> {
public:

    Graph_matrix() = default;
    Graph_matrix(const std::vector<Vertex<T>>& verts, const std::vector<Edge<T>>& eds) {
        this->vertices = verts;
        this->edges = eds;
        constructAdjacency();
    }

    std::vector<std::vector<int> > adjacencyMatrix;

    void constructAdjacency() override {
        int n = this->vertices.size();
        adjacencyMatrix.assign(n, std::vector<int>(n, 0));
        for (auto &edge: this->edges) {
            if (edge.isActive()) {
                adjacencyMatrix[edge.from][edge.to] = 1;
                adjacencyMatrix[edge.to][edge.from] = 1;
            }
        }
    }

    void addEdge(int fId, int tId, int weight = 1) override {
        int f = this->findIndexById(fId);
        int t = this->findIndexById(tId);
        if (f == -1 || t == -1) return;
        if (this->edgeExists(fId, tId)) return;

        Edge<T> edge; edge.connect(f, t); edge.weight = weight;
        this->edges.push_back(edge);

        constructAdjacency();
    }

    void addVertex(int id, T data)  override {
        if (this->findIndexById(id) != -1) return;
        Vertex<T> vertex; vertex.id = id; vertex.data = data;
        this->vertices.push_back(vertex);
        constructAdjacency();
    }

    void print() const override{
        int n = adjacencyMatrix.size();
        if (n == 0) {
            std::cout << "Graph is empty.\n";
            return;
        }

        std::cout << "    ";
        for (const auto &v : this->vertices)
            std::cout << v.id << " ";
        std::cout << "\n";

        for (int i = 0; i < n; ++i) {
            std::cout << this->vertices[i].id << " : ";
            for (int j = 0; j < n; ++j) {
                std::cout << adjacencyMatrix[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    void removeVertex(int id) override {
        int idx = this->findIndexById(id);
        if (idx == -1) return;

        this->vertices[idx].id = -1;
        this->vertices[idx].data = "";

        for (auto &edge : this->edges) {
            if (edge.from == idx || edge.to == idx) edge.disconnect();
        }

        constructAdjacency();
    }

    void removeEdge(int fId, int tId) override {
        int f = this->findIndexById(fId);
        int t = this->findIndexById(tId);
        if (f == -1 || t == -1) return;

        for (auto &edge : this->edges) {
            if ((edge.from == f && edge.to == t) || (edge.from == t && edge.to == f))
                edge.disconnect();
        }

        constructAdjacency();
    }

};
#endif //GRAPH_MATRIX_H

#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H
#include "Graph.h"

template<typename T>
class GraphList : public Graph<T> {
public:
    std::vector<std::vector<std::pair<int, int> > > adjacencyList;

    void constructAdjacency() override {
        adjacencyList.clear();
        adjacencyList.resize(this->vertices.size());

        for (auto &edge: this->edges) {
            if (edge.isActive()) {
                adjacencyList[edge.from].push_back({edge.to, edge.weight});
                adjacencyList[edge.to].push_back({edge.from, edge.weight});
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

    void addVertex(int id, T data) override {
        if (this->findIndexById(id) != -1) return;
        Vertex<T> vertex; vertex.setId(id); vertex.setData(data);
        this->vertices.push_back(vertex);
        adjacencyList.resize(this->vertices.size());
     }

    void removeVertex(int id) override {
        int idx = this->findIndexById(id);
        if (idx == -1) return;

        this->vertices[idx].setId(-1);
        this->vertices[idx].setData(T{});

        for (auto &edge : this->edges) {
            if (edge.getFrom() == idx || edge.getTo() == idx) edge.disconnect();
        }

        constructAdjacency();
    }

    void removeEdge(int fId, int tId) override {
        int f = this->findIndexById(fId);
        int t = this->findIndexById(tId);
        if (f == -1 || t == -1) return;

        for (auto &edge : this->edges) {
            if ((edge.getFrom() == f && edge.getTo() == t) || (edge.getFrom() == t && edge.getTo() == f))
                edge.disconnect();
        }

        constructAdjacency();
    }

    void print() const override{
            std::cout << "Graph adjacency list:\n";
            for (size_t i = 0; i < adjacencyList.size(); i++) {
                if(this->vertices[i].getId() !=-1) {
                    std::cout << "Vertex " << this->vertices[i].getId() << " (" << this->vertices[i].getData() << "): ";
                    for (auto& [neighborIdx, weight] : adjacencyList[i]) {
                        std::cout << this->vertices[neighborIdx].getData() << " (w=" << weight << ") ";
                    }
                    std::cout << std::endl;
                }
            }
        }



};
#endif //GRAPH_LIST_H
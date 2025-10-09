#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
#include "Edge.h"
#include "Vertex.h"

template<typename T>
class Graph {
protected:
    std::vector<Vertex<T> > vertices;
    std::vector<Edge<T> > edges;
public:
    virtual ~Graph() = default;

    const std::vector<Vertex<T>>& getVertices() const { return vertices; }
    const std::vector<Edge<T>>& getEdges() const { return edges; }


    int findIndexById(int id) const{
        for (int i = 0; i < vertices.size(); ++i)
            if (vertices[i].id == id) return i;
        return -1;
    }

    virtual void constructAdjacency() = 0;

    virtual void addVertex(int id, T data) = 0;

    virtual void addEdge(int fId, int tId, int weight = 1) = 0;

    virtual void removeVertex(int id) = 0;

    virtual void removeEdge(int fId, int tId) = 0;

    virtual void print() const = 0;

    std::string vertexSearch(int id) {
            for(auto vert : vertices) {
                if(vert.id == id) {
                    return vert.data;
                }
            }
            return "none";
        }


    bool edgeExists(int fId, int tId) const {
        int f = findIndexById(fId);
        int t = findIndexById(tId);
        if (f == -1 || t == -1) return false;

        for (auto &ed: edges)
            if (ed.connects(f, t) && ed.isActive()) return true;
        return false;
    }

    std::vector<std::string> getNeighbors(int id) {
        std::vector<std::string> neighbors;
        int idx = findIndexById(id);
        if (idx == -1) return neighbors;

        for (auto& ed : edges) {
            if (!ed.isActive()) continue;
            if (ed.from == idx) neighbors.push_back(vertexSearch(this->vertices[ed.to].id));
            else if (ed.to == idx) neighbors.push_back(vertexSearch(this->vertices[ed.from].id));
        }
        return neighbors;
    }


};

#endif //GRAPH
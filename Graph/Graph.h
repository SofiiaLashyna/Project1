#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <iostream>
#include "Edge.h"
#include "Vertex.h"
/**
 * @file Graph.h
 * @brief Defines the abstract base class for Graph implementations (List/Matrix).
 */

/**
 * @class Graph
 * @brief An abstract base class defining the interface for a graph structure.
 *
 * This class provides the core storage (vertices, edges) and a common
 * interface that concrete classes (GraphList, GraphMatrix) must implement.
 * @tparam T The type of data stored in the vertices.
 */
template<typename T>
class Graph {
protected:
    std::vector<Vertex<T> > vertices; ///< The list of all vertices in the graph.
    std::vector<Edge<T> > edges; ///< The list of all edges in the graph.
public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~Graph() = default;

    std::vector<Vertex<T>>& getVertices() { return vertices; }
    std::vector<Edge<T>>& getEdges() { return edges; }


    int findIndexById(int id) const{
        for (int i = 0; i < vertices.size(); ++i)
            if (vertices[i].getId() == id) return i;
        return -1;
    }
    /**
             * @brief Pure virtual method to construct the adjacency structure (List or Matrix).
             */
    virtual void constructAdjacency() = 0;
    /**
             * @brief Pure virtual method to add a new vertex.
             * @param id The public ID for the new vertex.
             * @param data The data (of type T) to store in the vertex.
             */
    virtual void addVertex(int id, T data) = 0;
    /**
             * @brief Pure virtual method to add a new edge.
             * @param fId The public ID of the 'from' vertex.
             * @param tId The public ID of the 'to' vertex.
             * @param weight The weight of the edge (default is 1).
             */
    virtual void addEdge(int fId, int tId, int weight = 1) = 0;
    /**
             * @brief Pure virtual method to remove a vertex.
             * @param id The public ID of the vertex to remove.
             * IDs are external identifiers, not internal array indices.
             */
    virtual void removeVertex(int id) = 0;
    /**
             * @brief Pure virtual method to remove an edge.
             * @param fId The public ID of the 'from' vertex.
             * @param tId The public ID of the 'to' vertex.
             * IDs are external identifiers, not internal array indices.
             */
    virtual void removeEdge(int fId, int tId) = 0;
    /**
             * @brief Pure virtual method to print the graph structure (e.g., to console).
             */
    virtual void print() const = 0;
    /**
         * @brief Searches for a vertex by its public ID.
         *
         * @param id The public ID of the vertex to search for.
         * @return The data of type T stored in the vertex.
         * @throws std::runtime_error If the vertex with the given ID is not found.
         */
    T vertexSearch(int id) {
        for (auto &vert: vertices) {
            if (vert.getId() == id) {
                return vert.getData();
            }
        }
        throw std::runtime_error("Vertex not found");
    }

    /**
             * @brief Checks if an active edge exists between two vertex IDs.
             * @param fId The public ID of the first vertex.
             * @param tId The public ID of the second vertex.
             * @return True if an active edge exists, false otherwise.
             */
    bool edgeExists(int fId, int tId) const {
        int f = findIndexById(fId);
        int t = findIndexById(tId);
        if (f == -1 || t == -1) return false;

        for (auto &ed: edges)
            if (ed.connects(f, t) && ed.isActive()) return true;
        return false;
    }
    /**
         * @brief Returns a list of neighbor data for a given vertex ID.
         *
         * This method finds all vertices connected to the vertex with the given ID
         * via active edges and returns their data as a vector of type T.
         *
         * @param id The public ID of the vertex whose neighbors are to be found.
         * @return std::vector<T> A vector containing the data of all neighboring vertices.
         */
    std::vector<T> getNeighbors(int id) {
        std::vector<T> neighbors;
        int idx = findIndexById(id);
        if (idx == -1) return neighbors;

        for (auto& ed : edges) {
            if (!ed.isActive()) continue;
            if (ed.from == idx) neighbors.push_back(vertexSearch(this->vertices[ed.to].getId()));
            else if (ed.to == idx) neighbors.push_back(vertexSearch(this->vertices[ed.from].getId()));
        }
        return neighbors;
    }



};

#endif //GRAPH
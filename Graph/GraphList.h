#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H
#include "Graph.h"
/**
 * @file GraphList.h
 * @brief Defines the GraphList class, a concrete Graph implementation
 * using an adjacency list.
 */

/**
 * @class GraphList
 * @brief A concrete implementation of the Graph interface using an adjacency list.
 * @tparam T The type of data stored in the vertices.
 * @note For undirected graphs, each edge is stored twice: once for each direction.
 *       That is, if there is an edge between vertex A and B, both A's and B's adjacency lists
 *       will contain the other vertex.
 * @see Graph
 */
template<typename T>
class GraphList : public Graph<T> {
public:
    ///< Each entry adjacencyList[i] stores pairs (neighborIndex, weight).
    std::vector<std::vector<std::pair<int, int> > > adjacencyList;
    /**
         * @brief Constructs the adjacency list from the current set of vertices and edges.
         *
         * @details This method clears the existing adjacency lists and rebuilds them
         *          according to the active edges in the graph.
         *          For undirected graphs, edges are added in both directions.
         * @note Clears the previous adjacency list before rebuilding.

         */
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
    /**
             * @brief Adds an edge and rebuilds the adjacency list.
             * @param fId The public ID of the 'from' vertex.
             * @param tId The public ID of the 'to' vertex.
             * @param weight The weight of the edge (default is 1).
             */
    void addEdge(int fId, int tId, int weight = 1) override {
        int f = this->findIndexById(fId);
        int t = this->findIndexById(tId);
        if (f == -1 || t == -1) return;
        if (this->edgeExists(fId, tId)) return;

        Edge<T> edge;
        edge.connect(f, t);
        edge.weight = weight;
        this->edges.push_back(edge);

        constructAdjacency();
    }
    /**
             * @brief Adds a vertex and resizes the adjacency list.
             * @param id The public ID for the new vertex.
             * @param data The data (of type T) to store in the vertex.
             */
    void addVertex(int id, T data) override {
        if (this->findIndexById(id) != -1) return;
        Vertex<T> vertex;
        vertex.setId(id);
        vertex.setData(data);
        this->vertices.push_back(vertex);
        adjacencyList.resize(this->vertices.size());
     }
    /**
             * @brief Removes a vertex (soft delete) and rebuilds the adjacency list.
             * @param id The public ID of the vertex to remove.
             */
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
    /**
             * @brief Removes an edge (soft delete) and rebuilds the adjacency list.
             * @param fId The public ID of the 'from' vertex.
             * @param tId The public ID of the 'to' vertex.
             */
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
    /**
             * @brief Prints a user-friendly representation of the adjacency list to the console.
             */
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
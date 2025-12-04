#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H
#include "Graph.h"
/**
 * @file GraphMatrix.h
 * @brief Defines the GraphMatrix class, a concrete Graph implementation
 * using an adjacency matrix.
 */

/**
 * @class GraphMatrix
 * @brief A concrete implementation of the Graph interface using an adjacency matrix.
 * @tparam T The type of data stored in the vertices.
 */
template<typename T>
class GraphMatrix : public Graph<T> {
public:
    /**
            * @brief Default constructor.
            */
    GraphMatrix() = default;
    /**
     * @brief Constructor to initialize the graph from existing vertex/edge lists.
     * @param verts Vector of vertices to add.
     * @param eds Vector of edges to add.
     * @see Graph
     */
    GraphMatrix(const std::vector<Vertex<T>>& verts, const std::vector<Edge<T>>& eds) {
        this->vertices = verts;
        this->edges = eds;
        constructAdjacency();
    }
    ///< The adjacency matrix. Index [row][col] stores the edge weight.
    std::vector<std::vector<int> > adjacencyMatrix;
    /**
            * @brief Rebuilds the adjacency matrix from the internal edges vector.
            */
    void constructAdjacency() override {
        int n = this->vertices.size();
        adjacencyMatrix.assign(n, std::vector<int>(n, 0));
        for (auto &edge: this->edges) {
            if (edge.isActive()) {
                adjacencyMatrix[edge.from][edge.to] = edge.weight;
                adjacencyMatrix[edge.to][edge.from] = edge.weight;
            }
        }
    }
    /**
             * @brief Adds an edge and rebuilds the adjacency matrix.
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
             * @brief Adds a vertex and rebuilds the (inefficient) adjacency matrix.
             * @param id The public ID for the new vertex.
             * @param data The data (of type T) to store in the vertex.
             */
    void addVertex(int id, T data)  override {
        if (this->findIndexById(id) != -1) return;
        Vertex<T> vertex; vertex.setId(id); vertex.setData(data);
        this->vertices.push_back(vertex);
        constructAdjacency();
    }
    /**
             * @brief Prints a user-friendly representation of the adjacency matrix to the console.
             */
    void print() const override{
        int n = adjacencyMatrix.size();
        if (n == 0) {
            std::cout << "Graph is empty.\n";
            return;
        }

        std::cout << "    ";
        for (const auto &v : this->vertices)
            std::cout << v.getId() << " ";
        std::cout << "\n";

        for (int i = 0; i < n; ++i) {
            std::cout << this->vertices[i].getId() << " : ";
            for (int j = 0; j < n; ++j) {
                std::cout << adjacencyMatrix[i][j] << " ";
            }
            std::cout << "\n";
        }
    }
    /**
             * @brief Removes a vertex (soft delete) and rebuilds the adjacency matrix.
             * @param id The public ID of the vertex to remove.
             */
    void removeVertex(int id) override {
        int idx = this->findIndexById(id);
        if (idx == -1) return;

        this->vertices[idx].setId(-1);
        this->vertices[idx].setData(T{});

        for (auto &edge : this->edges) {
            if (edge.from == idx || edge.to == idx) edge.disconnect();
        }

        constructAdjacency();
    }
    /**
            * @brief Removes an edge (soft delete) and rebuilds the adjacency matrix.
            * @param fId The public ID of the 'from' vertex.
            * @param tId The public ID of the 'to' vertex.
            */
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

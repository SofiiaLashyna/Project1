#ifndef VERTEX_H
#define VERTEX_H
/**
 * @file Vertex.h
 * @brief Defines the Vertex class used by the Graph.
 */

/**
 * @class Vertex
 * @brief A data container for a single vertex in the graph.
 * @tparam T The type of data to store (e.g., std::string or CelestialObject*).
 */
template<typename T>
class Vertex {
private:
    int id; ///< The public, stable ID of the vertex.
    T data; ///< The data payload (e.g., name or pointer).
    bool visited = false; ///< A flag used by graph algorithms (BFS, DFS).

public:
    /**
     * @brief Marks the vertex as visited.
     */
    void markVisited() { visited = true; }
    /**
     * @brief Resets the visited flag to false.
     */
    void resetVisited() { visited = false; }
 /**
        * @brief Checks if the vertex has been visited.
        * @return True if visited, false otherwise.
        */
    bool isVisited() const { return visited; }
 /**
        * @brief Gets the public ID of the vertex.
        * @return The ID.
        */
    int getId() const { return id; }
    /**
        * @brief Sets the public ID of the vertex.
        * @param index The new ID.
        */
    void setId(int index) { id = index; }
    /**
        * @brief Gets the data payload of the vertex.
        * @return The data (of type T).
        */
    T getData() const { return data; }
    /**
        * @brief Sets the data payload of the vertex.
        * @param d The new data.
        */
    void setData(T d) { data = d; }
    /**
     * @brief Prints the vertex data to the console (for debugging).
     */
    void printData() const { std::cout << id << " " << data; }
};

#endif //VERTEX_H

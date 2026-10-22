#ifndef GRAPHALGORITHM_H
#define GRAPHALGORITHM_H

#include <iostream>
#include <type_traits>

/**
 * @file GraphAlgorithm.h
 * @brief Provides the base interface for graph traversal and pathfinding strategies.
 */

/**
 * @class GraphAlgorithm
 * @brief Abstract interface for graph algorithms implementing the Strategy Pattern.
 *
 * This class serves as a base for all concrete graph algorithms (e.g., Dijkstra, BFS, DFS).
 * It includes template-based utility methods to handle different vertex data types.
 *
 * @tparam GraphType The type of graph the algorithm will operate on (e.g., GraphList or GraphMatrix).
 * @tparam T The type of data stored within the graph vertices.
 */
template<typename GraphType, typename T>
class GraphAlgorithm {
protected:
    /**
     * @brief Specialized template to print data when T is a pointer.
     * * Uses `std::enable_if` to ensure this version is used only if T is a pointer type.
     * Expects the underlying object to have a `getName()` method.
     * * @param data The pointer to the vertex data.
     */
    template <typename U = T>
    typename std::enable_if<std::is_pointer<U>::value>::type
    printVertexData(const U& data) {
        std::cout << data->getName() << " ";
    }

    /**
     * @brief Specialized template to print data when T is NOT a pointer.
     * * Uses `std::enable_if` to ensure this version is used for primitive or value types.
     * Directly outputs the data to the standard stream.
     * * @param data The vertex data value.
     */
    template <typename U = T>
    typename std::enable_if<!std::is_pointer<U>::value>::type
    printVertexData(const U& data) {
        std::cout << data << " ";
    }

public:
    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~GraphAlgorithm() = default;

    /**
     * @brief Pure virtual method that executes the specific graph algorithm.
     * * Must be implemented by all concrete strategy classes.
     * * @param g A reference to the graph object.
     * @param startId The ID of the starting vertex.
     * @param endId The ID of the target vertex (optional, defaults to -1).
     * @return int The result of the algorithm (e.g., path weight, number of nodes visited),
     * typically returns -1 if no result is found.
     */
    virtual int run(GraphType &g, int startId, int endId = -1) = 0;
};

#endif //GRAPHALGORITHM_H
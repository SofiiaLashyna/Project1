#ifndef BFS_H
#define BFS_H
#include "Algorithm.h"
#include "GraphList.h"
/**
 * @file BFS.h
 * @brief Defines the BFSAlgorithm class implementing the Strategy
 *        pattern for Breadth-First Search on a GraphList.
 */

/**
 * @class BFSAlgorithm
 * @brief A strategy class that executes BFS on a GraphList.
 *
 * @tparam T The data type stored in graph vertices.
 */
template<typename T>
class BFSAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    /**
     * @brief Performs a Breadth-First Search (BFS) on a graph
     *        represented by an adjacency list.
     *
     * This method overrides the Strategy interface. It prints
     * the traversal order to std::cout and does not modify the graph.
     *
     * @param g The GraphList object to traverse.
     * @param startId ID of the starting vertex.
     * @param endId Unused parameter, present only to match the base signature.
     *
     * @return Always returns 0.
     */
    int run(GraphList<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphList<T>, T> alg(g);
        alg.BFS_list(startId);
        return 0;
    }

    /// @brief Default destructor.
    ~BFSAlgorithm() = default;
};





#endif //BFS_H

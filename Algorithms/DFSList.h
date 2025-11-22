#ifndef DFS_H
#define DFS_H
#include "Algorithm.h"
#include "GraphList.h"
/**
 * @file DFS.h
 * @brief Defines the DFSAlgorithm class implementing the Strategy
 *        pattern for Depth-First Search on a GraphList.
 */

/**
 * @class DFSAlgorithm
 * @brief A strategy class that executes DFS on a GraphList.
 *
 * @tparam T The type stored in graph vertices.
 */
template<typename T>
class DFSAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    /**
     * @brief Performs iterative DFS on a graph represented by an adjacency list.
     *
     * Prints the traversal order to std::cout. The graph remains unchanged.
     *
     * @param g GraphList object to traverse.
     * @param startId Starting vertex ID.
     * @param endId Unused.
     *
     * @return Always returns 0.
     */
    int run(GraphList<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphList<T>, T> alg(g);
        alg.DFS_list(startId);
        return 0;
    }

    /// @brief Default destructor.
    ~DFSAlgorithm() = default;
};


#endif //DFS_H

#ifndef BFS_MATRIX_H
#define BFS_MATRIX_H
#include "Algorithm.h"
#include "GraphMatrix.h"
/**
 * @file BFSMatrix.h
 * @brief Defines the BFSMatrixAlgorithm class implementing the Strategy
 *        pattern for BFS on a GraphMatrix.
 */

/**
 * @class BFSMatrixAlgorithm
 * @brief Strategy class executing BFS on a GraphMatrix.
 *
 * @tparam T The data type in graph vertices.
 */
template<typename T>
class BFSMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    /**
     * @brief Performs BFS on a graph represented by an adjacency matrix.
     *
     * The traversal order is printed to std::cout. The graph is not modified.
     *
     * @param g The GraphMatrix object to traverse.
     * @param startId ID of the starting vertex.
     * @param endId Ignored, kept for interface compatibility.
     *
     * @return Always returns 0.
     */
    int run(GraphMatrix<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphMatrix<T>, T> alg(g);
        alg.BFS_matrix(startId);
        return 0;
    }

    /// @brief Default destructor.
    ~BFSMatrixAlgorithm() = default;
};

#endif //BFS_MATRIX_H

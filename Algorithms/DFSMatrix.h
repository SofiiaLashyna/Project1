#ifndef DFS_MATRIX_H
#define DFS_MATRIX_H

#include "Algorithm.h"
#include "GraphMatrix.h"
/**
 * @file DFSMatrix.h
 * @brief Defines the DFSMatrixAlgorithm class implementing the Strategy
 *        pattern for DFS on a GraphMatrix.
 */

/**
 * @class DFSMatrixAlgorithm
 * @brief Strategy class executing DFS on a GraphMatrix.
 *
 * @tparam T Stored data type.
 */
template<typename T>
class DFSMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    /**
     * @brief Performs iterative DFS on an adjacency-matrix graph.
     *
     * Prints the visitation order to std::cout. The graph is not modified.
     *
     * @param g GraphMatrix object.
     * @param startId Starting vertex ID.
     * @param endId Ignored.
     *
     * @return Always returns 0.
     */
    int run(GraphMatrix<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphMatrix<T>, T> alg(g);
        alg.DFS_matrix(startId);
        return 0;
    }

    /// @brief Default destructor.
    ~DFSMatrixAlgorithm() = default;
};

#endif //DFS_MATRIX_H

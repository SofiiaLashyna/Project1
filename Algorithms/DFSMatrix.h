#ifndef DFS_MATRIX_H
#define DFS_MATRIX_H

#include "Algorithm.h"
#include "GraphMatrix.h"

template< typename T>
class DFSMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    int run(GraphMatrix<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphMatrix<T>, T> alg(g);
        alg.DFS_matrix(startId);
        return 0;
    }
    ~DFSMatrixAlgorithm() = default;
};

#endif //DFS_MATRIX_H

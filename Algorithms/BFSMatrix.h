#ifndef BFS_MATRIX_H
#define BFS_MATRIX_H
#include "Algorithm.h"
#include "GraphMatrix.h"
template< typename T>
class BFSMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    int run(GraphMatrix<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphMatrix<T>, T> alg(g);
        alg.BFS_matrix(startId);
        return 0;
    }
    ~BFSMatrixAlgorithm() = default;
};
#endif //BFS_MATRIX_H

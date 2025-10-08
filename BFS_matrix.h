#ifndef BFS_MATRIX_H
#define BFS_MATRIX_H
#include "Algorithm.h"
#include "Graph_matrix.h"
template< typename T>
class BFSMatrixAlgorithm : public GraphAlgorithm<Graph_matrix<T>, T> {
public:
    int run(Graph_matrix<T>& g, int startId, int endId = -1) override {
        Algorithms<Graph_matrix<T>, T> alg(g);
        alg.BFS_matrix(startId);
        return 0;
    }
    ~BFSMatrixAlgorithm() = default;
};
#endif //BFS_MATRIX_H

#ifndef DFS_MATRIX_H
#define DFS_MATRIX_H

#include "Algorithm.h"
#include "Graph_matrix.h"

template< typename T>
class DFSMatrixAlgorithm : public GraphAlgorithm<Graph_matrix<T>, T> {
public:
    int run(Graph_matrix<T>& g, int startId, int endId = -1) override {
        Algorithms<Graph_matrix<T>, T> alg(g);
        alg.DFS_matrix(startId);
        return 0;
    }
    ~DFSMatrixAlgorithm() = default;
};

#endif //DFS_MATRIX_H

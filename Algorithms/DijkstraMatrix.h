#ifndef DIJKSTRA_MATRIX_H
#define DIJKSTRA_MATRIX_H
#include "Algorithm.h"
#include "GraphMatrix.h"

template< typename T>
class DijkstraAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    int run(GraphMatrix<T>& g, int startId, int endId) override {
        Algorithms<GraphMatrix<T>, T> alg(g);
        int res = alg.Dijkstra_list(startId, endId);
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    ~DijkstraAlgorithm()= default;
};

#endif //DIJKSTRA_MATRIX_H

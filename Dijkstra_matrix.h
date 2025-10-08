#ifndef DIJKSTRA_MATRIX_H
#define DIJKSTRA_MATRIX_H
#include "Algorithm.h"
#include "Graph_matrix.h"

template< typename T>
class DijkstraAlgorithm : public GraphAlgorithm<Graph_matrix<T>, T> {
public:
    int run(Graph_matrix<T>& g, int startId, int endId) override {
        Algorithms<Graph_matrix<T>, T> alg(g);
        int res = alg.Dijkstra_list(startId, endId);
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    ~DijkstraAlgorithm()= default;
};

#endif //DIJKSTRA_MATRIX_H

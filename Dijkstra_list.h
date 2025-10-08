#ifndef DJIKSTRA_H
#define DJIKSTRA_H
#include "Algorithm.h"
#include "Graph_list.h"

template< typename T>
class DijkstraAlgorithm : public GraphAlgorithm<Graph_list<T>, T> {
public:
    int run(Graph_list<T>& g, int startId, int endId) override {
        Algorithms<Graph_list<T>, T> alg(g);
        int res = alg.Dijkstra_list(startId, endId);
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    ~DijkstraAlgorithm()= default;
};


#endif //DJIKSTRA_H

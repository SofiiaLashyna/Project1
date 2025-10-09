#ifndef DJIKSTRA_H
#define DJIKSTRA_H
#include "Algorithm.h"
#include "GraphList.h"

template< typename T>
class DijkstraAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    int run(GraphList<T>& g, int startId, int endId) override {
        Algorithms<GraphList<T>, T> alg(g);
        int res = alg.Dijkstra_list(startId, endId);
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    ~DijkstraAlgorithm()= default;
};


#endif //DJIKSTRA_H

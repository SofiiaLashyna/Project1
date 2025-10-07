#ifndef DJIKSTRA_H
#define DJIKSTRA_H
#include "Algorithm.h"

template<typename GraphType, typename T>
class DijkstraAlgorithm : public GraphAlgorithm<GraphType, T> {
public:
    int run(GraphType& g, int startId, int endId) override {
        Algorithms<GraphType, T> alg(g);
        int res = alg.Dijkstra(startId, endId);
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    ~DijkstraAlgorithm()= default;
};

#endif //DJIKSTRA_H

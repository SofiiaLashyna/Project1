#ifndef BFS_H
#define BFS_H
#include "Algorithm.h"


template<typename GraphType, typename T>
class BFSAlgorithm : public GraphAlgorithm<GraphType, T> {
public:
    int run(GraphType& g, int startId, int endId = -1) override {
        Algorithms<GraphType, T> alg(g);
        alg.BFS(startId);
        return 0;
    }
    ~BFSAlgorithm() = default;
};



#endif //BFS_H

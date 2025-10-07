#ifndef DFS_H
#define DFS_H
#include "Algorithm.h"

template<typename GraphType, typename T>
class DFSAlgorithm : public GraphAlgorithm<GraphType, T> {
public:
    int run(GraphType& g, int startId, int endId = -1) override {
        Algorithms<GraphType, T> alg(g);
        alg.DFS(startId);
        return 0;
    }
    ~DFSAlgorithm() = default;
};

#endif //DFS_H

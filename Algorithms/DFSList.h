#ifndef DFS_H
#define DFS_H
#include "Algorithm.h"
#include "GraphList.h"

template< typename T>
class DFSAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    int run(GraphList<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphList<T>, T> alg(g);
        alg.DFS_list(startId);
        return 0;
    }
    ~DFSAlgorithm() = default;
};

#endif //DFS_H

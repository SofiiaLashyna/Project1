#ifndef BFS_H
#define BFS_H
#include "Algorithm.h"
#include "GraphList.h"


template< typename T>
class BFSAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
    int run(GraphList<T>& g, int startId, int endId = -1) override {
        Algorithms<GraphList<T>, T> alg(g);
        alg.BFS_list(startId);
        return 0;
    }
    ~BFSAlgorithm() = default;
};




#endif //BFS_H

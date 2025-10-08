#ifndef BFS_H
#define BFS_H
#include "Algorithm.h"
#include "Graph_list.h"


template< typename T>
class BFSAlgorithm : public GraphAlgorithm<Graph_list<T>, T> {
public:
    int run(Graph_list<T>& g, int startId, int endId = -1) override {
        Algorithms<Graph_list<T>, T> alg(g);
        alg.BFS_list(startId);
        return 0;
    }
    ~BFSAlgorithm() = default;
};




#endif //BFS_H

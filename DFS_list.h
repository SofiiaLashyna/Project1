#ifndef DFS_H
#define DFS_H
#include "Algorithm.h"
#include "Graph_list.h"

template< typename T>
class DFSAlgorithm : public GraphAlgorithm<Graph_list<T>, T> {
public:
    int run(Graph_list<T>& g, int startId, int endId = -1) override {
        Algorithms<Graph_list<T>, T> alg(g);
        alg.DFS_list(startId);
        return 0;
    }
    ~DFSAlgorithm() = default;
};

#endif //DFS_H

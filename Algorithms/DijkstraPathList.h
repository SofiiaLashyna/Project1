#ifndef DIJKSTRAPATHLIST_H
#define DIJKSTRAPATHLIST_H

#include "Algorithm.h"
#include "GraphList.h"
#include <vector>

template<typename T>
class DijkstraPathList {
public:
    std::vector<int> findShortestPath(GraphList<T>& g, int startId, int endId) {
        Algorithms<GraphList<T>, T> alg(g);

        return alg.Dijkstra_findPath(startId, endId);
    }
};

#endif //DIJKSTRAPATHLIST_H

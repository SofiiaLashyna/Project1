#ifndef DJIKSTRA_H
#define DJIKSTRA_H
#include "Algorithm.h"
#include "GraphList.h"
/**
 * @file DijkstraList.h
 * @brief Defines the DijkstraAlgorithm class, which implements
 * the Strategy pattern for a Dijkstra's algorithm.
 */

/**
 * @class DijkstraListAlgorithm
 * @brief Strategy class that runs Dijkstra's shortest-path algorithm
 *        on a graph represented by an adjacency list.
 *
 * @tparam T The data stored in graph vertices.
 *
 * The algorithm computes the minimum path cost from `startId`
 * to `endId` using a priority queue. Works only with non-negative weights.
 *
 * The result (shortest distance) is returned and also printed to std::cout.
 */

template< typename T>
class DijkstraListAlgorithm : public GraphAlgorithm<GraphList<T>, T> {
public:
  /**
           * @brief Performs Dijkstra's algorithm for a graph
           * represented by an adjacency list.
           *
           * This is the implementation of the virtual 'run' method.
           * The traversal order is printed to std::cout.
           *
           * @param g The GraphList object to traverse.
           * @param startId The ID of the starting vertex.
           * @param endId The ID of the destination vertex.
           * @return The length of the shortest path, or -1 if no path is found.
           * @note Works only with non-negative weights.
           */
    int run(GraphList<T>& g, int startId, int endId) override {
        Algorithms<GraphList<T>, T> alg(g);
        int res = alg.Dijkstra_list(startId, endId);
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    /// @brief Default destructor.
    ~DijkstraListAlgorithm()= default;
};


#endif //DJIKSTRA_H

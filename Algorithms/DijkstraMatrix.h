#ifndef DIJKSTRA_MATRIX_H
#define DIJKSTRA_MATRIX_H
#include "Algorithm.h"
#include "GraphMatrix.h"
/**
 * @file DijkstraMatrix.h
 * @brief Defines the DijkstraMatrixAlgorithm class, which implements
 * the Strategy pattern for a Dijkstra's algorithm.
 */

/**
 * @class DijkstraMatrixAlgorithm
 * @brief Strategy class implementing Dijkstra's algorithm for
 *        graphs represented by an adjacency matrix.
 *
 * @tparam T The data stored in graph vertices.
 *
 * Computes the minimum path cost from `startId` to `endId`.
 * Returns -1 if no path exists. Assumes non-negative weights.
 */

template< typename T>
class DijkstraMatrixAlgorithm : public GraphAlgorithm<GraphMatrix<T>, T> {
public:
    /**
              * @brief Performs Dijkstra's algorithm for a graph
              * represented by an adjacency matrix.
              *
              * This is the implementation of the virtual 'run' method.
              * The traversal order is printed to std::cout.
              *
              * @param g The GraphMatrix object to traverse.
              * @param startId The ID of the starting vertex.
              * @param endId The ID of the destination vertex.
              * @return The length of the shortest path, or -1 if no path is found.
              */
    int run(GraphMatrix<T>& g, int startId, int endId) override {
        Algorithms<GraphMatrix<T>, T> alg(g);
        int res = alg.Dijkstra_matrix(startId, endId);
        std::cout << "Shortest path weight = " << res << std::endl;
        return res;
    }
    /// @brief Default destructor.
    ~DijkstraMatrixAlgorithm()= default;
};

#endif //DIJKSTRA_MATRIX_H

/**
* @file Algorithm.h
 * @brief Contains implementations of classical graph algorithms (BFS, DFS, Dijkstra)
 * implemented as a templated utility class.
 *
 * This file also defines an abstract interface GraphAlgorithm for
 * implementing the Strategy pattern when selecting graph algorithms at runtime.
 */
#ifndef ALGORITHM_H
#define ALGORITHM_H
#include "Graph.h"
#include "Queue.h"
/**
 * @class Algorithms
 * @brief A collection of classic graph algorithms (BFS, DFS, Dijkstra)
 * implemented for a user-supplied graph type.
 *
 * This is a templated utility class that operates on an external graph object.
 * The graph type is expected to provide:
 *   - `findIndexById(int)` — returns internal vertex index by ID
 *   - `getVertices()` — returns container of vertices
 *   - `adjacencyList` — std::vector<std::vector<std::pair<int,int>>>
 *      where pair = (neighborIndex, weight).
 *   - `adjacencyMatrix` — 2D matrix of weights (0 = no edge)
 *
 * @tparam GraphType The graph representation (list/matrix) that algorithms operate on.
 * @tparam T The data type stored in each graph vertex.
 */
template<typename GraphType, typename T>
class Algorithms {
    GraphType &graph;

public:
    /**
     * @brief Constructor for the Algorithms class.
     * @param g A reference to the graph that the algorithms will run on.
     */
    Algorithms(GraphType &g) : graph(g) {
    }

    /**
 * @brief Performs Breadth-First Search using an adjacency list.
 *
 * The traversal starts from the vertex with ID `startId`.
 * This function does not return the list of visited vertices — the
 * visitation order is printed directly to the standard output.
 *
 * BFS uses a queue to ensure that vertices are processed in
 * increasing order of their distance from the start vertex.
 *
 * @param startId The ID of the starting vertex.
 *
 * @note Each vertex is visited at most once.
 * @note Neighbors are processed in the natural order of adjacencyList[v].
 */

    void BFS_list(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1) return;

        std::vector<bool> visited(graph.getVertices().size(), false);
        Queue<int> q;
        q.push(start, 0);
        visited[start] = true;

        std::cout << "BFS order: ";
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            std::cout << graph.getVertices()[u].getData() << " ";
            for (auto &[v, w]: graph.adjacencyList[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v, 0);
                }
            }
        }
        std::cout << std::endl;
    }

    /**
 * @brief Performs Breadth-First Search using an adjacency matrix.
 *
 * The traversal begins at the vertex with ID `startId`.
 * The method prints the BFS visitation order instead of returning it.
 *
 * BFS processes vertices level-by-level using a queue. Only matrix
 * cells with positive weights are treated as valid edges.
 *
 * @param startId The ID of the starting vertex.
 *
 * @note Vertices are checked in ascending index order (0..n-1).
 * @note A matrix value of 0 means “no edge” (except diagonal entries).
 */

    void BFS_matrix(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1 || graph.adjacencyMatrix.empty()) return;

        int n = graph.adjacencyMatrix.size();
        std::vector<bool> visited(n, false);
        Queue<int> q;
        q.push(start, 0);
        visited[start] = true;

        std::cout << "BFS (matrix) order: ";

        while (!q.empty()) {
            auto [u, _] = q.popMin();
            std::cout << graph.getVertices()[u].getData() << " ";

            for (int v = 0; v < n; ++v) {
                if (graph.adjacencyMatrix[u][v] && !visited[v]) {
                    visited[v] = true;
                    q.push(v, 0);
                }
            }
        }

        std::cout << std::endl;
    }

    /**
 * @brief Performs an iterative Depth-First Search using adjacency lists.
 *
 * DFS traversal starts at the vertex with ID `startId` and uses an
 * explicit LIFO stack. The order of visited vertices is printed to
 * the standard output.
 *
 * @param startId The ID of the starting vertex.
 *
 * @note Neighbors are pushed onto the stack in reverse order to ensure
 *       a left-to-right traversal that matches the adjacency list order.
 * @note Each vertex is processed exactly once.
 */

    void DFS_list(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1) return;

        std::vector<bool> visited(graph.getVertices().size(), false);
        std::vector<int> stack;
        stack.push_back(start);

        std::cout << "DFS (iterative) order: ";
        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();
            if (visited[v]) continue;
            visited[v] = true;
            std::cout << graph.getVertices()[v].getData() << " ";

            for (int i = (int) graph.adjacencyList[v].size() - 1; i >= 0; --i) {
                int neighbor = graph.adjacencyList[v][i].first;
                if (!visited[neighbor]) stack.push_back(neighbor);
            }
        }
        std::cout << std::endl;
    }

    /**
 * @brief Performs an iterative Depth-First Search using an adjacency matrix.
 *
 * The traversal begins at vertex `startId`. The method prints the
 * visitation sequence without returning it.
 *
 * DFS uses an explicit stack. Since the adjacency matrix stores neighbors
 * by numeric index, the algorithm scans the row of the matrix in reverse
 * order (n-1 → 0) so that the resulting traversal resembles a natural
 * left-to-right DFS.
 *
 * @param startId The ID of the starting vertex.
 *
 * @note A matrix entry > 0 is treated as an edge.
 * @note Vertices are visited at most once.
 */

    void DFS_matrix(int startId) {
        int start = graph.findIndexById(startId);
        if (start == -1 || graph.adjacencyMatrix.empty())
            return;

        int n = graph.adjacencyMatrix.size();
        std::vector<bool> visited(n, false);
        std::vector<int> stack;
        stack.push_back(start);

        std::cout << "DFS (matrix) order: ";

        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();

            if (visited[v])
                continue;

            visited[v] = true;
            std::cout << graph.getVertices()[v].getData() << " ";

            for (int u = n - 1; u >= 0; --u) {
                if (graph.adjacencyMatrix[v][u] && !visited[u]) {
                    stack.push_back(u);
                }
            }
        }

        std::cout << std::endl;
    }


    /**
         * @brief Computes the shortest path between two vertices using Dijkstra’s algorithm
         * on adjacency lists.
         *
         * @param startId ID of the starting vertex.
         * @param endId ID of the target vertex.
         * @return Minimum distance between the vertices, or -1 if no path exists.
         *
         * @pre All edge weights must be non-negative.
         */

    int Dijkstra_list(int startId, int endId) {
        int start = graph.findIndexById(startId);
        int end = graph.findIndexById(endId);
        if (start == -1 || end == -1) return -1;

        int V = graph.getVertices().size();
        std::vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        Queue<int> q;
        q.push(start, 0);
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            for (auto &[v, w]: graph.adjacencyList[u]) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    q.push(v, dist[v]);
                }
            }
        }
        return dist[end] == INT_MAX ? -1 : dist[end];
    }

    /**
    * @brief Computes the shortest path using Dijkstra’s algorithm
    * on a graph represented by an adjacency matrix.
    *
    * @param startId ID of the starting vertex.
    * @param endId ID of the target vertex.
    * @return Shortest path distance, or -1 if unreachable.
    *
    * @note Cells with value 0 are interpreted as absence of an edge (except diagonal).
    * @pre Edge weights must be non-negative.
    */

    int Dijkstra_matrix(int startId, int endId) {
        int start = graph.findIndexById(startId);
        int end = graph.findIndexById(endId);
        if (start == -1 || end == -1) return -1;

        int V = graph.getVertices().size();
        std::vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        Queue<int> q;
        q.push(start, 0);
        while (!q.empty()) {
            auto [u, d] = q.popMin();
            int n = graph.adjacencyMatrix.size();
            for (int v = 0; v < n; ++v) {
                int w = graph.adjacencyMatrix[u][v];
                if (w > 0 && dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    q.push(v, dist[v]);
                }
            }
        }
        return dist[end] == INT_MAX ? -1 : dist[end];
    }
};

/**
 * @class GraphAlgorithm
 * @brief Abstract interface for graph algorithms (Strategy Pattern).
 *
 * Concrete strategies must implement `run()` to perform the selected algorithm
 * (e.g., BFS, DFS, Dijkstra).
 *
 * @tparam GraphType Graph type used by the strategy.
 * @tparam T Type stored in graph vertices.
 */
template<typename GraphType, typename T>
class GraphAlgorithm {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~GraphAlgorithm() = default;

    /**
         * @brief Pure virtual method to run the algorithm.
         *
         * @param g A reference to the graph.
         * @param startId The ID of the starting vertex.
         * @param endId The ID of the destination vertex (optional).
         * @return The result of the algorithm (e.g., distance).
         */
    virtual int run(GraphType &g, int startId, int endId = -1) = 0;
};


#endif //ALGORITHM_H

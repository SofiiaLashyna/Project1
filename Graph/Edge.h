#ifndef EDGE_H
#define EDGE_H
/**
 * @file Edge.h
 * @brief Defines the Edge class used by the Graph.
 */

/**
 * @class Edge
 * @brief Represents a connection (edge) between two vertices in a graph.
 * @tparam T The template parameter (templating for compatibility with Graph class).
 * @note This edge is conceptually undirected, since connects() checks both directions.
 */
template<typename T>
class Edge {
public:
    int from; ///< The index of the starting vertex.
    int to; ///< The index of the ending vertex.
    int weight; ///< The weight of the edge (e.g., distance).
    /**
             * @brief Default constructor. Initializes an inactive edge.
             */
    Edge() : from(-1), to(-1), weight(0) {
    }

    /**
             * @brief Sets the weight of the edge.
             * @param w The new weight.
             */
    void changeWeight(int w) { weight = w; }
    /**
     * @return The int value of the weight.
     */
    int getWeight() { return weight; }
    /**
             * @brief Connects two vertices.
             * @param u The index of the first vertex.
             * @param v The index of the second vertex.
             */
    void connect(int u, int v) {
        from = u;
        to = v;
    }

    /*
     * @brief Returns index of starting vertex
     */
    int getFrom() { return from; }
    /*
     * @brief Returns index of ending vertex
     */

    int getTo() { return to; }
    /**
             * @brief Resets the edge to an inactive state.
             */
    void disconnect() {
        from = -1;
        to = -1;
        weight = 0;
    }

    /**
             * @brief Checks if this edge connects two specific vertices (undirected).
             * @param u Index of the first vertex.
             * @param v Index of the second vertex.
             * @return True if it connects u and v, false otherwise.
             */
    bool connects(int u, int v) const { return (from == u && to == v) || (from == v && to == u); }
    /**
    * @brief Checks if the edge is currently active (connected).
    * @return True if active, false otherwise.
    */
    bool isActive() const { return from != -1 && to != -1; }
};

#endif //EDGE_H

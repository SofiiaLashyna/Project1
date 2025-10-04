#include <iostream>
#include "Graph.h"
int main()
{
    std::cout << "Hello, World!" << std::endl;

    Graph<std::string> galaxy;

    galaxy.vertices.push_back({0, "Sirius"});
    galaxy.vertices.push_back({1, "Vega"});
    galaxy.vertices.push_back({2, "Betelgeuse"});

    galaxy.edges.push_back({0, 1, 5});
    galaxy.edges.push_back({1, 2, 2});
    galaxy.edges.push_back({0, 2, 8});

    galaxy.constructAdjacency();

    galaxy.print();
    return 0;
}

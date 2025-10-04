#include <iostream>
#include "Graph.h"
int main()
{
    std::cout << "Hello, World!" << std::endl;

    Graph<std::string> galaxy;

    galaxy.addVertex(0,"Sirius");
    galaxy.addVertex(1,"Vega");
    galaxy.addVertex(2, "Betelgeus");
    galaxy.addVertex(3, "Polaris");

    galaxy.addEdge(0, 1, 5);
    galaxy.addEdge(1, 2, 2);
    galaxy.addEdge(0, 2, 8);
    galaxy.addEdge(2, 3, 4);

    galaxy.constructAdjacency();

    galaxy.print();
    std::cout << std::endl;

    galaxy.BFS(0);
    galaxy.DFS(0);

    std::cout << "\nNeighbors of Sirius: ";
    for(auto n : galaxy.getNeighbors(0)) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    galaxy.removeEdge(0,2);
    std::cout << "After removing edge 0 2: \n";
    galaxy.print();

    galaxy.removeVertex(1);
    std::cout << "After removing Vega: \n";
    galaxy.print();

    int dist = galaxy.Dijkstra(0,3);
    std::cout << "Shortest distane from Sirius to Polaris: " << dist;

    return 0;
}

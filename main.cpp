#include <iostream>
#include "Algorithm.h"
// #include "Graph.h"
#include "Galaxy.h"
#include "CelestialObject.h"
#include "Planet.h"
#include "Star.h"
#include "Dijkstra.h"
#include "BFS.h"
#include "DFS.h"
#include "Graph_matrix.h"
#include "Graph_list.h"


int main() {
    Star sun("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
    Star betelgeuse("Betelgeuse", 11.6, 3500, Star::starType::Red_Giant);
    Star sirius_b("Sirius B", 1.02, 25000, Star::starType::White_Dwarf);

    Planet earth("Earth", 1.0, 1.0, 29.78, 0.0, Planet::planetType::Terrestrial_Planet);
    Planet jupiter("Jupiter", 317.8, 5.2, 13.07, 0.0, Planet::planetType::Gas_Giant);

    StarSystem solar(1, "Solar system", sun);
    StarSystem sir_b(2, " Sirius binary star system", sirius_b);
    StarSystem betel(3, "Betelgeuse system", betelgeuse);

    solar.addPlanet(earth);
    solar.addPlanet(jupiter);

    Galaxy<Graph_list<StarSystem*>> MilkyWay;
    MilkyWay.addSystem(solar);
    MilkyWay.addSystem(sir_b);
    MilkyWay.addSystem(betel);

    MilkyWay.connectSystems(1, 2, 4);
    MilkyWay.connectSystems(2, 3, 6);
    MilkyWay.connectSystems(1, 3, 16);


    std::cout << "~~GALAXY~~\n";
    MilkyWay.showGalaxy();
    std::cout << "\n~~STAR SYSTEM DETAILS~~\n";
    solar.showSystem();
    std::cout << std::endl;
    sir_b.showSystem();
    std::cout << std::endl;


    GraphAlgorithm<Graph_list<StarSystem*>, StarSystem *> *distance = new DijkstraAlgorithm<Graph_list<StarSystem*>, StarSystem *>();
    distance->run(MilkyWay.getGraph(), 1, 3);
    return 0;

    // ----------------------------------------------------
    //  std::cout << "Hello, World!" << std::endl;
    //
    //  Graph<std::string> galaxy;
    //
    //  galaxy.addVertex(0, "Sirius");
    //  galaxy.addVertex(1, "Vega");
    //  galaxy.addVertex(2, "Betelgeus");
    //  galaxy.addVertex(3, "Polaris");
    //
    //  galaxy.addEdge(0, 1, 5);
    //  galaxy.addEdge(1, 2, 2);
    //  galaxy.addEdge(0, 2, 8);
    //  galaxy.addEdge(2, 3, 4);
    //
    //  galaxy.constructAdjacency();
    //
    //  galaxy.print();
    //  std::cout << std::endl;
    //
    //  // galaxy.BFS(0);
    //  // galaxy.DFS(0);
    //  // std::cout << std::endl;
    //  // galaxy.DFS_iterative(0);
    //
    //  std::cout << "\nNeighbors of Sirius: ";
    //  for (auto n: galaxy.getNeighbors(0)) {
    //      std::cout << n << " ";
    //  }
    //  std::cout << std::endl;
    //
    //  galaxy.removeEdge(0, 2);
    //  std::cout << "After removing edge 0 2: \n";
    //  galaxy.print();
    //
    //  galaxy.removeVertex(1);
    //  std::cout << "After removing Vega: \n";
    //  galaxy.print();
    //
    // Algorithms<std::string> alg(galaxy);
    // std::cout << "Graph connected (list): " << std::boolalpha << alg.isConnectedList() << std::endl;
    // std::cout << "Graph connected (matrix): " << std::boolalpha << alg.isConnectedMatrix() << std::endl;

    // int dist = galaxy.Dijkstra(0, 3);
    // if (dist == -1)
    //     std::cout << "No path exists between Sirius and Polaris.\n";
    // else
    //     std::cout << "Shortest distance from Sirius to Polaris: " << dist << "\n";
    //
    return 0;
}

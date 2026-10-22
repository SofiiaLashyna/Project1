/**
* @file BasicUsageExample.cpp
 * @brief Demonstrates creating a Galaxy with various celestial objects.
 * @example BasicUsageExample.cpp
 * This is an example of how to:
 * 1. Create Stars, Planets, and Nebulae.
 * 2. Group them into StarSystems.
 * 3. Add systems and nebulae to a polymorphic Galaxy.
 * 4. Connect objects and run algorithms (Dijkstra, BFS, DFS) on the graph.
 */

#include "GraphList.h"
#include "Galaxy.h"
#include "StarSystem.h"
#include "Nebula.h"
#include "Planet.h"
#include "GraphAlgorithms.h"      // Base class for algorithms
#include "DijkstraList.h"   // Include Dijkstra
#include "BFSList.h"        // Include BFS
#include "DFSList.h"        // Include DFS
#include "IsConnectedList.h"// Include IsConnected
#include <iostream>

int test_examples() {
    // 1. Create objects (all on the heap 'new' because they will be owned by others)

    // Stars
    Star* sun = new Star("The Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
    Star* sirius_b = new Star("Sirius B", 1.018, 25000, Star::starType::White_Dwarf);
    Star* betelgeuse = new Star("Betelgeuse", 20.0, 3500, Star::starType::Red_Giant);

    // Planets
    Planet* earth = new Planet("Earth", 1.0, 1.0, 30.0, 0.0, Planet::planetType::Terrestrial_Planet, true);
    Planet* jupiter = new Planet("Jupiter", 317.8, 5.2, 13.0, 1.3, Planet::planetType::Gas_Giant, false);

    // 2. Create StarSystems (they take ownership of the Star*)
    StarSystem* solar = new StarSystem(1, "Solar System", sun);
    StarSystem* sirius = new StarSystem(2, "Sirius Binary System", sirius_b);
    StarSystem* betel = new StarSystem(3, "Betelgeuse System", betelgeuse);

    // Add planets to systems (StarSystem takes ownership of Planet*)
    solar->addPlanet(earth);
    solar->addPlanet(jupiter);

    // Create a Nebula
    Nebula* cat_eye = new Nebula("Cat's Eye Nebula", 0.1, Nebula::nebulaType::Planetary);

    // 3. Create the Galaxy
    // It will hold all object types (StarSystem, Nebula, etc.)
    Galaxy<GraphList<CelestialObject*>> galaxy("Milky Way");

    // Add objects (Galaxy takes ownership of StarSystem* and Nebula*)
    galaxy.addObject(solar);     ///< Index 0 (ID 1)
    galaxy.addObject(sirius);    ///< Index 1 (ID 2)
    galaxy.addObject(betel);     ///< Index 2 (ID 3)
    galaxy.addObject(cat_eye);   ///< Index 3 (ID 4 - from Nebula constructor)

    // 4. Connect objects (using their public IDs)
    galaxy.connectObjects(0, 1, 4);  ///< Solar (0) <-> Sirius (1)
    galaxy.connectObjects(1, 2, 6);  ///< Sirius (1) <-> Betelgeuse (2)
    galaxy.connectObjects(0, 2, 16); ///< Solar (0) <-> Betelgeuse (2)
    galaxy.connectObjects(1, 3, 30); ///< Sirius (1) <-> Cat's Eye (3)
    // 5. Show what we built
    std::cout << "~~~ GALAXY INFO ~~~" << std::endl;
    galaxy.showGalaxy();

    std::cout << "\n\n~~~ STAR SYSTEM DETAILS ~~~" << std::endl;
    solar->displayInfo();
    std::cout << std::endl;

    // 6. Run algorithms
    std::cout << "\n\n~~~ ALGORITHMS ~~~" << std::endl;

    // --- Dijkstra Example ---
    std::cout << "\n--- Dijkstra (Solar to Betelgeuse) ---" << std::endl;
    GraphAlgorithm<GraphList<CelestialObject*>, CelestialObject*>* dijkstra;
    dijkstra = new DijkstraListAlgorithm<CelestialObject*>();
    dijkstra->run(galaxy.getGraph(), 1, 3); ///< Find path from Solar (1) to Betelgeuse (3)
    delete dijkstra;

    // --- BFS Example ---
    std::cout << "\n--- BFS (starting from Solar System) ---" << std::endl;
    GraphAlgorithm<GraphList<CelestialObject*>, CelestialObject*>* bfs;
    bfs = new BFSListAlgorithm<CelestialObject*>();
    bfs->run(galaxy.getGraph(), 1); ///< Start traversal from Solar (ID 1)
    delete bfs;

    // --- DFS Example ---
    std::cout << "\n--- DFS (starting from Solar System) ---" << std::endl;
    GraphAlgorithm<GraphList<CelestialObject*>, CelestialObject*>* dfs;
    dfs = new DFSListAlgorithm<CelestialObject*>();
    dfs->run(galaxy.getGraph(), 1); ///< Start traversal from Solar (ID 1)
    delete dfs;

    // --- IsConnected Example ---
    std::cout << "\n--- Connectivity Check ---" << std::endl;
    GraphAlgorithm<GraphList<CelestialObject*>, CelestialObject*>* connected;
    connected = new IsConnectedList<CelestialObject*>();
    int isConnected = connected->run(galaxy.getGraph(), 1);
    if (isConnected == 1) {
        std::cout << "The galaxy graph is connected." << std::endl;
    } else {
        std::cout << "The galaxy graph is NOT connected." << std::endl;
    }
    delete connected;

    /* 7. Clean up
    * The 'galaxy' object (on the stack) is about to be destroyed.
    * Its destructor will automatically 'delete' solar, sirius, betel, and cat_eye.
    * The StarSystem destructors will automatically 'delete' their stars and planets.
    * All memory is cleaned up!
    */
    std::cout << "\nExample complete. Cleaning up." << std::endl;
    return 0;
}
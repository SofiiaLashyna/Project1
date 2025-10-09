#ifndef GALAXYTESTING_H
#define GALAXYTESTING_H

#include <cassert>
#include <iostream>
#include <sstream>
#include "Galaxy.h"
#include "CelestialObject.h"
#include "Star.h"
#include "Planet.h"

template<typename GraphType>
class GalaxyTesting {
public:
    static void runAll() {
        testInitialization();
        testAddObject();
        testConnectObjects();
        testGetGraph();
        testShowGalaxy();
    }

private:
    static void testInitialization() {
        Galaxy<GraphType> g("Milky Way");

        assert(!g.getGraph().getVertices().size() && "Graph should be empty initially");
        std::cout << "1/5 passed\n";
    }

    static void testAddObject() {
        Galaxy<GraphType> g;

        Star star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
        g.addObject(&star);

        assert(g.getGraph().getVertices().size() == 1 && "Graph should have 1 vertex after adding object");

        g.addObject(nullptr);
        assert(g.getGraph().getVertices().size() == 2 && "Graph should handle nullptr");

        std::cout << "2/5 passed\n";
    }

    static void testConnectObjects() {
        Galaxy<GraphType> g;

        Star star1("Alpha", 1.0, 5000, Star::starType::Main_sequence_Star);
        Star star2("Beta", 1.0, 5000, Star::starType::Main_sequence_Star);
        g.addObject(&star1);
        g.addObject(&star2);

        g.connectObjects(1, 2, 100);

        assert(g.getGraph().edgeExists(1, 2) && "Vertices should be connected");
        assert(!g.getGraph().edgeExists(1, 3) && "Non-existent vertex should not crash");

        std::cout << "3/5 passed\n";
    }

    static void testGetGraph() {
        Galaxy<GraphType> g;

        Star star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
        g.addObject(&star);

        auto &graph = g.getGraph();
        assert(&graph == &g.getGraph() && "getGraph should return the same object");

        auto verts = graph.getVertices();
        assert(!verts.empty() && verts[0].data != nullptr && "Vertex should store object");

        std::cout << "4/5 passed\n";
    }

    static void testShowGalaxy() {
        Galaxy<GraphType> g;

        Star star("Sun", 1.0, 5778, Star::starType::Main_sequence_Star);
        Planet planet("Earth", 1.0, 1.0, 30.0, 0.0, Planet::planetType::Terrestrial_Planet);

        g.addObject(&star);
        g.addObject(&planet);

        std::stringstream out;
        std::streambuf* oldCout = std::cout.rdbuf(out.rdbuf());

        g.showGalaxy();

        std::cout.rdbuf(oldCout);

        std::string output = out.str();
        assert(output.find("Sun") != std::string::npos && "Output should contain star name");
        assert(output.find("Earth") != std::string::npos && "Output should contain planet name");

        std::cout << "5/5 passed\n";
    }
};
#endif //GALAXYTESTING_H

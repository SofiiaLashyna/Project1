#ifndef GALAXY_H
#define GALAXY_H
#include <utility>
#include <iostream>
#include <vector>
#include "CelestialObject.h"



template<typename GraphType>
class Galaxy {
private:
    std::string name;
    GraphType systemGraph;
    std::vector<CelestialObject *> celestial_objects;

public:
    Galaxy(std::string name = "Unnamed Galaxy") : name(std::move(name)) {
    }

    void addObject(CelestialObject *obj) {
        celestial_objects.push_back(obj);
        systemGraph.addVertex(celestial_objects.size(), obj);
    }

    std::vector<CelestialObject *> getObject() {
        return celestial_objects;
    }

    void connectObjects(int id1, int id2, int distance) {
        systemGraph.addEdge(id1, id2, distance);
    }

    GraphType &getGraph() {
        return systemGraph;
    }


    void showGalaxy() {
        std::cout << "Galaxy: " << name << "\n";

        for (auto i: celestial_objects) {
            i->displayInfo();
        }
    }
};
#endif //GALAXY_H
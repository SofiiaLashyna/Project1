#ifndef GALAXY_H
#define GALAXY_H
#include <vector>
#include "CelestialObject.h"
#include "Graph.h"
#include "Planet.h"
#include "Star.h"

class StarSystem {
private:
    int id;
    std::string name;
    Star centralStar;
    std::vector<Planet> planets;
public:
    StarSystem(int id, std::string name, Star& star);

    void addPlanet(Planet& planet);
    void removePlanet(const std::string &name);

    Star& getStar();
    std::vector<Planet>& getPlanets();

    void showSystem();
    int getId();
    std::string getName();
};
template<typename GraphType>
class Galaxy {
private:
    std::string name;
    GraphType systemGraph;
    std::vector<StarSystem> systems;
public:
    Galaxy() = default;

    void addSystem(StarSystem& system) {
        systems.push_back(system);
        systemGraph.addVertex(systems.back().getId(), &systems.back());
    }

    void connectSystems(int id1, int id2, int distance) {
        systemGraph.addEdge(id1, id2, distance);
    }

    GraphType& getGraph() {
        return systemGraph;
    }


    void showGalaxy() {

        std::cout << "Galaxy: " << name << "\n Star systems: ";
        int k = 1;
        for (auto i: systems) {
            std::cout << k << ") " << i.getName() << std::endl;
            std::vector<Planet> planet = i.getPlanets();
            int j = planet.size();
            std::cout << " Number of planets: " << j << std::endl;
            k++;
        }
    }
};
#endif //GALAXY_H

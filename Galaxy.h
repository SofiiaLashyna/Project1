#ifndef GALAXY_H
#define GALAXY_H
#include <vector>
#include "CelestialObjects.h"
#include "Graph.h"

class StarSystem {
private:
    Star centralStar;
    std::vector<Planet> planets;
public:
    StarSystem(int id, Star& star);

    void addPlanet(Planet& planet);
    void removePlanet(std::string& name);

    Star getStar();
    std::vector<Planet> getPlanets();

    void showSystem();
    int getId();
};
class Galaxy {
private:
    Graph<> systemGraph;
    std::vector<StarSystem> systems;
public:
    Galaxy();

    void addSystem(StarSystem& system);
    void connectSystems(int id1, int id2);

    void generateGalaxy(int numSystems);
    void showGalaxy();
};
#endif //GALAXY_H

#ifndef GALAXY_H
#define GALAXY_H
#include <vector>
#include "CelestialObjects.h"
#include "Graph.h"

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
class Galaxy {
private:
    std::string name;
    Graph<StarSystem*> systemGraph;
    std::vector<StarSystem> systems;
public:
    Galaxy();

    void addSystem(StarSystem& system);
    void connectSystems(int id1, int id2, int distance);
    Graph<StarSystem*> getGraph();
    void showGalaxy();
};
#endif //GALAXY_H

#include "Galaxy.h"

#include <algorithm>

StarSystem::StarSystem(int id, std::string naming, Star &star) : id(id), name(naming), centralStar(star) {
}


int StarSystem::getId() {
    return id;
}

std::vector<Planet> &StarSystem::getPlanets() {
    return planets;
}

Star &StarSystem::getStar() {
    return centralStar;
}

std::string StarSystem::getName() {
    return name;
}

void StarSystem::addPlanet(Planet &planet) {
    planets.push_back(planet);
}

void StarSystem::removePlanet(const std::string &name) {
    planets.erase(
        std::remove_if(planets.begin(), planets.end(),
                       [&](Planet &p) { return p.getName() == name; }),
        planets.end());
}

void StarSystem::showSystem() {
    std::cout << "Star system: " << name << "\n Central star: " << centralStar.getName();
    std::cout << "\n planets int orbit: \n";
    int k = 1;
    for (auto i: planets) {
        std::cout << k << ") " << i.getName() << std::endl;
        k++;
    }
}

Galaxy::Galaxy() {
}

void Galaxy::addSystem(StarSystem &system) {
    systems.push_back(system);
    systemGraph.addVertex(systems.back().getId(), &systems.back());
}

void Galaxy::connectSystems(int id1, int id2, int distance) {
    systemGraph.addEdge(id1, id2, distance);
}

void Galaxy::showGalaxy() {
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

Graph<StarSystem *>& Galaxy::getGraph() {
    return systemGraph;
}


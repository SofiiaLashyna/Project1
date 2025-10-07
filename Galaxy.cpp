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



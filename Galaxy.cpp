#include "StarSystem.h"

StarSystem::StarSystem(int id, std::string naming, Star &star) : id(id), CelestialObject(naming, star.getMass()), centralStar(star) {
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


void StarSystem::addPlanet(Planet &planet) {
    planets.push_back(planet);
    mass += planet.getMass();
}

void StarSystem::removePlanet(const std::string &name) {
    auto it = std::find_if(planets.begin(), planets.end(),
                           [&](Planet &p) { return p.getName() == name; });
    if (it != planets.end()) {
        mass -= it->getMass();
        planets.erase(it);
    }
}

void StarSystem::displayInfo() const{
    std::cout << "Star system: " << name << "\n Central star: " << centralStar.getName();
    std::cout << "\n planets in orbit: \n";
    int k = 1;
    for (auto i: planets) {
        std::cout << k << ") " << i.getName() << std::endl;
        k++;
    }
}

std::string StarSystem::getType() const {

    return "Star system";
}



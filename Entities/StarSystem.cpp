#include "StarSystem.h"

StarSystem::StarSystem(int id, std::string naming, Star *star) : id(id), CelestialObject(naming, star->getMass()), centralStar(star) {
}

StarSystem::~StarSystem() {
    delete centralStar;
    for (Planet *p: planets) {
        delete p;
    }
}

int StarSystem::getId() {
    return id;
}

std::vector<Planet*> &StarSystem::getPlanets() {
    return planets;
}

Star &StarSystem::getStar() {
    return *centralStar;
}


void StarSystem::addPlanet(Planet *planet) {
    if (planet == nullptr) return;
    planets.push_back(planet);
    double oldMass = getMass();
    setMass(oldMass+(*planet).getMass());
    sortPlanetsByOrbit();
}

void StarSystem::removePlanet(const std::string &name) {
    auto it = std::find_if(planets.begin(), planets.end(),
                       [&](Planet *p) { return p->getName() == name; });
    if (it != planets.end()) {
        double oldMass = getMass();
        Planet *planetToRemove = *it;
        setMass(oldMass - planetToRemove->getMass());

        planets.erase(it);
        delete planetToRemove;;
    }
}

void StarSystem::removePlanet() {
    planets.pop_back();
}


void StarSystem::displayInfo() const{
    std::cout << "Star system: " << getName() << "\n Central star: " << centralStar->getName();
    std::cout << "\n planets in orbit: \n";
    int k = 1;
    for (auto i: planets) {
        std::cout << k << ") " << i->getName() << std::endl;
        k++;
    }
}

std::string StarSystem::getType() const {

    return "StarSystem";
}

double StarSystem::calculateMass() {
    double mass =0;
    mass += centralStar->getMass();
    for (auto i : planets) {
        mass += i->getMass();
    }
    return mass;
}


void StarSystem::lifeExists(Planet& planet) {
    double radius = planet.getOrbit();
    if(centralStar->getStarType() == Star::starType::Main_sequence_Star) {
        if(radius>= 0.8 && radius<= 30)
            planet.setLifeExistence(true);
        else
            planet.setLifeExistence(false);
    }
    else if(centralStar->getStarType() == Star::starType::Red_Dwarf) {
        if(radius>=0.01 && radius<=0.3)
            planet.setLifeExistence(true);
        else
            planet.setLifeExistence(false);
    }

    else if(centralStar->getStarType() == Star::starType::Red_Giant) {
        if(radius>=10 && radius<=100)
            planet.setLifeExistence(true);
        else
            planet.setLifeExistence(false);
    }

    else if(centralStar->getStarType() == Star::starType::White_Dwarf) {
        if(radius>=0.001 && radius<=0.02)
            planet.setLifeExistence(true);
        else
            planet.setLifeExistence(false);
    }

    else if(centralStar->getStarType() == Star::starType::Brown_Dwarf) {
        if(radius>=0.0005 && radius<=0.002)
            planet.setLifeExistence(true);
        else
            planet.setLifeExistence(false);
    }

    else if(centralStar->getStarType() == Star::starType::Neutron_Star) {
        planet.setLifeExistence(false);
    }
}

void StarSystem::sortPlanetsByOrbit() {
    std::sort(planets.begin(), planets.end(),
          [](const Planet* a, const Planet* b) {
              return a->getOrbit() < b->getOrbit();
          });

}
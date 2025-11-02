#include "Planet.h"
#include <iostream>

Planet::Planet(const std::string &name, double mass, double orbit, double speed, double angle,
               planetType type, bool life) : CelestialObject(name, mass), orbitRadius(orbit), orbitSpeed(speed),
                                  inclination(angle), planet_type(type), habitable(life) {
    if (mass<=0)
        throw std::invalid_argument("Mass should be above zero!");
    if(orbit<= 0)
        throw std::invalid_argument("Planet can't exist that close to a star!");

}


void Planet::displayInfo() const {
    std::cout << "Planet name: " << name << " type: ";
    switch (planet_type) {
        case planetType::Terrestrial_Planet:
            std::cout << "Terrestrial planet";
            break;
        case planetType::Dwarf:
            std::cout << "Dwarf planet";
            break;
        case planetType::Gas_Giant:
            std::cout << "Giant planet";
            break;
    }
    std::cout << " life ";
    if(habitable)
        std::cout << "exists";
    std::cout << "doesn't exists";
}

std::string Planet::getType() const {
    return "Planet";
}

bool Planet::isHabitable() {
    return  habitable;
}

void Planet::setLifeExistence(bool life) {
    habitable = life;
}

Planet::planetType Planet::getPlanetType() {
    return planet_type;
}


double Planet::getOrbit() const{
    return orbitRadius;
}

QColor Planet::getColor() {
    return color;
}

void Planet::setColor(QColor newColor) {
    color = newColor;
}



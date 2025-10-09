#include "Planet.h"
#include <iostream>

Planet::Planet(const std::string &name, double mass, double orbit, double speed, double angle,
               planetType type) : CelestialObject(name, mass), orbitRadius(orbit), orbitSpeed(speed),
                                  currentAngle(angle), planet_type(type) {
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
}

std::string Planet::getType() const {
    return "Planet";
}

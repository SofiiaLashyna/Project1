#include "Nebula.h"

#include <iostream>

Nebula::Nebula(const std::string &name, double mass, nebulaType type) : CelestialObject(name, mass), nebula_type(type) {
}


void Nebula::displayInfo() const {
    std::cout << "Nebula name: " << name << " type: ";
    switch (nebula_type) {
        case nebulaType::Dark:
            std::cout << "Dark nebula";
            break;
        case nebulaType::Emission:
            std::cout << "Emission nebula";
            break;
        case nebulaType::Planetary:
            std::cout << "Planetary nebula";
            break;
        case nebulaType::Supernova:
            std::cout << "Supernova nebula";
            break;
        case nebulaType::Reflection:
            std::cout << "Reflection nebula";
            break;
    }
}


std::string Nebula::getType() const {
    return "Nebula";
}

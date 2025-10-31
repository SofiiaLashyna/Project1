#include "Nebula.h"

#include <iostream>

Nebula::Nebula(const std::string &name, double mass, nebulaType type) : CelestialObject(name, mass), nebula_type(type) {
    if (mass<=0)
        throw std::invalid_argument("Mass should be above zero!");

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

Nebula::nebulaType Nebula::getNebulaType() {
    return nebula_type;
}


void Nebula::setNebulaType(std::string newtype) {
    if(newtype == "Reflection nebula" || newtype == "reflection nebula")
        nebula_type = nebulaType::Reflection;
    if(newtype == "Emission nebula" || newtype == "emission nebula")
        nebula_type = nebulaType::Emission;
    if(newtype == "Supernova" || newtype == "supernova")
        nebula_type = nebulaType::Supernova;
    if(newtype == "Planetary nebula" || newtype == "planetary nebula")
        nebula_type = nebulaType::Planetary;
    if(newtype == "Dark nebula" || newtype == "dark nebula")
        nebula_type = nebulaType::Dark;
}

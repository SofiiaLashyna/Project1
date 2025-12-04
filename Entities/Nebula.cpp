#include "Nebula.h"

#include <iostream>

Nebula::Nebula(const std::string &name, double mass, nebulaType type) : CelestialObject(name, mass), nebula_type(type) {
    if (mass<=0)
        throw std::invalid_argument("Mass should be above zero!");

}


void Nebula::displayInfo() const {
    std::cout << "Nebula name: " << getName() << " type: ";
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

std::string Nebula::getNebulaTypeString() const {
    switch (nebula_type) {
        case nebulaType::Dark: return "Dark";
        case nebulaType::Emission: return "Emission";
        case nebulaType::Planetary: return "Planetary";
        case nebulaType::Supernova: return "Supernova";
        case nebulaType::Reflection: return "Reflection";
    }
    return "Unknown";
}

void Nebula::setNebulaType(std::string newtype) {
    if(newtype == "Reflection" || newtype == "reflection")
        nebula_type = nebulaType::Reflection;
    if(newtype == "Emission" || newtype == "emission")
        nebula_type = nebulaType::Emission;
    if(newtype == "Supernova" || newtype == "supernova")
        nebula_type = nebulaType::Supernova;
    if(newtype == "Planetary" || newtype == "planetary")
        nebula_type = nebulaType::Planetary;
    if(newtype == "Dark" || newtype == "dark")
        nebula_type = nebulaType::Dark;
}

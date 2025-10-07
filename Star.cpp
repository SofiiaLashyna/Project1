#include "Star.h"

#include <iostream>

Star::Star(const std::string &name, double mass, double temperature, starType type) : CelestialObject(name, mass),
    temperature(temperature), star_type(type) {
}


std::string Star::getType() const {
    return "Star";
}

void Star::displayInfo() const {
    std::cout << "Star name: " << name << " type: ";
    switch (star_type) {
        case starType::Red_Giant:
            std::cout << "Red giant";
            break;
        case starType::White_Dwarf:
            std::cout << "White dwarf";
            break;
        case starType::Brown_Dwarf:
            std::cout << "Brown dwarf";
            break;
        case starType::Red_Dwarf:
            std::cout << "Red dwarf";
            break;
        case starType::Main_sequence_Star:
            std::cout << "Main sequence star";
            break;
        case starType::Neutron_Star:
            std::cout << "Neutron star";
            break;
    }
    std::cout << " temperature: " << temperature;
}

#include "Star.h"

#include <iostream>

Star::Star(const std::string &name, double mass, double temperature, starType type) : CelestialObject(name, mass),
    temperature(temperature), star_type(type) {
    if (mass<=0)
        throw std::invalid_argument("Mass should be above zero!");
    if(temperature<= 0)
        throw std::invalid_argument("Planet can't exist that close to a star!");

}


Star::starType Star::getStarType() {
    return star_type;
}


std::string Star::getType() const {
    return "Star";
}

void Star::setStarType(std::string type) {
    if (type == "Red giant" || type == "red giant")
        star_type = starType::Red_Giant;
    else if (type == "White dwarf" || type == "white dwarf")
        star_type = starType::White_Dwarf;
    else if (type == "Brown dwarf" || type == "brown dwarf")
        star_type = starType::Brown_Dwarf;
    else if (type == "Red dwarf" || type == "red dwarf")
        star_type = starType::Red_Dwarf;
    else if (type == "Main sequence star" || type == "main sequence star")
        star_type = starType::Main_sequence_Star;
    else if (type == "Neutron star" || type == "neutron star")
        star_type = starType::Neutron_Star;

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
    std::cout << " temperature: " << temperature <<std::endl;
}

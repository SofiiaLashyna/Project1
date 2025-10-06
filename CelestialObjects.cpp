#include "CelestialObjects.h"

#include <iostream>

CelestialObjects::CelestialObjects(const std::string &name, double mass) : name(name), mass(mass){}
Planet::Planet(const std::string &name, double mass, double orbit, double speed, double angle, planetType type) : CelestialObjects(name, mass), orbitRadius(orbit), orbitSpeed(speed), currentAngle(angle), planet_type(type){}
Star::Star(const std::string &name, double mass, double temperature, starType type) : CelestialObjects(name, mass), temperature(temperature), star_type(type){}
Nebula::Nebula(const std::string &name, double mass, nebulaType type) : CelestialObjects(name, mass), nebula_type(type){}





void Star::displayInfo() const {
    std::cout << "Star name: " << name << " type: ";
    switch (star_type) {
        case starType::Red_Giant:
            std::cout << "Red giant";
            break;
        case  starType::White_Dwarf:
            std::cout << "White dwarf";
            break;
        case  starType::Brown_Dwarf:
            std::cout << "Brown dwarf";
            break;
        case  starType::Red_Dwarf:
            std::cout << "Red dwarf";
            break;
        case  starType::Main_sequence_Star:
            std::cout << "Main sequence star";
            break;
        case  starType::Neutron_Star:
            std::cout << "Neutron star";
            break;
    }
    std::cout << " temperature: " << temperature;
}
void Planet::displayInfo() const {

        std::cout << "Planet name: " << name << " type: ";
    switch (planet_type){
        case planetType::Terrestrial_Planet:
            std::cout<<"Terrestrial planet";
            break;
        case planetType::Dwarf:
            std::cout<<"Dwarf planet";
            break;
        case planetType::Gas_Giant:
            std::cout<<"Giant planet";
            break;
    }
}
void Nebula::displayInfo() const {
    std::cout<< "Nebula name: " << name << " type: ";
    switch (nebula_type){
        case nebulaType::Dark:
            std::cout<<"Dark nebula";
            break;
        case nebulaType::Emission:
            std::cout<<"Emission nebula";
            break;
        case nebulaType::Planetary:
            std::cout<<"Planetary nebula";
            break;
        case nebulaType::Supernova:
            std::cout<<"Supernova nebula";
            break;
        case nebulaType::Reflection:
            std::cout<<"Reflection nebula";
            break;
    }
}
std::string Planet::getType() const {
    return "Planet";
}
std::string Star::getType() const {
    return "Star";
}
std::string Nebula::getType() const {
    return "Nebula";
}
std::string CelestialObjects::getName() {
    return name;
}
double CelestialObjects::getMass() {
    return mass;
}



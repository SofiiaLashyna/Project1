#include "CelestialObject.h"

#include <iostream>

CelestialObject::CelestialObject(const std::string &name, double mass) : name(name), mass(mass){}

std::string CelestialObject::getName() const{
    return name;
}
double CelestialObject::getMass() const{
    return mass;
}
void CelestialObject::setName(const std::string& newName) {
    name = newName;
}

void CelestialObject::setMass(const double newMass) {
    if (newMass <= 0) {
        throw std::invalid_argument("Mass must be greater than zero.");
    }
    mass = newMass;
}


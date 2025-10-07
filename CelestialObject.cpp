#include "CelestialObject.h"

#include <iostream>

CelestialObject::CelestialObject(const std::string &name, double mass) : name(name), mass(mass){}

std::string CelestialObject::getName() const{
    return name;
}
double CelestialObject::getMass() const{
    return mass;
}



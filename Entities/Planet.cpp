#include "Planet.h"
#include <iostream>

Planet::Planet(const std::string &name, double mass, double orbit, double speed, double angle,
               planetType type, bool life) : CelestialObject(name, mass), orbitRadius(orbit), orbitSpeed(speed),
                                  inclination(angle), planet_type(type), habitable(life) {
    if (mass<=0)
        throw std::invalid_argument("Mass should be above zero!");
    if(orbit<= 0)
        throw std::invalid_argument("Planet can't exist that close to a star!");
    rings = false;
}


void Planet::displayInfo() const {
    std::cout << "Planet name: " << getName() << " type: ";
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

bool Planet::isHabitable() const {
    return  habitable;
}

void Planet::setLifeExistence(bool life) {
    habitable = life;
}

Planet::planetType Planet::getPlanetType() const {
    return planet_type;
}


double Planet::getOrbit() const{
    return orbitRadius;
}

void Planet::setOrbit(double newOrbit) {
    orbitRadius = newOrbit;
}


RGBColor Planet::getColor() const{
    return color;
}

void Planet::setTexturePath(const std::string& path) {
    texturePath = path;
}

std::string Planet::getTexturePath() const {
    return texturePath;
}

void Planet::setColor(int r, int g, int b, int a) {
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
}

void Planet::setRings(bool hasRings, double innerFactor, double outerFactor, int r, int g, int b, int a) {
    rings = hasRings;
    ringInnerRadiusFactor = innerFactor;
    ringOuterRadiusFactor = outerFactor;
    ringColor.r = r;
    ringColor.g = g;
    ringColor.b = b;
    ringColor.a = a;
}

bool Planet::hasRings() const {
    return rings;
}

double Planet::getRingInnerFactor() const {
    return ringInnerRadiusFactor;
}

double Planet::getRingOuterFactor() const {
    return ringOuterRadiusFactor;
}

RGBColor Planet::getRingColor() const {
    return ringColor;
}

double Planet::getInclination() const {
    return inclination;
}

void Planet::setInclination(double angle) {
    inclination = angle;
}


double Planet::getSpeed() const {
    return orbitSpeed;
}

void Planet::setSpeed(double newSpeed) {
    orbitSpeed = newSpeed;
}

std::string Planet::getPlanetTypeString() const {
    switch (planet_type) {
        case planetType::Dwarf:
            return "Dwarf";
        case planetType::Gas_Giant:
            return "Gas_Giant";
        case planetType::Terrestrial_Planet:
            return "Terrestrial_Planet";
    }
    return "Unknown_Planet_Type";
}

void Planet::setPlanetType(std::string type) {
    if (type == "Gas_Giant" || type == "Gas Giant" || type == "gas giant")
        planet_type = planetType::Gas_Giant;
    else if (type == "Terrestrial_Planet" || type == "Terrestrial Planet" || type == "terrestrial planet")
        planet_type = planetType::Terrestrial_Planet;
    else if (type == "Dwarf" || type == "dwarf")
        planet_type = planetType::Dwarf;
}

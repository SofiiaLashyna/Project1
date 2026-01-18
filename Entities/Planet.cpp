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

QColor Planet::getColor() const{
    return color;
}

void Planet::setTexturePath(const std::string& path) {
    texturePath = path;
}

std::string Planet::getTexturePath() const {
    return texturePath;
}

void Planet::setColor(QColor newColor) {
    color = newColor;
}

void Planet::setRings(bool hasRings, double innerFactor, double outerFactor, QColor rColor) {
    rings = hasRings;
    ringInnerRadiusFactor = innerFactor;
    ringOuterRadiusFactor = outerFactor;
    ringColor = rColor;
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

QColor Planet::getRingColor() const {
    return ringColor;
}


#ifndef STARSYSTEM_H
#define STARSYSTEM_H
#include <vector>
#include <algorithm>
#include <iostream>
#include "CelestialObject.h"
#include "Planet.h"
#include "Star.h"

class StarSystem : public CelestialObject {
private:
    int id;
    Star &centralStar;
    std::vector<Planet> planets;

    void sortPlanetsByOrbit();

public:
    StarSystem(int id, std::string name, Star &star);

    void addPlanet(Planet *planet);

    void removePlanet();

    void removePlanet(const std::string &name);

    Star &getStar();

    std::vector<Planet> &getPlanets();

    void displayInfo() const override;

    std::string getType() const override;

    double calculateMass();

    int getId();

    void lifeExists(Planet &planet);
};
#endif //STARSYSTEM_H

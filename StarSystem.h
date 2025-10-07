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
    Star centralStar;
    std::vector<Planet> planets;

public:
    StarSystem(int id, std::string name, Star &star);

    void addPlanet(Planet &planet);

    void removePlanet(const std::string &name);

    Star &getStar();

    std::vector<Planet> &getPlanets();

    void displayInfo() const override;

    std::string getType() const override;

    int getId();

};
#endif //STARSYSTEM_H

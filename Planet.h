#ifndef PLANET_H
#define PLANET_H
#include "CelestialObject.h"

class Planet : public CelestialObject {
private:
    double orbitRadius;
    double orbitSpeed;
    double currentAngle;

public:
    enum class planetType { Gas_Giant, Dwarf, Terrestrial_Planet };

    planetType planet_type;

    Planet(const std::string &name, double mass, double orbit, double speed, double angle, planetType type);

    void displayInfo() const override;

    std::string getType() const override;
};

#endif //PLANET_H
